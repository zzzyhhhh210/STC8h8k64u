#include "main.h"

// 全局变量定义
volatile uint8_t sec = 0;   
extern volatile bit g_timer_flag; 

// 用于判断秒数是否变化
uint8_t last_sec = 0xFF;     

// 函数声明
void Load_Seconds_From_EEPROM(void);	
void Save_Seconds_To_EEPROM(void);



/*
//亮灯测试
void main(void)
{
	 uint8_t readVal;
	I2C_Init();
	
	P40=0;
    P6 = 0xFF; // 熄灭 LED (假设低电平亮)

    // --- 测试写入 ---
    // 向地址 0x01 写入数据 0xFE
    eeprom_write(0x01, 0xFE);
    
//    // --- 测试读取 ---
//    // 从地址 0x01 读取数据
    readVal = eeprom_read(0x01);
//    
//    // --- 结果验证 ---
//    // 将读取到的数据输出到 P6 口
//    // 如果读取正确，P6 应为 0xFE (1111 1110)，即 P6.0 亮灯
    P6 = readVal;
	
	while(1);
}
*/

int main(void)
{  
    // 1. 硬件初始化
    I2C_Init();   
    Nix_Init();   

    Load_Seconds_From_EEPROM();
    
    // 现在才初始化定时器，sec 已经是正确值了
    Timer0_Init(); 
    
    // 同步标记
    last_sec = sec;

    while(1)
    {
        // --- 数码管动态扫描显示 ---
        P6 = 0XFF;        
        P7 = 0xfe;        
        P6 = Nix[sec % 10]; 
        Delay_ms(2);      
        
        P6 = 0XFF;        
        P7 = 0xfd;        
        P6 = Nix[sec / 10]; 
        Delay_ms(2);      

        // --- 数据保存逻辑 ---
        if (sec != last_sec)
        {
            Save_Seconds_To_EEPROM(); 
			Delay_ms(5);
            last_sec = sec;           
        }
    }
}

/**
 * @brief 从 EEPROM 加载秒数
 */
void Load_Seconds_From_EEPROM(void)
{
    uint8_t val = eeprom_read(0x10);
	Delay_ms(5);
    if (val <= 99)
    {
        // 读取到有效数据 (0-99)，直接恢复
        sec = val;
    }
    else
    {
        sec = 0;
        eeprom_write(0x10, 0); 
    }
}

/**
 * @brief 保存秒数到 EEPROM
 */
void Save_Seconds_To_EEPROM(void)
{
    eeprom_write(0x10, sec);
}