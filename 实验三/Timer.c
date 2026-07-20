#include <stc8h.h>

sbit PWM_OUT = P2^3;

void Timer1_Init()		//1ºÁë@12.000MHz
{
	AUXR &= 0x7F;			//¶¨ʱƷʱ֓12Tģʽ
	TMOD &= 0x0f;			//ɨփ¶¨ʱƷģʽ
	TL1 = 0x18;				//ɨփ¶¨ʱ³õʼֵ
	TH1 = 0xFC;				//ɨփ¶¨ʱ³õʼֵ
	TF1 = 0;				//ǥ³ýTF0±ꖾ
	TR1 = 1;				//¶¨ʱƷ0¿ªʼ¼Ɗ±
	ET1 = 1;
	EA = 1;
}

void Timer0_Init()
{
    TMOD &= 0XF0;
    TMOD |= 0X05;
    TF0 = 0;				          //ǥ³ýTF0±ꖾ
    TR0= 1;				     //¶¨ʱƷ0¿ªʼ¼Ɗ±
    TL0= 0x00;				 //ɨփ¶¨ʱ³õʼֵ
    TH0 = 0x00;				//ɨփ¶¨ʱ³õʼֵ
    ET0 = 0;
    //P32=1;
}



void Timer2_Init(void)
{
    AUXR |= 0x04;       // T2x12=1£¬¶¨ʱƷ2ʱ֓=ϵͳʱ֓/1
    
    IE2 |= 0x04;        // ET2=1£¬ʹĜ¶¨ʱƷ2֐¶ύ
    EA = 1;             // ¿ªƴל֐¶ύ
    
    // 4. ψֹͣ¶¨ʱƷ2
    AUXR &= ~0x10;      // T2R=0
}

void Timer2_SetFreq(unsigned long freq)
{
    unsigned int reload;
    
    if(freq == 0 || freq > 6000000UL)  //上限改成6M，大于999999才关输出
    {
        AUXR &= ~0x10;
        PWM_OUT = 0;
        return;
    }
    reload = 65536UL - (12000000UL / (2UL * freq));
    
    T2H = (unsigned char)(reload >> 8);
    T2L = (unsigned char)reload;
    
    AUXR |= 0x10;
}

// ¶¨ʱƷ2֐¶Ϸþαº¯ʽ£¨֐¶Ϻű2£¬¹ٷ½ʖ²ᑩ֤սȷ£©
void Timer2_ISR(void) interrupt 12
{
    // ·­תʤ³öҽ½ō
    PWM_OUT = ~PWM_OUT;
}

