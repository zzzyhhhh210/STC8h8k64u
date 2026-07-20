#include "ADC.h"

unsigned int Get_ADC12bitResult(unsigned char channel)
{
    unsigned int timeout;

    ADC_RES = 0;    // ADC结果高四位
    ADC_RESL = 0;   // ADC结果低八位

    ADC_CONTR = (ADC_CONTR & 0xE0) | 0x40 | channel;  // 启动AD转换
    _nop_();
    _nop_();
    _nop_();
    _nop_();

    // 等待转换完成, 加超时保护(输入>Vref时ADC可能不完成, 防止死循环)
    timeout = 10000;
    while((ADC_CONTR & 0x20) == 0)
    {
        if(--timeout == 0) break;   // 超时退出, 返回0
    }
    ADC_CONTR &= ~0x20;              // 清除ADC转换完成标志(bit5), 原代码~0x02错误
    return (((unsigned int)ADC_RES << 8) | ADC_RESL);
}
