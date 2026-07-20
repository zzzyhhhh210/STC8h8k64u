#include "main.h"
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

