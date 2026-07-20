#include "main.h"

// ================= 全局变量定义 =================
uint8_t key = 0;
uint16_t voltage = 0;           // DAC输出电压值 (10mV单位, 0~500对应0~5.00V)
uint16_t v_cnt = 0;               // Timer0计数器(100us/tick)
uint16_t alarm_timer = 0; 
uint16_t set_temp = 0,parameter=28;
uint16_t temperature = 2500;      // 温度值 (乘以100, 单位℃, 初始25.00℃)
// 外部变量
extern uint8_t LedBuff[8];
extern uint8_t code LedChar[];
// 电压显示段码: U _ _ _ _ X. X X
uint8_t v_char[8] = {0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
// 参数显示段码: P _ _ _ _ _ D D
uint8_t p_char[8] = {0x8C, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
// 温度显示段码: C _ _ _ D D . D D
uint8_t c_char[8] = {0xC6, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// 状态变量
uint8_t show_mode = DISP_C;  // 0=温度显示界面, 1=电压输出界面, 2=参数设置界面 (初始:温度显示界面)
bit key5_mode = 0;     // 0=模式1：输出电压与温度相关, 1=模式2：电压按照图7给出的关系输出
sbit BUZZ  = P5^4;     //蜂鸣器0-响；1-停
bit Alarm_Active = 0;  //0-闹钟关闭；1-开启闹钟 

// 函数声明
void Adc_run(void);
void Seg_Run(void);
void Key_Run(void);
void Led_Run(void);
// ================= 主函数 =================
void main(void)
{
	GPIO();
	DS18B20_Init();   
	BUZZ = 1;
    Timer0_Init();          
    Timer1_Init();
	
    // ADC初始化 (通道3, 内部2.5V参考)
    ADCCFG   |= 0x20;           // ADC时钟=SYSCLK/2 (12MHz@24MHz)
    ADC_CONTR |= 0x80;          // 开启ADC电源
	
    while(1) 
    {
        Key_Run();
        Adc_run();  
        Seg_Run();

    }
}

// ================= Timer1中断 (1ms, 时基+频率测量门控) =================
void timer1() interrupt 3
{
    TL1 = 0x18;                 // 手动重装: 65536-1000=64536=0xFC18
    TH1 = 0xFC;

	v_cnt++;
   if(temperature > parameter * 100)
    {
        if(Alarm_Active == 0)
        {
            BUZZ = 0;
            alarm_timer++;
            if(alarm_timer >= 5000|P0 !=0x0F)
            {
                BUZZ = 1;
                Alarm_Active = 1;
                alarm_timer = 0;
            }
        }
        // Alarm_Active=1时，温度还超标，直接跳过，保持静音
    }
    else
    {
        // 温度正常，全部复位
        BUZZ = 1;
        alarm_timer = 0;
        Alarm_Active = 0;
    }


    if(v_cnt & 0x01)            // 奇数tick: 数码管扫描
    {                           
        Seg_Scan();
    } 
    else                        // 偶数tick: LED刷新
    {                           
        P7 = 0xFF;              // 先关位选
        P6 = 0xFF;              // ★清P6再切电源, 防止段码串扰LED
        P40 = 0;                // 开LED电源(清空后再开)
        Led_Run();
    }
}
// ================= 显示缓存更新 =================
void Seg_Run(void)
{
    uint8_t i;
	if(show_mode == DISP_C)  // 温度显示界面
		{
			c_char[4] = LedChar[temperature /1000];  // 十位 (2)
			c_char[5] = LedChar[temperature /100%10] & 0x7F;  // 个位带小数点 (5.)
			c_char[6] = LedChar[temperature /10%10];  // 小数点后第一位 (4)
			c_char[7] = LedChar[temperature % 10];  // 小数点后第二位 (2)
			
			for(i = 0; i < 8; i++)
				LedBuff[i] = c_char[i];
		} 
	else if(show_mode == DISP_P)  // 参数设置界面
		{
			// 参数 parameter 是整数，范围0-99
			p_char[6] = LedChar[parameter / 10];  // 十位
			p_char[7] = LedChar[parameter % 10];  // 个位
			
			for(i = 0; i < 8; i++)
				LedBuff[i] = p_char[i];
		} 
	 else if(show_mode == DISP_U)  // 电压输出界面
		{		

			v_char[5] = LedChar[voltage / 100]& 0x7F;        // 整数位
			v_char[6] = LedChar[voltage % 100/ 10] ;    // 小数点后第一位，带小数点
			v_char[7] = LedChar[voltage % 100% 10];           // 小数点后第二位
			
			for(i = 0; i < 8; i++)
				LedBuff[i] = v_char[i];
		} 
}

// ================= 温度采集 & 输出控制 & 数据刷新=================
void Adc_run(void)
{
    if(v_cnt >= 500)
	{
		v_cnt = 0;
		EA = 0;

		// 更新温度值
		Start18B20();//关键
		temperature = Get18B20Temp();  // 温度乘以100
		
		// 始终读取ADC，计算电压和频率
		voltage = Get_ADC12bitResult(3);
		voltage = (250UL * voltage) / 4096;          // 0~250 (0~2.50V)
		
		EA = 1;	 
	}
	
}

// ================= 按键处理 (S4~S7) =================
void Key_Run(void)
{
    key = Key_scan();
    switch(key) {

		// S4: 显示界面切换 (温度->电压->参数->温度...)
        case 4: 
            show_mode++; 
            if(show_mode == 3) show_mode = DISP_C;  // 循环 0->1->2->0
            break;

		// S5: 模式切换 (模式1<-->模式2)        
		case 5:
            key5_mode = !key5_mode;
            break;

		// S6: 温度参数减1
        case 6:
            if(show_mode == DISP_P) {
                if(parameter > 0) parameter--;
                else parameter = 99;  // 循环
            }
            break;

		// S7: 温度参数加1
        case 7:
            if(show_mode == DISP_P) {
                if(parameter < 99) parameter++;
                else parameter = 0;  // 循环
            }
 
        default: break;	

    }
}
void Led_Run(void)
{
    P6 |= 0x0F;       
    if(key5_mode == 1){   
    switch(show_mode)// 界面指示灯 
		{
        case 0:P6 &= ~0x01;break;
        case 1:P6 &= ~0x02;break;
        case 2:P6 &= ~0x04;break;		
        default:break;
		}
	}
}
