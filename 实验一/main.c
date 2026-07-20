#include "main.h"

// ================= 全局变量定义 =================
volatile uint8_t cnt = 0;    // 定时器计数，用于产生1s时基

/*温度相关位缓存*/
volatile uint8_t List[] = {0, 0, 0};
volatile uint8_t clok_tmp[4] = {0x80, 0xbe, 0, 0XC6}; //配置温度显示
volatile uint16_t temp0 = 0; 
/* 按键与显示状态变量*/
unsigned char Key_Val, Key_Down, Key_Up, Key_Old; //消影

/* RTC 相关 */
volatile uint8_t clok_time[8] = {0, 0, 0xbf, 0, 0, 0xbf, 0, 0};
volatile uint8_t Rtc_Save[3]={23,59,50};      // 时钟设置缓存: [0]=时, [1]=分, [2]=秒
uint8_t Rtc_flag = 0;         // 当前设置的索引: 0=时, 1=分, 2=秒

/* 设置模式变量 */
volatile uint8_t mode = SHOW_CLOCK; // 默认状态：显示时钟

/* 闹钟变量 */
volatile uint8_t Alarm_Save[3] = {0, 0, 0}; //初始闹钟时间00：00：00
volatile uint16_t Alarm_Timer_ms = 0; // 闹钟运行时间计时
volatile uint8_t Blink_State = 0;     // LED闪烁状态
volatile uint8_t Alarm_Blink_Cnt = 0; // 专门用于LED闪烁的计数器

bit Alarm_Active = 0;  //0-闹钟关闭；1-开启闹钟 
bit Seg_Star_Flag; // 闪烁标志位: 由定时器中断每1秒翻转一次
bit shouldBlink;   // 0-亮，1-灭
// ================= 函数声明 =================
void Key_Proc();
void Display_Setting_With_Blink(void); 
void Alarm(void);
// ================= 主函数 =================
void main(void)
{
    // 初始化
    P_SW2 |= 0x80;
    P5M1 = 0X00; P5M0 = 0X00; 
    Nix_Init();                 
    Key_Init();
    DS18B20_Init();
    I2C_Init(); 
    Timer0_Init();              
    RTC_config();
    IE2 |= 0x20;  
    EA = 1;
    temp0 = Get18B20Temp();
    Delay_ms(1000); 

    while (1)
    {
        Key_Proc();
		Alarm();
        // 【修改点2】根据整合后的状态执行不同逻辑
        switch (mode)
        {
            case SHOW_CLOCK:                  
                Display_Clock();    // 正常每秒更新一次
                break;
                
            case SHOW_TEMP:
                Temperature_Scan(); // 假设此函数在其它文件中实现，负责显示温度
                break;
                
            case SET_CLOCK:
            case SET_ALARM:
                Display_Setting_With_Blink();
                break;
                
            default:
                mode = SHOW_CLOCK; // 异常状态复位
                break;
        }
    }
}

// ================= 按键处理函数 =================
void Key_Proc()
{ 
    
    Key_Val = Key_scan();
    Key_Down = Key_Val & (~Key_Old); 
    Key_Up = ~Key_Val & Key_Old;     
    Key_Old = Key_Val;
    
    if (!Key_Down) return; // 无按键按下直接返回
   
    // 1.按键4用于切换时钟/温度////
    if ((mode == SHOW_CLOCK || mode == SHOW_TEMP) && Key_Down == 4)
    {
        if (mode == SHOW_CLOCK)
            mode = SHOW_TEMP;
        else
            mode = SHOW_CLOCK;
        return; 
    }

	// 2. 进入设置模式
    if (mode == SHOW_CLOCK || mode == SHOW_TEMP)
    {
        if (Key_Down == 7) //进入时钟设置
        {
            mode = SET_CLOCK;
            Rtc_flag = 0; //小时

            RTCCR = 0x00; // 停止 RTC           
            // 从只读寄存器读取当前时间到缓存，以便基于当前时间修改
            Rtc_Save[0] = HOUR; 
            Rtc_Save[1] = MIN; 
            Rtc_Save[2] = SEC;
        }
        else if (Key_Down == 6)//进入闹钟设置
        {
            mode = SET_ALARM;
            Rtc_flag = 0;//小时
        }
        return;
    }

    // 3. 在设置模式下操作
    switch (Key_Down) 
    {
        // ========== 按键7: 切换设置项 或 退出设置 ==========
        case 7: 
            if (mode == SET_CLOCK)
            {
                if (Rtc_flag < 2) 
                {
                    Rtc_flag++; 
                }
                else 
                {
                    
                    // 1.停止RTC
                    RTCCR = 0x00;
//                  RTCCFG |= 0x20; 
					//2.写入新时间到INI寄存器
                    INIHOUR = Rtc_Save[0];
                    INIMIN  = Rtc_Save[1];
                    INISEC  = Rtc_Save[2];                   
                    //关闭初始化使能（退出配置模式）
//					RTCCFG &= ~0x20; 
					// 3.将 INI 寄存器值写入 RTC 计数器
					RTCCFG |= 0x01;//关键
					//清除所有RTC中断标志（避免旧中断触发异常）
//					RTCIF = 0x00;
                    //4.重新启动RTC
                    RTCCR = 0x01;
                  
                    mode = SHOW_CLOCK; // 退出设置
                    Rtc_flag = 0;      // 重置标志
                }
            }
            else if (mode == SET_ALARM)
            {
                // 闹钟设置逻辑：仅切换光标，超过秒则退出
                if (++Rtc_flag > 2) 
                { 
                    mode = SHOW_CLOCK; 
                    Rtc_flag = 0;   
                }
            }
            break;
        case 6:
             if (mode == SET_ALARM || mode == SET_CLOCK)
             {
                 if (++Rtc_flag > 2) 
                 { 
                     mode = SHOW_CLOCK; 
                     Rtc_flag = 0;   
                 }
             }
             break;

      // ========== 按键5: 加 ==========
        case 5: 
            if (mode == SET_CLOCK)
            {
                if (Rtc_flag == 0) // 时
                    Rtc_Save[0] = (Rtc_Save[0] >= 23) ? 0 : Rtc_Save[0] + 1;
                else // 分或秒
                    Rtc_Save[Rtc_flag] = (Rtc_Save[Rtc_flag] >= 59) ? 0 : Rtc_Save[Rtc_flag] + 1;
            }
            else 
            {
                if (Rtc_flag == 0) // 时
                    Alarm_Save[0] = (Alarm_Save[0] >= 23) ? 0 : Alarm_Save[0] + 1;
                else // 分或秒
                    Alarm_Save[Rtc_flag] = (Alarm_Save[Rtc_flag] >= 59) ? 0 : Alarm_Save[Rtc_flag] + 1;
            }
            break;

        // ========== 按键4: 减 ==========
        case 4: 
            if (mode == SET_CLOCK)
            {
                if (Rtc_flag == 0) // 时
                    Rtc_Save[0] = (Rtc_Save[0] == 0) ? 23 : Rtc_Save[0] - 1;
                else // 分或秒
                    Rtc_Save[Rtc_flag] = (Rtc_Save[Rtc_flag] == 0) ? 59 : Rtc_Save[Rtc_flag] - 1;
            }
            else
            {
                if (Rtc_flag == 0) // 时
                    Alarm_Save[0] = (Alarm_Save[0] == 0) ? 23 : Alarm_Save[0] - 1;
                else // 分或秒
                    Alarm_Save[Rtc_flag] = (Alarm_Save[Rtc_flag] == 0) ? 59 : Alarm_Save[Rtc_flag] - 1;
            }
            break;
    }
}

// ================= 闹钟检测 =================
void Alarm(void)
{
    if (!Alarm_Active && HOUR == Alarm_Save[0] && MIN == Alarm_Save[1] && SEC == Alarm_Save[2])
    {
        Alarm_Active = 1;       
        Alarm_Timer_ms = 0;     
        Alarm_Blink_Cnt = 0;    // 重置闪烁计数
        LED_L1 = 0;             // 初始点亮 LED
        BUZZ = 0;            // 如果需要蜂鸣器响，在这里开启
    }
	// 任意键停止闹钟
    if (Alarm_Active && Key_Down) 
    {
        Alarm_Active = 0; 
		LED_L1 = 1; 
		 BUZZ = 1;
        return;
    }
}
// ================= 屏幕闪烁显示设置界面 =================
void Display_Setting_With_Blink(void)
{
    unsigned char i;
    unsigned char buf[8];
    unsigned char h, m, s;
    unsigned char dataIndex;
    
    // 根据 mode 决定读取哪个数据源
    if (mode == SET_CLOCK) {
        h = Rtc_Save[0]; m = Rtc_Save[1]; s = Rtc_Save[2];
    } else { // SET_ALARM
        h = Alarm_Save[0]; m = Alarm_Save[1]; s = Alarm_Save[2];
    }    
    buf[0] = h / 10; buf[1] = h % 10;
    buf[3] = m / 10; buf[4] = m % 10; 
    buf[6] = s / 10; buf[7] = s % 10;
    buf[2] = 0xBF;buf[5] = 0xBF; // 显示分隔符 '-'

    for (i = 0; i < 8; i++)
    {
        P7 = _crol_(0xfe, i);     
        shouldBlink = 0; // 默认全亮       
        if (Rtc_flag == 0 && (i == 6 || i == 7)) shouldBlink = 1; // 时
        if (Rtc_flag == 1 && (i == 3 || i == 4)) shouldBlink = 1; // 分
        if (Rtc_flag == 2 && (i == 0 || i == 1)) shouldBlink = 1; // 秒      
        if (shouldBlink && Seg_Star_Flag == 0) 
            P6 = 0xFF; // 熄灭，产生闪烁效果
        else 
        {
            dataIndex = 7 - i;  
            if (i == 2 || i == 5)
                P6 = 0xBF; // 显示分隔符
            else 
            {
                 if (buf[dataIndex] <= 9)
                     P6 = Nix[buf[dataIndex]];
                 else
                     P6 = 0xFF;
            }
        }
        
        Delay_us(500); 
        P6 = 0xFF;     
    }
}

// ================= 定时器中断 =================
void Timer0_Isr(void) interrupt 1
{
    TL0 = 0xB0; 
    TH0 = 0x3C;

    cnt++;
    if (cnt >= 10) //500ms
    {
        cnt = 0;
        Seg_Star_Flag = !Seg_Star_Flag; // 500ms翻转一次，用于闪烁
    }
	// --- 闹钟处理逻辑 ---
    if (Alarm_Active)
    {
        Alarm_Timer_ms += 50; // 累加总时间，用于判断5秒停止
        Alarm_Blink_Cnt++;

        if (Alarm_Blink_Cnt >= 4) 
        {
            Alarm_Blink_Cnt = 0; // 重置计数器
            LED_L1 = !LED_L1;    // 翻转 LED 状态
			 BUZZ = !BUZZ; 
        } 
        // --- 5秒自动停止逻辑 ---
        if (Alarm_Timer_ms >= 5000)
        {
            Alarm_Active = 0;       // 关闭闹钟标志
            Alarm_Timer_ms = 0;     // 重置总计时
            Alarm_Blink_Cnt = 0;    // 重置闪烁计数
            LED_L1 = 1;             // 确保 LED 熄灭 (假设高电平灭)
            BUZZ = 1;               // 确保蜂鸣器停止
        }
    }
}