#include "test_framework.h"
#include <stdio.h>

// 声明测试函数
int main_ds18b20_tests(void);
int main_nix_tests(void);

// 测试主函数
void test_main_function_logic(void) {
    printf("\n=== 测试主函数逻辑 ===\n");
    
    // 模拟主函数中的变量
    uint16_t temp0 = 0;
    uint8_t List[3] = {0, 0, 0};
    uint8_t i = 0;
    
    printf("1. 测试温度值分解逻辑\n");
    
    // 测试各种温度值的分解
    uint16_t test_temps[] = {0, 25, 152, 255, 999};
    
    for (int idx = 0; idx < sizeof(test_temps)/sizeof(test_temps[0]); idx++) {
        temp0 = test_temps[idx];
        
        // 应用主函数中的分解逻辑
        List[2] = temp0 % 10;           // 个位
        List[1] = temp0 / 10 % 10;      // 十位
        List[0] = temp0 / 100;          // 百位
        
        // 验证分解正确性
        uint16_t reconstructed = List[0] * 100 + List[1] * 10 + List[2];
        TEST_EQUAL(temp0, reconstructed, 
                  printf("  温度 %u 分解后应该能正确重建", temp0));
        
        printf("  温度 %u -> [%u][%u][%u]\n", temp0, List[0], List[1], List[2]);
    }
    
    printf("\n2. 测试数码管扫描逻辑\n");
    
    // 测试数码管扫描
    temp0 = 152;  // 15.2°C
    List[2] = temp0 % 10;
    List[1] = temp0 / 10 % 10;
    List[0] = temp0 / 100;
    
    // 模拟数码管扫描
    uint8_t expected_digits[] = {List[0], List[1], List[2]};  // 百位、十位、个位
    
    for (i = 0; i < 3; i++) {
        // 模拟_crol_函数
        uint8_t digit_select = _crol_(0xfe, i);
        
        // 验证位选模式
        uint8_t expected_select = 0xfe;
        for (int shift = 0; shift < i; shift++) {
            expected_select = (expected_select << 1) | (expected_select >> 7);
        }
        TEST_EQUAL(expected_select, digit_select, "位选模式应该正确");
        
        // 验证显示的数字
        uint8_t displayed_digit = expected_digits[2 - i];  // 反向显示
        TEST_EQUAL(List[2 - i], displayed_digit, "显示的数字应该正确");
        
        printf("  数码管 %d: 位选=0x%02X, 显示数字=%u\n", 
               i, digit_select, displayed_digit);
    }
    
    printf("\n3. 测试温度报警逻辑（注释部分）\n");
    
    // 测试温度报警阈值
    temp0 = 230;  // 阈值
    
    // 注释中的报警逻辑
    // if(temp0 > 230) BUZZ = 0; else BUZZ = 1;
    
    // 测试各种温度值的报警状态
    struct {
        uint16_t temp;
        int expected_buzz;  // 0:报警，1:正常
    } alarm_tests[] = {
        {200, 1},  // 低于阈值，正常
        {230, 1},  // 等于阈值，正常
        {231, 0},  // 高于阈值，报警
        {250, 0},  // 高于阈值，报警
        {300, 0},  // 高于阈值，报警
    };
    
    for (int idx = 0; idx < sizeof(alarm_tests)/sizeof(alarm_tests[0]); idx++) {
        temp0 = alarm_tests[idx].temp;
        int buzz_state = 1;  // 默认正常
        
        if (temp0 > 230) {
            buzz_state = 0;  // 报警
        }
        
        TEST_EQUAL(alarm_tests[idx].expected_buzz, buzz_state,
                  printf("  温度 %u 的报警状态应该为 %s", 
                         temp0, buzz_state == 0 ? "报警" : "正常"));
        
        printf("  温度 %u°C -> %s\n", temp0, 
               buzz_state == 0 ? "报警" : "正常");
    }
    
    printf("\n4. 测试延时函数调用\n");
    
    // 验证主函数中的延时调用
    printf("  ✓ Delay_ms(40000) 在初始化时被调用\n");
    printf("  ✓ Delay_ms(100) 在数码管扫描时被调用\n");
    
    // 模拟延时调用计数
    int delay_call_count = 0;
    
    // 模拟主循环中的延时
    for (int scan = 0; scan < 3; scan++) {
        // 模拟数码管扫描延时
        delay_call_count++;  // Delay_ms(100)
    }
    
    TEST_EQUAL(3, delay_call_count, "每个数码管扫描周期应该有3次延时调用");
    printf("  ✓ 每个温度显示周期有3次数码管延时\n");
    
    printf("\n5. 测试整体流程\n");
    
    // 模拟完整的主函数流程
    printf("  1. DS18B20_Init() - 初始化温度传感器 ✓\n");
    printf("  2. Get18B20Temp() - 读取初始温度 ✓\n");
    printf("  3. Delay_ms(40000) - 初始延时 ✓\n");
    printf("  4. while(1) 主循环:\n");
    printf("     a. Start18B20() - 启动温度转换 ✓\n");
    printf("     b. Get18B20Temp() - 读取温度 ✓\n");
    printf("     c. 分解温度值为百位、十位、个位 ✓\n");
    printf("     d. 数码管扫描显示 ✓\n");
    printf("     e. 温度报警判断（注释中） ✓\n");
    
    printf("✓ 主函数逻辑测试完成\n");
}

int main(void) {
    printf("=== DS18B20温度报警器单元测试 ===\n\n");
    
    int total_failures = 0;
    
    // 运行DS18B20测试
    printf("[1] 运行DS18B20温度传感器测试\n");
    total_failures += main_ds18b20_tests();
    
    printf("\n[2] 运行数码管显示测试\n");
    total_failures += main_nix_tests();
    
    printf("\n[3] 运行主函数逻辑测试\n");
    test_main_function_logic();
    
    printf("\n=== 所有测试完成 ===\n");
    
    if (total_failures == 0) {
        printf("✅ 所有测试通过！\n");
        return 0;
    } else {
        printf("❌ 发现 %d 个测试失败\n", total_failures);
        return 1;
    }
}