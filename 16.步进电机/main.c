#include <stc8h.h>
#include "LED.h"
#include "Delay.h"
unsigned char code BeatCode[8] = {//步进电机节拍对应io口
      0x70,0x30,0xb0,0x90,0xc0,0xe0,0xe0,0x60
};
void TurnMotor(unsigned long angle);
void  main(void)
{
//      unsigned char tmp;//定义一个临时变量
//      unsigned char index = 0;//定义节拍输出索引
      P6M0 = 0xF0;P6M1 = 0x00;     
		TurnMotor(90);
      while(1)
      {
//            tmp =P6;//用tmp保存当前P6的值
//            tmp = tmp & 0x0f;//只保留P6的4位
//            tmp = tmp|BeatCode[index];//将BeatCode[index]的值与P6的4位进行或运算
//            P6 = tmp;//将tmp的值赋给P6
//            index++;//节拍输出索引加1
//            index = index & 0x07;//高效循环：index & 0x07 比 index % 8 更快（位运算替代取模）
//            Delay_ms(2);

      }
}
// 输入参数：angle为旋转角度（度）
void TurnMotor(unsigned long angle)
{
      unsigned char tmp; // 临时变量，用于位操作
      unsigned char index = 0; // 节拍索引（0-7循环）
      unsigned long beats = 0;// 总节拍数

      beats = (angle*4096)/360;     // 计算需要输出的节拍数
      while(beats--) // 每循环一次，beats减1，直到为0退出
      {
             // [与之前解释相同的位操作逻辑]
            tmp = P6; // 保留低4位
            tmp = tmp & 0x0f;
            tmp = tmp | BeatCode[index];// 组合节拍码
            P6 = tmp;  // 输出到端口
            index++; // 节拍索引递增
            index = index & 0x07; // 模8循环（0-7）
            Delay_ms(5); // 每拍2ms
            
      }
      P6 = P6 | 0xf0;// 输出0xF0到P6高4位，关闭所有相

}
// TurnMotor(90);   // 旋转90度（1024拍）
// TurnMotor(180);  // 旋转180度（2048拍）
// TurnMotor(360);  // 旋转一圈（4096拍）