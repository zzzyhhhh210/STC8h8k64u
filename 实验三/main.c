#include "main.h"

// ================= 全局变量定义 =================
uint8_t key = 0;
uint16_t voltage = 100;           // ADC电压值 (10mV单位, 0~250对应0~2.50V)
uint16_t v_cnt = 0;               // Timer0计数器(100us/tick)
uint16_t freq_cnt = 0;
volatile uint32_t freq_count = 0; // INT0脉冲累加器
uint32_t freq = 0;                // 实际测量频率值(Hz)
uint32_t set_freq=0;
// 电压显示段码: U _ _ _ _ X. X X
uint8_t v_char[8] = {0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0xC0, 0xC0};

// 频率显示段码: F _ D D D D D D
uint8_t f_char[8] = {0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// 状态变量 (初始值参见2.5节)
bit show_mode = 0;  // 0=电压显示界面, 1=频率显示界面 (初始:电压)
bit v_mode = 1;     // 0=DAC跟随W2, 1=DAC固定1.0V (初始:固定1.0V)
bit led_mode = 0;   // 0=LED启用, 1=LED关闭 (初始:启用)
bit seg_mode = 0;   // 0=数码管启用, 1=数码管关闭 (初始:启用)

// 外部变量
extern uint8_t LedBuff[8];
extern uint8_t code LedChar[];

// 函数声明
void Adc_run(void);
void Seg_Run(void);
void Key_Run(void);
void Led_Run(void);
void ExtInt0_Init(void);

// ================= 主函数 =================
void main(void)
{
 GPIO();
           
    Timer0_Init();          
    Timer1_Init();
    Timer2_Init();

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

// ================= Timer1中断 (500us, 时基+频率测量门控) =================
void timer1() interrupt 3
{
    TL1 = 0x18;                 // 手动重装: 65536-1000=64536=0xFC18
    TH1 = 0xFC;

    freq_cnt++;
	v_cnt++;
    
    if(v_cnt & 0x01)            // 奇数tick: 数码管扫描
    {                           
        if(seg_mode == 0)
            Seg_Scan();
		if(freq_cnt >= 1000) 
			{    
				freq_cnt = 0;
				freq= set_freq;
//				Timer2_SetFreq(set_freq);
			}

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

    if(show_mode == DISP_V)
    {
		v_char[7] = LedChar[voltage % 10];
		v_char[6] = LedChar[voltage / 10 % 10];
		v_char[5] = LedChar[voltage / 100] & 0x7F;
        for(i = 0; i < 8; i++)
            LedBuff[i] = v_char[i];
    } 
    else if(show_mode == DISP_F)
    {
        // 6位频率数字 (位置2~7)
        f_char[2] = LedChar[freq / 100000 % 10];
        f_char[3] = LedChar[freq / 10000 % 10];
        f_char[4] = LedChar[freq / 1000 % 10];
        f_char[5] = LedChar[freq / 100 % 10];
        f_char[6] = LedChar[freq / 10 % 10];
        f_char[7] = LedChar[freq % 10];

        // 前导零熄灭 (位置2~6, 位置7始终保留最低位)
        for(i = 2; i < 7; i++)
        {
            if(f_char[i] != 0xC0) break;
            else f_char[i] = 0xFF;
        }
        
        for(i = 0; i < 8; i++)
            LedBuff[i] = f_char[i];
    } 
}

// ================= ADC采集 & 数据刷新 (每约250ms, <0.5s满足要求) =================
void Adc_run(void)
{
    if(v_cnt >= 500)
	{
		v_cnt = 0;
		EA = 0;
		 if(show_mode == DISP_V)
		{
		if(v_mode == 0)
		{		
			// 始终读取ADC，计算电压和频率
			voltage = Get_ADC12bitResult(3);
			voltage = (250UL * voltage) / 4096;          // 0~250 (0~2.50V)				
		}
		else
		{
			voltage = 100;                           // 显示固定1.00V
		}
		}
		else
		{
			voltage = Get_ADC12bitResult(3);
			set_freq = (999999UL * voltage) / 4095UL; 
			if(set_freq > 999999) set_freq = 999999;     // 钳位到0~999999
			Timer2_SetFreq(set_freq);
		}

		EA = 1;	
	}
}

// ================= 按键处理 (S4~S7) =================
void Key_Run(void)
{
    key = Key_scan();
    switch(key) {

		// S4: 显示界面切换 (电压<-->频率)
        case 4:show_mode = !show_mode;
            break;

		// S5: DAC模式切换 (跟随W2 <--> 固定1.0V)        
		case 5:v_mode = !v_mode;
            if(v_mode == 1) voltage = 100;     // 切到固定模式时显示1.00V
            break;

		// S6: LED指示灯开关
        case 6:led_mode = !led_mode;
            break;

		// S7: 数码管开关
        case 7:seg_mode = !seg_mode;
            break;

        default: break;	
    }
}

// ================= LED指示灯 (P6口, 低电平点亮) =================
// L1=P6.0: 电压测量指示    L2=P6.1: 频率测量指示
// L3=P6.2: 电压阈值指示    L4=P6.3: 频率阈值指示
// L5=P6.4: DAC模式指示    L6~L8: 未使用(始终熄灭)
void Led_Run(void)
{
    uint8_t led_val;
    
    if(led_mode == 1)
    {
        // LED功能关闭: 全部熄灭
        P6 = 0xFF;               // ★先清P6
        P40 = 1;                 // 再关LED电源(清空后再关, 防止段码闪一下)
        return;
    }
    
    // LED功能启用 (P7/P6/P40已在Timer0 ISR中处理好)
    led_val = 0xFF;              // 默认全部熄灭(bit=1灭, bit=0亮)
    
    // L1/L2: 显示模式指示
    if(show_mode == DISP_V)
	{
        led_val &= ~0x01;        // L1亮(电压模式), L2灭
		// L3: 电压阈值指示
    // VW2<0.6V:灭, 0.6≤VW2<1.2V:亮, 1.2≤VW2<1.8V:灭, VW2≥1.8V:亮
    // voltage单位是10mV, 即阈值 60=0.6V, 120=1.2V, 180=1.8V
			if((voltage >= 60 && voltage < 120) || (voltage >= 180))
			led_val &= ~0x04;        // L3亮
    // L5: DAC模式指示 (VW2跟随模式亮, 固定1.0V模式灭)
    if(v_mode == 0)
        led_val &= ~0x10;        // L5亮(DAC跟随W2)
	}

    else
	{
		led_val &= ~0x02;        // L2亮(频率模式), L1灭        
    // L4: 频率阈值指示
    // F<1KHz:灭, 1KHz≤F<5KHz:亮, 5KHz≤F<10KHz:灭, F≥10KHz:亮
    if((freq >= 1000 && freq < 5000) || (freq >= 10000))
        led_val &= ~0x08;        // L4亮
    }
    
    // L6~L8:熄灭
    P6 = led_val;
}
