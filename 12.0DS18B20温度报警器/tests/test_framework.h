#ifndef __TEST_FRAMEWORK_H__
#define __TEST_FRAMEWORK_H__

#include <stdio.h>
#include <string.h>
#include <stdint.h>

// 测试统计
typedef struct {
    int total;
    int passed;
    int failed;
} TestStats;

extern TestStats test_stats;

// 模拟硬件寄存器
extern uint8_t P3;
extern uint8_t P6;
extern uint8_t P7;
extern uint8_t P3M0;
extern uint8_t P3M1;
extern uint8_t P6M0;
extern uint8_t P6M1;
extern uint8_t P7M0;
extern uint8_t P7M1;
extern bit EA;

// 模拟_nop_函数
#define _nop_() do {} while(0)

// 模拟_crol_函数
uint8_t _crol_(uint8_t value, uint8_t shift);

// 测试宏
#define TEST_INIT() do { \
    memset(&test_stats, 0, sizeof(test_stats)); \
    printf("=== 开始测试 ===\n"); \
} while(0)

#define TEST_END() do { \
    printf("\n=== 测试结果 ===\n"); \
    printf("总计: %d\n", test_stats.total); \
    printf("通过: %d\n", test_stats.passed); \
    printf("失败: %d\n", test_stats.failed); \
    printf("成功率: %.1f%%\n", (test_stats.passed * 100.0) / test_stats.total); \
    return test_stats.failed == 0 ? 0 : 1; \
} while(0)

#define TEST_ASSERT(condition, message) do { \
    test_stats.total++; \
    if (condition) { \
        test_stats.passed++; \
        printf("✓ "); \
    } else { \
        test_stats.failed++; \
        printf("\n✗ 测试失败: %s\n", message); \
        printf("  在文件: %s, 行: %d\n", __FILE__, __LINE__); \
    } \
} while(0)

#define TEST_EQUAL(expected, actual, message) \
    TEST_ASSERT((expected) == (actual), message)

#define TEST_NOT_EQUAL(expected, actual, message) \
    TEST_ASSERT((expected) != (actual), message)

#define TEST_RANGE(value, min, max, message) \
    TEST_ASSERT((value) >= (min) && (value) <= (max), message)

// 模拟延时函数
void Delay_ms(uint8_t ms);
void Delay_us(uint16_t us);

#endif // __TEST_FRAMEWORK_H__