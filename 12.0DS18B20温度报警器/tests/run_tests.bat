@echo off
echo ============================================
echo DS18B20温度报警器单元测试
echo ============================================
echo.

REM 检查是否安装了gcc
where gcc >nul 2>nul
if %errorlevel% neq 0 (
    echo 错误: 未找到gcc编译器。
    echo 请安装MinGW或配置PATH环境变量。
    echo.
    pause
    exit /b 1
)

echo 1. 清理旧文件...
make clean > nul 2> nul

echo 2. 编译测试程序...
make all
if %errorlevel% neq 0 (
    echo 编译失败！
    pause
    exit /b 1
)

echo.
echo 3. 运行测试...
echo ============================================
.\run_tests.exe
set TEST_RESULT=%errorlevel%

echo.
echo ============================================
if %TEST_RESULT% equ 0 (
    echo 所有测试通过！
) else (
    echo 测试失败！
)
echo ============================================

echo.
pause
exit /b %TEST_RESULT%