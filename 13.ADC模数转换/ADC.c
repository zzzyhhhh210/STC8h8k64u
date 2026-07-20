#include "ADC.h"

unsigned char KeyCode = 0;
unsigned char cnt1ms = 0;
unsigned char ADC_KeyState = 0;
unsigned char ADC_KeyHoldCnt = 0;

//p223
void ADC_Iint(void)
{
//    unsigned int j;
    P6M1 = 0XFF; P6M0 = 0XFF;
    P7M1 = 0X00; P7M0 = 0X00;
   P1M1 = P1M1 |0X08;  //高阻

   ADCCFG = ADCCFG|0X20;
    ADC_CONTR = 0x80;  //初始化ADC控制寄存器
    
}

unsigned int Get_ADC12bitResult(unsigned char channel)
{
    ADC_RES = 0;
    ADC_RESL = 0;

    ADC_CONTR = (ADC_CONTR & 0xE0) |0X40 | channel;//启动AD转换
    _nop_();
    _nop_();
    _nop_();
    _nop_();

    while((ADC_CONTR & 0X20) == 0);
    ADC_CONTR &= ~0X20;
    return (((unsigned int)ADC_RES << 8) | ADC_RESL);

}

void CalculateAdcKey(unsigned char adc)
{
    unsigned char i;
      unsigned char j;

      if(adc<(256-ADC_OFFSET))
      {
        ADC_KeyState = 0;
        ADC_KeyHoldCnt = 0;

      }
      j = 256;

      for(i=1;i<=16;i++)
      {
        if((adc>=(j - ADC_OFFSET))&&(adc<=(j + ADC_OFFSET)))
        break;
        j+= 256;

      }
        if(i<=16)
        {
            KeyCode = i;
        }

}
void Timer0_Interrupt() interrupt 1
{
    cnt1ms++;
}