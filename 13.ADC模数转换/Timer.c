#include "Timer.h"

unsigned char cnt = 0;

void Timer0_Init(void)      //��ʱ��0�ж�  T0  MODE0
{
	P7M0=0XFF;
	P7M1=0XFF;
	P6M0=0X00;
	P6M1=0X00;		
	AUXR = 0X7F; //�����Ĵ�������	
	
	TMOD = 0XF0;//��ʱ��ģʽ�Ĵ�������
	TCON = 0X10;
	TL0 = 0XFF;
	TH0 = 0XFF;//50ms
	
	EA = 1;
	ET0 = 1;//�ж�����λ	
}
//�ж�ʱ���������ܷ�������
//�жϲ��ܴ���
//�ж�û�з���ֵ
//�жϺ�����������
//�жϺô�1.���CPUЧ��2.ʵ��ʵʱ��������3.ʵ�ַ�ʱ����
//
void Timer0(void) interrupt 0
{ 
		cnt ++;
		if(cnt == 20)
		{
			cnt = 0;					
			P6 =~ P6;
		}

}
