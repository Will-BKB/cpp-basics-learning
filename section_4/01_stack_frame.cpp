/**
 * 教学程序：栈帧结构与返回地址
 * 目标：理解函数调用时的栈帧结构和返回地址机制
 *
 * 核心知识点：
 * 1. 栈帧的基本结构
 * 2. 返回地址的存储和恢复
 * 3. 跨平台的寄存器差异（x86-64 vs ARM64）
 * 4. 函数调用时的栈帧变化
 */

#include <iostream>
#include <cstdint>

// 平台检测
#if defined(__x86_64__)
    #define PLATFORM_X86_64
    #define PLATFORM_NAME "x86-64"
#elif defined(__aarch64__)
    #define PLATFORM_ARM64
    #define PLATFORM_NAME "ARM64"
#elif defined(__i386__)
    #define PLATFORM_X86_32
    #define PLATFORM_NAME "x86-32"
#else
    #define PLATFORM_UNKNOWN
    #define PLATFORM_NAME "Unknown"
#endif

// 获取栈帧地址（跨平台）
void* get_frame_address() {
    return __builtin_frame_address(0);
}

// 获取返回地址（跨平台）
void* get_return_address() {
    return __builtin_return_address(0);
}

// 打印平台信息
void print_platform_info() {
    std::cout << "========================================" << std::endl;
    std::cout << "平台信息" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "平台: " << PLATFORM_NAME << std::endl;
    std::cout << "指针大小: " << sizeof(void*) * 8 << " bits" << std::endl;

#ifdef PLATFORM_X86_64
    std::cout << "栈帧指针: RBP" << std::endl;
    std::cout << "栈指针: RSP" << std::endl;
    std::cout << "返回地址: 栈上" << std::endl;
#elif defined(PLATFORM_ARM64)
    std::cout << "栈帧指针: FP (X29)" << std::endl;
    std::cout << "栈指针: SP (X31)" << std::endl;
    std::cout << "返回地址: LR (X30)" << std::endl;
#endif
    std::cout << std::endl;
}

// 简单的函数，演示栈帧结构
void simple_function() {
    void* frame_addr = get_frame_address();
    void* return_addr = get_return_address();

    std::cout << "========================================" << std::endl;
    std::cout << "simple_function 栈帧信息" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "栈帧地址: " << frame_addr << std::endl;
    std::cout << "返回地址: " << return_addr << std::endl;

    // 局部变量（在栈帧上）
    int local_var = 42;
    std::cout << "局部变量地址: " << &local_var << std::endl;
    std::cout << "局部变量值: " << local_var << std::endl;
    std::cout << std::endl;
}

// 演示函数调用链的栈帧变化
void level_3() {
    void* frame_addr = get_frame_address();
    void* return_addr = get_return_address();

    std::cout << "========================================" << std::endl;
    std::cout << "level_3 栈帧信息" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "栈帧地址: " << frame_addr << std::endl;
    std::cout << "返回地址: " << return_addr << std::endl;
    std::cout << "这是最深的调用层级" << std::endl;
    std::cout << std::endl;
}

void level_2() {
    void* frame_addr = get_frame_address();
    void* return_addr = get_return_address();

    std::cout << "========================================" << std::endl;
    std::cout << "level_2 栈帧信息" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "栈帧地址: " << frame_addr << std::endl;
    std::cout << "返回地址: " << return_addr << std::endl;
    std::cout << "准备调用 level_3..." << std::endl;
    std::cout << std::endl;

    level_3();

    std::cout << "返回到 level_2" << std::endl;
    std::cout << "栈帧地址: " << frame_addr << " (未改变)" << std::endl;
    std::cout << std::endl;
}

void level_1() {
    void* frame_addr = get_frame_address();
    void* return_addr = get_return_address();

    std::cout << "========================================" << std::endl;
    std::cout << "level_1 栈帧信息" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "栈帧地址: " << frame_addr << std::endl;
    std::cout << "返回地址: " << return_addr << std::endl;
    std::cout << "准备调用 level_2..." << std::endl;
    std::cout << std::endl;

    level_2();

    std::cout << "返回到 level_1" << std::endl;
    std::cout << "栈帧地址: " << frame_addr << " (未改变)" << std::endl;
    std::cout << std::endl;
}

// 主函数
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "栈帧结构与返回地址" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "编译说明：" << std::endl;
    std::cout << "g++ -g -Wall -Wextra -std=c++17 01_stack_frame.cpp -o 01_stack_frame" << std::endl;
    std::cout << std::endl;

    // 打印平台信息
    print_platform_info();

    // 演示简单函数的栈帧
    simple_function();

    // 演示函数调用链
    level_1();

    std::cout << "========================================" << std::endl;
    std::cout << "总结" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. 栈帧结构：" << std::endl;
    std::cout << "   - 每个函数调用都有一个栈帧" << std::endl;
    std::cout << "   - 栈帧包含局部变量、返回地址等" << std::endl;
    std::cout << "   - 栈向低地址增长" << std::endl;

    std::cout << "\n2. 返回地址：" << std::endl;
    std::cout << "   - x86-64: 返回地址存储在栈上" << std::endl;
    std::cout << "   - ARM64: 返回地址存储在 LR 寄存器" << std::endl;
    std::cout << "   - 函数返回时跳转到返回地址" << std::endl;

    std::cout << "\n3. 跨平台差异：" << std::endl;
    std::cout << "   - x86-64: 使用 RBP/RSP 寄存器" << std::endl;
    std::cout << "   - ARM64: 使用 FP/SP 寄存器，LR 存储返回地址" << std::endl;

    std::cout << "\n4. 栈帧生命周期：" << std::endl;
    std::cout << "   - 函数调用时创建栈帧" << std::endl;
    std::cout << "   - 函数返回时销毁栈帧" << std::endl;
    std::cout << "   - 栈帧地址在函数执行期间保持不变" << std::endl;

    return 0;
}