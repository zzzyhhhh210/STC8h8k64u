#include "test_framework.h"

// 测试统计
TestStats test_stats;

// 模拟硬件寄存器
uint8_t P3 = 0;
uint8_t P6 = 0;
uint8_t P7 = 0;
uint8_t P3M0 = 0;
uint8_t P3M1 = 0;
uint8_t P6M0 = 0;
uint8_t P6M1 = 0;
uint8_t P7M0 = 0;
uint8_t P7M1 = 0;
bit EA = 0;

// 模拟_crol_函数
uint8_t _crol_(uint8_t value, uint8_t shift) {
    shift %= 8;
    return (value << shift) | (value >> (8 - shift));
}

// 模拟延时函数
void Delay_ms(uint8_t ms) {
    // 在测试中，我们不实际延时，只记录调用
    // printf("Delay_ms(%u) called\n", ms);
}

void Delay_us(uint16_t us) {
    // 在测试中，我们不实际延时，只记录调用
    // printf("Delay_us(%u) called\n", us);
}