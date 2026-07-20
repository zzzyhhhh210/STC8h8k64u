#include "RTC.h"
extern uint8_t Rtc_Save[3];
extern uint8_t clok_time[8];
void RTC_config()
{
	X32KCR = 0XC0;
	while(!(X32KCR & 0X01));
	RTCCFG &= ~0X02;
	
	// 4. 写入初始时间 (秒→分→时顺序写入)
    INISEC = Rtc_Save[2];
    INIMIN = Rtc_Save[1];
    INIHOUR = Rtc_Save[0];
	
	RTCCFG |= 0X01;
	RTCCR = 0X01;//启动RTC

	while(RTCCFG & 0x01);

}

void Display_Clock(void)
{
 unsigned char i;
	clok_time[0] = HOUR / 10;
    clok_time[1] = HOUR % 10;
    clok_time[2] = 0xBF; 
    clok_time[3] = MIN / 10;
    clok_time[4] = MIN % 10;
    clok_time[5] = 0xBF; 
    clok_time[6] = SEC / 10;
    clok_time[7] = SEC % 10;
   
    for(i = 0; i < 8; i++)
    {
        P7 = _crol_(0xfe, i);
        
        if(i == 2 || i == 5)
            P6 = clok_time[i]; // 显示分隔符 '-'
        else 
            P6 = Nix[clok_time[7 - i]]; // 显示数字
        
        Delay_ms(1);
        P6 = 0xFF; // 消隐
    }
}
