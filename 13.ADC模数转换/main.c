#include "main.h"

extern volatile unsigned char KeyCode;
extern volatile unsigned char cnt1ms;
extern volatile unsigned char ADC_KeyState;
extern volatile unsigned char ADC_KeyHoldCnt;

unsigned char v[] = {0,0,0};

unsigned int volotage = 0;//数据类型12位选int

 int main(void)
{
	unsigned int i,adc_val;
      ADC_Iint();
      Timer0_Init();
      while(1)
      {
      // P7 = 0xfe;
      //  P6 = Nix[KeyCode%10];
      //  Delay_ms(1);
      //  P6 = 0XFF;

      //  P7 = 0XFD;
      //  P6 = Nix[KeyCode/10];
      //  Delay_ms(1);
      //  P6 = 0XFF;
       
      //  if(cnt1ms>=30)
      //  {
      //       cnt1ms = 0;
      //       j = Get_ADC12bitResult(0);
      //       if(j>192)//256
      //       {
      //            Delay_ms(10);
      //            j = Get_ADC12bitResult(0);
      //           if(j<4096) CalculateAdcKey(j);
      //       }

      //  } 
       if(cnt1ms>=50)
       {
            cnt1ms = 0;
		   adc_val = Get_ADC12bitResult(15); //1.内部
//            adc_val = Get_ADC12bitResult(3); //2.外部
       }
       volotage = (250UL*adc_val) /4096; //1
//	   volotage = (500UL*adc_val) /4096;//2

       v[0] = volotage%10;
       v[1] = volotage/10%10;
       v[2] = volotage/100;

       for(i=0;i<3;i++)
       {
            P6 = 0XFF;
            P7 = _crol_(0xfe,i);

            if(i==2)P6 = Nix[v[i]] & 0x7f;
            else P6 = Nix[v[i]];
            Delay_ms(1);
            P6 = 0XFF;
            P7 = 0xFF;

       }
      }

}


       