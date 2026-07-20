# DS18B20温度报警器单元测试Makefile
# 适用于Windows/MinGW或Linux/gcc环境

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I. -I..
LDFLAGS = 

# 源文件
TEST_SRCS = test_framework.c test_ds18b20.c test_nix.c test_main.c
TEST_OBJS = $(TEST_SRCS:.c=.o)

# 目标文件
TARGET = run_tests.exe

# 默认目标
all: $(TARGET)

# 链接测试程序
$(TARGET): $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# 编译测试源文件
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理
clean:
	rm -f $(TEST_OBJS) $(TARGET) *.exe

# 运行测试
test: $(TARGET)
	./$(TARGET)

# 编译并运行测试
all-test: clean all test

# 帮助信息
help:
	@echo "可用命令:"
	@echo "  make all      - 编译测试程序"
	@echo "  make clean    - 清理编译文件"
	@echo "  make test     - 运行测试"
	@echo "  make all-test - 清理、编译并运行测试"
	@echo "  make help     - 显示此帮助信息"

# 伪目标
.PHONY: all clean test all-test help