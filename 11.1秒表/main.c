#include "main.h"

// EEPROM 地址规划
#define ADDR_SEC  0x15
#define ADDR_MIN  0x16
#define ADDR_HOUR 0x17

// 全局变量定义 (默认初始值 23:59:50)
volatile uint8_t sec = 50;
volatile uint8_t min = 59;
volatile uint8_t hour = 23;

// 用于记录上次保存时的分钟，避免每秒保存导致抖动

uint8_t last_sec = 0xFF;
uint8_t last_min = 0xFF;
uint8_t last_hour = 0xFF;

// 函数声明
void Load_time_From_EEPROM(void);
void Save_time_To_EEPROM(void);

int main(void)	
{ 	    	   
    I2C_Init(); 
	Nix_Init();
	 
    // 1. 加载时间
	Load_time_From_EEPROM();
    Delay_ms(5);
    // 2. 初始化定时器
    Timer0_Init(); 
    
        // 同步标记，必须
    last_sec = sec;
	last_min = min;
	last_hour = hour;

    while(1)
    {
        // --- 数码管动态扫描显示 ---
        P7 = 0XFF;        
        P7 = ~T_COM[0];       
        P6 = Nix[sec % 10]; 
        Delay_ms(1);      
        
        P7 = 0XFF;        
        P7 = ~T_COM[1];        
        P6 = Nix[sec / 10]; 
        Delay_ms(1);      

        P7 = 0XFF;        
        P7 = ~T_COM[2];        
        P6 = ~Nix[10]; 
        Delay_ms(1);  
		
        P7 = 0XFF;        
        P7 = ~T_COM[3];        
        P6 = Nix[min % 10]; 
        Delay_ms(1); 
		
        P7 = 0XFF;        
        P7 = ~T_COM[4];        
        P6 = Nix[min / 10]; 
        Delay_ms(1); 
		
        P7 = 0XFF;        
        P7 = ~T_COM[5];        
        P6 = ~Nix[10]; 
        Delay_ms(1); 
		
        P7 = 0XFF;        
        P7 = ~T_COM[6];        
        P6 = Nix[hour % 10]; 
        Delay_ms(1);
		
        P7 = 0XFF;        
        P7 = ~T_COM[7];        
        P6 = Nix[hour / 10]; 
        Delay_ms(1);   		
		
        // --- 数据保存逻辑 (每分钟保存一次) ---
        if (sec  != last_sec| min != last_min | hour != last_hour)
        {
            Save_time_To_EEPROM();Delay_ms(5); 
                 last_sec = sec;
	         last_min = min;
	        last_hour = hour;
        }
    }
}
void Save_time_To_EEPROM(void)
{
    eeprom_write(ADDR_SEC, sec);Delay_ms(5);
	eeprom_write(ADDR_MIN, min);Delay_ms(5);
	eeprom_write(ADDR_HOUR, hour);Delay_ms(5);
}
void Load_time_From_EEPROM(void)
{
	uint8_t s ,m,h;

    //sec
    s = eeprom_read(ADDR_SEC);Delay_ms(5);
    m = eeprom_read(ADDR_MIN);Delay_ms(5);
    h = eeprom_read(ADDR_HOUR);Delay_ms(5);
     // 4. 校验数据合法性
    if (s < 60 && m < 60 && h < 24)
    {
        sec = s;
        min = m;
        hour = h;
    }
    else
    {
        // 数据损坏，重置为默认值并重新写入
        sec = 50;
        min = 59;
        hour = 23;
        Save_time_To_EEPROM();
    }
	
	
}

