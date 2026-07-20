/**
 * @file I2C_test.c
 * @brief I2C模块单元测试 - 针对I2C_Isr中断处理逻辑
 */

#include <stdio.h>
#include <assert.h>
#include <stdint.h>

/* ========== 硬件寄存器模拟 ========== */
static volatile uint8_t I2CMSST = 0;
static volatile uint8_t busy = 0;

/* 被测试函数声明 */
void I2C_Isr(void);

/* ========== 测试用例 ========== */

/**
 * @test I2C_Isr_InterruptFlagSet
 * @brief 测试中断标志置位时，应清除标志并设置busy=0
 */
void test_I2C_Isr_InterruptFlagSet(void)
{
    printf("[TEST] test_I2C_Isr_InterruptFlagSet\n");
    
    /* 初始化：中断标志置位，busy=1 */
    I2CMSST = 0x40;
    busy = 1;
    
    /* 执行中断处理 */
    I2C_Isr();
    
    /* 验证：中断标志被清除，busy=0 */
    assert((I2CMSST & 0x40) == 0 && "中断标志应被清除");
    assert(busy == 0 && "busy应被清零");
    
    printf("[PASS] 中断标志置位时正确清除标志并设置busy=0\n");
}

/**
 * @test I2C_Isr_InterruptFlagNotSet
 * @brief 测试中断标志未置位时，busy保持不变
 */
void test_I2C_Isr_InterruptFlagNotSet(void)
{
    printf("[TEST] test_I2C_Isr_InterruptFlagNotSet\n");
    
    /* 初始化：中断标志未置位，busy=1 */
    I2CMSST = 0x00;  /* 中断标志位(bit6)为0 */
    busy = 1;
    
    /* 执行中断处理 */
    I2C_Isr();
    
    /* 验证：busy保持原值1 */
    assert(busy == 1 && "busy应保持不变");
    
    printf("[PASS] 中断标志未置位时busy保持不变\n");
}

/**
 * @test I2C_Isr_OnlyClearOwnFlag
 * @brief 测试仅清除自身标志位(0x40)，不影响其他位
 */
void test_I2C_Isr_OnlyClearOwnFlag(void)
{
    printf("[TEST] test_I2C_Isr_OnlyClearOwnFlag\n");
    
    /* 初始化：I2CMSST有多个标志位设置 */
    I2CMSST = 0x7F;  /* bit6为1，其他位也设1 */
    busy = 1;
    
    /* 执行中断处理 */
    I2C_Isr();
    
    /* 验证：仅bit6被清除，其他位保持 */
    assert((I2CMSST & 0x40) == 0 && "bit6应被清除");
    assert((I2CMSST & 0x3F) == 0x3F && "其他位应保持不变");
    assert(busy == 0 && "busy应被清零");
    
    printf("[PASS] 仅清除bit6，其他位保持不变\n");
}

/**
 * @test I2C_Isr_MultipleCalls
 * @brief 测试连续调用时的行为
 */
void test_I2C_Isr_MultipleCalls(void)
{
    printf("[TEST] test_I2C_Isr_MultipleCalls\n");
    
    /* 第一次调用：标志置位 */
    I2CMSST = 0x40;
    busy = 1;
    I2C_Isr();
    assert(busy == 0 && "第一次调用后busy应为0");
    
    /* 第二次调用：标志未置位，busy保持0 */
    I2CMSST = 0x00;
    I2C_Isr();
    assert(busy == 0 && "第二次调用后busy应保持0");
    
    printf("[PASS] 连续调用时行为正确\n");
}

/* ========== 测试框架 ========== */

int main(void)
{
    printf("===========================================\n");
    printf("  I2C模块单元测试 - I2C_Isr中断处理逻辑\n");
    printf("===========================================\n\n");
    
    int passed = 0;
    int total = 4;
    
    /* 运行所有测试 */
    test_I2C_Isr_InterruptFlagSet();
    passed++;
    
    test_I2C_Isr_InterruptFlagNotSet();
    passed++;
    
    test_I2C_Isr_OnlyClearOwnFlag();
    passed++;
    
    test_I2C_Isr_MultipleCalls();
    passed++;
    
    /* 输出测试结果 */
    printf("\n===========================================\n");
    printf("  测试结果: %d/%d 通过\n", passed, total);
    printf("===========================================\n");
    
    return (passed == total) ? 0 : 1;
}
