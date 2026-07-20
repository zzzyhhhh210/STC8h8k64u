#include "main.h"

// ================= 全局变量定义 =================
//定时器
uint16_t cnt = 0;           // LED流转计数器(单位:0.1ms)
uint16_t blink_cnt = 0;     // 闪烁计数器(0.8s)
uint8_t pwm_cnt = 0;        // PWM计数器(0-199, 20ms周期)
uint8_t bright_tab[] = {50, 100, 150, 200};  // 4档亮度PWM阈值
//s1s4模式
uint8_t key1_num = 0;        // 设置状态: 0=全灭, 1=模式编号, 2=流转间隔
uint8_t key4_num = 1;       // 亮度等级 1~4(Key.c引用)
//led
uint8_t Set_led_mode = 1;   // 设置运行模式 1
uint8_t led_mode = 1;       // 默认模式1， 1~4
uint8_t index = 0;          // LED流转索引
uint8_t P6_OUT = 0xFF;      // LED输出锁存值

// EEPROM: 各模式流转间隔(单位:100ms, 范围4~12即400~1200ms)
unsigned char time = 4;
// 数码管段码表(共阳极)
unsigned char code LedChar[] = {
    0xC0,   // 0
    0xF9,   // 1
    0xA4,   // 2
    0xB0,   // 3
    0x99,   // 4
    0x92,   // 5
    0x82,   // 6
    0xF8,   // 7
    0x80,   // 8
    0x90,   // 9
    0xBF,   // 10: '-'
    0xFF,   // 11: 熄灭
};
// 显示缓存
uint8_t LedBuff[8];                         // 数码管段码缓存(直接段码值)
uint8_t set_char[8];                        // 设置界面段码
uint8_t led_lec[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};  // Key.c引用
// 标志位
bit flag = 0;           // 启动/停止
bit flash_state = 0;    // 闪烁状态(0.8s翻转)
//#define close_mode  0
//#define set_mode  1
bit mode = close_mode;  // 默认为关闭数码管
bit led_f = 0;          // S3按下显示亮度(Key.c引用)
// ================= 函数声明 =================
void Led_Run(void);
void Key_Run(void);
void Seg_Run(void);
void Update_Set_Char(void);
void Save_Settings(void);
void Load_Settings(void);
// ================= 主函数 =================
void main(void)
{
    // 初始化
    I2C_Init();
    Key_Init();
    LED_Init();
    Nix_Init();
    Timer0_Init();

    // 从EEPROM加载保存的间隔值
    Load_Settings();

    // 初始: 数码管全灭
    Seg_Run();

    while(1)
    {
        Key_Run();
        // 刷新显示状态(非设置模式下,处理S3松手后熄灭等场景)
        if(mode == close_mode)
            Seg_Run();
    }
}

// ================= 定时器中断 (100us) =================
// 功能: LED流转计时 + 0.8s闪烁 + PWM亮度 + SegScan
void timer0() interrupt 1
{
    static uint8_t seg_pos = 0;
    // ---------- 1. LED流转计数器 ----------
    cnt++;
    if(cnt >= time * 1000)  // 转换为0.1ms单位
    {
        cnt = 0;
        if(flag == 1)//启动
            Led_Run();      // 执行一次LED移位
    }
    // ---------- 2. 闪烁计数器(0.8s = 8000 * 0.1ms) ----------
    blink_cnt++;
    if(blink_cnt >= 8000)
    {
        blink_cnt = 0;
        flash_state = ~flash_state;      // 翻转闪烁状态
        if(set_mode == 1)
            Seg_Run();                  // 刷新显示(实现闪烁)
    }
    // ---------- 3. PWM亮度控制 + SegScan(20ms周期) ----------
    pwm_cnt++;
    if(pwm_cnt >= 200)  pwm_cnt = 0;
    if(pwm_cnt <= 30)
    {
        // ===== 数码管扫描时间(3ms/20ms) =====
        P40 = 1;                        // 关闭LED电源
        P7 = 0xFF;                      // 消影
        P6 = LedBuff[seg_pos];          // 输出段码
        P7 = ~(0x01 << seg_pos);        // 位选
        seg_pos = (seg_pos + 1) % 8;    // 切换下一位
    }
    else if(pwm_cnt < bright_tab[key4_num - 1])
    {
        // ===== LED点亮时间 =====
        P7 = 0xFF;      // 关闭数码管位选
        P40 = 0;        // 开启LED电源
        P6 = P6_OUT;    // 输出LED灯状态
    } 
    else
    {
        // ===== LED熄灭时间 =====
        P40 = 1;        // 关闭LED电源
    }
}
// ================= 按键处理函数 =================
void Key_Run(void)
{
	uint8_t key;
    key = Key_scan();
    switch(key)
    {
        // ========== S0: 启动/停止 ==========
        case 0:
            flag = ~flag;break;
        // ========== S1: 设置(循环: 全灭→模式→间隔→全灭) ==========
        case 1:
			Set_led_mode = led_mode;//同步当前模式
			key1_num++;mode = set_mode;
            Update_Set_Char();      // 更新设置界面内容
            Seg_Run();             // 刷新显示
            if(key1_num >= 3)
            {
                key1_num = 0;
				mode = close_mode;
				
                Seg_Run();          // 全灭
                Save_Settings();    // 退出时保存到EEPROM
            }
            break;
        // ========== S2: 加 ==========
        case 2:
            if(mode == close_mode) break;
            if(key1_num == 1 && mode == set_mode)             // 模式编号+1
            {
                Set_led_mode++;
                if(Set_led_mode > 4)Set_led_mode = 1;
            }
            else if(key1_num == 2)         // 间隔+100ms(上限1200ms)
            {
                if(time < 12)
                time++;
            }
            Update_Set_Char();
            Seg_Run();
            break;
        // ========== S3: 减(=在设置模式) / 显示亮度(=在非设置模式) ==========
        case 3:
            if(mode == set_mode)
            {
                if(key1_num == 1)             // 模式编号-1
                {
                    Set_led_mode--;
                    if(Set_led_mode < 1) Set_led_mode = 4;
                }
                else if(key1_num == 2)         // 间隔-100ms(下限400ms)
                {
                    if(time > 4)
                        time--;
                }
                Update_Set_Char();
                Seg_Run();
            }
            // 非设置模式: Key_scan已设置led_f=1并调用Seg_Run
            // 松手后led_f=0, 下次主循环Key_Run会刷新
            break;
        // ========== S4: 亮度等级控制(1→2→3→4→1) ==========
        case 4:
            key4_num++;
            if(key4_num > 4) key4_num = 1;
            break;
        default:
            break;
    }
}
// ================= 更新设置界面显示缓存 =================
void Update_Set_Char(void)
{
    uint16_t val;
    uint8_t i;
	val = time * 100;     // 转为实际ms值默认，400==>4

	// 状态1: 显示运行模式编号
    // 格式: "- [mode] -    [百]  [十]  [个]"
	//问题2：数码管显示3位，当val>900时，显示错乱
	//解决方法：分别在Update_Set_Char();设置缓存与Seg_Run();闪烁的添加判断时间的大小
    if(key1_num == 1)
    {
       
        for(i = 0; i < 8; i++)
        set_char[i] = LedChar[11];          // 默认熄灭

        set_char[7] = LedChar[10];              // '-'
        set_char[6] = LedChar[Set_led_mode];        // 模式编号
        set_char[5] = LedChar[10];              // '-'
		
		
		if(time<10){
        set_char[2] = LedChar[val / 100];        // 百位
        set_char[1] = LedChar[val / 10 % 10];   // 十位
		set_char[0] = LedChar[val % 10];        // 个位
}
		else if(time>9&&time<13){
		set_char[3] = LedChar[val / 1000];        // 千位
        set_char[2] = LedChar[val / 100% 10];        // 百位
        set_char[1] = LedChar[val / 10 % 10];   // 十位
		set_char[0] = LedChar[val % 10];        // 个位
}
    }
	// 状态2: 显示流转间隔(ms)
    // 格式: "- [mode] -    [百]  [十]  [个]"
    else if(key1_num == 2)
    {
             
        for(i = 0; i < 8; i++)
        set_char[i] = LedChar[11];          // 默认熄灭

		set_char[7] = LedChar[10];              // '-'
        set_char[6] = LedChar[Set_led_mode];        // 模式编号
        set_char[5] = LedChar[10];              // '-'
	
		if(time<10){
        set_char[2] = LedChar[val / 100];        // 百位
        set_char[1] = LedChar[val / 10 % 10];   // 十位
		set_char[0] = LedChar[val % 10];        // 个位
}
		else if(time>9&&time<13){
		set_char[3] = LedChar[val / 1000];        // 千位
        set_char[2] = LedChar[val / 100% 10];        // 百位
        set_char[1] = LedChar[val / 10 % 10];   // 十位
		set_char[0] = LedChar[val % 10];        // 个位
}

    }
}
// ================= 数码管显示刷新 =================
void Seg_Run(void)
{
    uint8_t i;
    // 非设置模式下 S3 按住: 显示亮度等级
    // 格式: "         - [level]"
    if(led_f == 1 && mode == close_mode && key1_num == 0)
    {
        for(i = 0; i < 6; i++)
        LedBuff[i] = LedChar[11];            // '熄灭'
        LedBuff[1] = LedChar[10];               // '-'
        LedBuff[0] = LedChar[key4_num];         // 亮度等级(1~4)
    }
    // 设置模式
    else if(mode == set_mode)
    {
        // 填入完整数据
        for(i = 0; i < 8; i++)
            LedBuff[i] = set_char[i];
        // 闪烁: flash_state=1时熄灭当前选中单元
        if(flash_state == 1)
        {
            if(key1_num == 1)//运行模式
            {
                LedBuff[7] = LedChar[11];       // 熄灭模式编号单元
				LedBuff[6] = LedChar[11];
				LedBuff[5] = LedChar[11];
            }   
            if(key1_num == 2)
            {
				if(time<10){
                LedBuff[2] = LedChar[11];       // 熄灭间隔值单元(百十个位)
                LedBuff[1] = LedChar[11];
                LedBuff[0] = LedChar[11];
            }
			else if(time>9&&time<13){
				LedBuff[3] = LedChar[11];		// 熄灭间隔值单元(千百十个位)
                LedBuff[2] = LedChar[11];       
                LedBuff[1] = LedChar[11];
                LedBuff[0] = LedChar[11];
            }
			}
        }
    }
    // 默认: 全灭
    else
    {
        for(i = 0; i < 8; i++)
            LedBuff[i] = LedChar[11];           // 全灭
    }
}
// ================= LED流水灯运行 =================
void Led_Run(void)
{
	//问题卡死在led为模式1
	//	led_mode = Set_led_mode;
    // 只在模式变化时同步，不每次覆盖！
    if(mode == set_mode)
    {
        if(led_mode != Set_led_mode)
        {
            led_mode = Set_led_mode;
            index = 0;  // 模式切换时复位位置
        }
    }
    switch(led_mode)
    {
        //模式1:L1→L8左到右单循环
        case 1:P6_OUT = _cror_(0x7F, index);index++;
            if(index >= 8){index = 0;led_mode = 2;}
            break;
        //模式2:L8→L1右到左单循环
        case 2:P6_OUT = _crol_(0xFE, index);index++;
            if(index >= 8){index = 0;led_mode = 3;}
            break;
        //模式3:L1+L8→L2+L7→L3+L6→L4+L5双向汇聚
        case 3:P6_OUT = _cror_(0x7F, index) & _crol_(0xFE, index);index++;
            if(index >= 4){index = 0;led_mode = 4;}
            break;
        //模式4:L4+L5→L3+L6→L2+L7→L1+L8双向发散
        case 4:P6_OUT = _crol_(0xEF, index) & _cror_(0xF7, index);index++;
            if(index >= 4){index = 0;led_mode = 1;}
            break;
        default:led_mode = 1;index = 0;
            break;
    }
}            