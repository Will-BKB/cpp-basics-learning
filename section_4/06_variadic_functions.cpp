/**
 * 教学程序：可变参数函数
 * 目标：理解可变参数函数的工作原理和实现机制
 *
 * 核心知识点：
 * 1. va_list, va_start, va_arg, va_end 宏的使用
 * 2. 格式字符串解析
 * 3. 可变参数的底层实现
 * 4. 类型安全的问题
 */

#include <iostream>
#include <cstdarg>
#include <string>

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

// 简单的可变参数函数实现
void my_printf(const char* format, ...) {
    va_list args;          // 声明可变参数列表
    va_start(args, format); // 初始化参数列表，format 是最后一个固定参数

    // 遍历格式字符串
    for (const char* p = format; *p != '\0'; p++) {
        if (*p == '%') {
            // 遇到格式说明符，读取下一个字符
            p++;
            switch (*p) {
                case 'd': {  // 整数
                    int value = va_arg(args, int);
                    std::cout << value;
                    break;
                }
                case 'f': {  // 浮点数
                    double value = va_arg(args, double);
                    std::cout << value;
                    break;
                }
                case 'c': {  // 字符
                    int value = va_arg(args, int);  // char 在可变参数中提升为 int
                    std::cout << (char)value;
                    break;
                }
                case 's': {  // 字符串
                    char* value = va_arg(args, char*);
                    std::cout << value;
                    break;
                }
                case '%': {  // 百分号
                    std::cout << '%';
                    break;
                }
                default: {
                    std::cout << "[未知格式: " << *p << "]";
                    break;
                }
            }
        } else {
            // 普通字符，直接输出
            std::cout << *p;
        }
    }

    va_end(args); // 清理参数列表
    std::cout << std::endl;
}

// 演示基本的可变参数函数
void demonstrate_basic_variadic() {
    std::cout << "========================================" << std::endl;
    std::cout << "基本的可变参数函数" << std::endl;
    std::cout << "========================================" << std::endl;

    // 调用 my_printf
    my_printf("Hello, %s!", "World");
    my_printf("整数: %d, 浮点数: %f", 42, 3.14);
    my_printf("字符: %c, 字符串: %s", 'A', "C++");
    my_printf("多个参数: %d %f %c %s", 10, 2.5, 'X', "Test");

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 可变参数函数可以接受任意数量的参数" << std::endl;
    std::cout << "- 使用格式字符串指定参数类型" << std::endl;
    std::cout << "- 函数根据格式字符串解析参数" << std::endl;
    std::cout << std::endl;
}

// 演示可变参数的底层原理
void demonstrate_implementation_details() {
    std::cout << "========================================" << std::endl;
    std::cout << "可变参数的底层原理" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "1. va_list：" << std::endl;
    std::cout << "   - 是一个指针类型" << std::endl;
    std::cout << "   - 指向可变参数在栈上的位置" << std::endl;
    std::cout << "   - 不同平台可能有不同的实现" << std::endl;

    std::cout << "\n2. va_start(args, last_fixed)：" << std::endl;
    std::cout << "   - 初始化 va_list" << std::endl;
    std::cout << "   - last_fixed 是最后一个固定参数" << std::endl;
    std::cout << "   - args 指向第一个可变参数" << std::endl;

    std::cout << "\n3. va_arg(args, type)：" << std::endl;
    std::cout << "   - 获取当前参数，并移动到下一个" << std::endl;
    std::cout << "   - type 指定参数的类型" << std::endl;
    std::cout << "   - 必须与实际参数类型匹配" << std::endl;

    std::cout << "\n4. va_end(args)：" << std::endl;
    std::cout << "   - 清理 va_list" << std::endl;
    std::cout << "   - 某些平台需要释放资源" << std::endl;
    std::cout << std::endl;
}

// 演示类型安全问题
void demonstrate_type_safety() {
    std::cout << "========================================" << std::endl;
    std::cout << "类型安全问题" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "正确的使用：" << std::endl;
    my_printf("整数: %d", 42);
    my_printf("浮点数: %f", 3.14);

    std::cout << "\n类型不匹配（未定义行为）：" << std::endl;
    std::cout << "my_printf(\"整数: %d\", 3.14);  // 错误：格式是 %d 但传入 double" << std::endl;
    std::cout << "my_printf(\"浮点数: %f\", 42);   // 错误：格式是 %f 但传入 int" << std::endl;

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 可变参数函数没有类型检查" << std::endl;
    std::cout << "- 类型不匹配会导致未定义行为" << std::endl;
    std::cout << "- 编译器无法检测这类错误" << std::endl;
    std::cout << "- C++11 引入了可变参数模板来解决这个问题" << std::endl;
    std::cout << std::endl;
}

// 演示参数提升规则
void demonstrate_argument_promotion() {
    std::cout << "========================================" << std::endl;
    std::cout << "参数提升规则" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "在可变参数函数中，小类型会被提升：" << std::endl;
    std::cout << "- char, short → int" << std::endl;
    std::cout << "- float → double" << std::endl;

    std::cout << "\n示例：" << std::endl;
    my_printf("char: %c", 'A');      // char 被提升为 int
    my_printf("short: %d", (short)100);  // short 被提升为 int
    my_printf("float: %f", 1.5f);    // float 被提升为 double

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 使用 va_arg 时必须使用提升后的类型" << std::endl;
    std::cout << "- char 和 short 必须用 int 读取" << std::endl;
    std::cout << "- float 必须用 double 读取" << std::endl;
    std::cout << std::endl;
}

// 演示跨平台注意事项
void demonstrate_cross_platform() {
    std::cout << "========================================" << std::endl;
    std::cout << "跨平台注意事项" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "当前平台: " << PLATFORM_NAME << std::endl;

    std::cout << "\n不同平台的调用约定：" << std::endl;
    std::cout << "x86-64 (System V ABI)：" << std::endl;
    std::cout << "  - 前 6 个整数参数通过寄存器传递" << std::endl;
    std::cout << "  - 前 8 个浮点参数通过 XMM 寄存器传递" << std::endl;
    std::cout << "  - 剩余参数通过栈传递" << std::endl;

    std::cout << "\nARM64 (AAPCS64)：" << std::endl;
    std::cout << "  - 前 8 个参数通过 x0-x7 寄存器传递" << std::endl;
    std::cout << "  - 前 8 个浮点参数通过 v0-v7 寄存器传递" << std::endl;
    std::cout << "  - 剩余参数通过栈传递" << std::endl;

    std::cout << "\n可变参数函数的特殊处理：" << std::endl;
    std::cout << "- 可变参数总是通过栈传递" << std::endl;
    std::cout << "- 即使固定参数通过寄存器传递" << std::endl;
    std::cout << "- va_list 的实现会处理这些差异" << std::endl;
    std::cout << std::endl;
}

// 主函数
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "可变参数函数" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "编译说明：" << std::endl;
    std::cout << "g++ -g -Wall -Wextra -std=c++17 06_variadic_functions.cpp -o 06_variadic_functions" << std::endl;
    std::cout << std::endl;

    std::cout << "平台: " << PLATFORM_NAME << std::endl;
    std::cout << std::endl;

    // 演示基本的可变参数函数
    demonstrate_basic_variadic();

    // 演示底层实现细节
    demonstrate_implementation_details();

    // 演示类型安全问题
    demonstrate_type_safety();

    // 演示参数提升规则
    demonstrate_argument_promotion();

    // 演示跨平台注意事项
    demonstrate_cross_platform();

    std::cout << "========================================" << std::endl;
    std::cout << "总结" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. 可变参数函数语法：" << std::endl;
    std::cout << "   - 使用 ... 表示可变参数" << std::endl;
    std::cout << "   - 必须至少有一个固定参数" << std::endl;
    std::cout << "   - 示例：void func(const char* format, ...)" << std::endl;

    std::cout << "\n2. 核心宏：" << std::endl;
    std::cout << "   - va_list: 声明参数列表" << std::endl;
    std::cout << "   - va_start: 初始化参数列表" << std::endl;
    std::cout << "   - va_arg: 获取下一个参数" << std::endl;
    std::cout << "   - va_end: 清理参数列表" << std::endl;

    std::cout << "\n3. 底层实现：" << std::endl;
    std::cout << "   - 参数在栈上连续存储" << std::endl;
    std::cout << "   - va_list 是指向栈的指针" << std::endl;
    std::cout << "   - va_arg 根据类型移动指针" << std::endl;

    std::cout << "\n4. 类型安全：" << std::endl;
    std::cout << "   - 没有编译时类型检查" << std::endl;
    std::cout << "   - 类型不匹配导致未定义行为" << std::endl;
    std::cout << "   - C++11 可变参数模板提供类型安全" << std::endl;

    std::cout << "\n5. 参数提升：" << std::endl;
    std::cout << "   - char, short → int" << std::endl;
    std::cout << "   - float → double" << std::endl;
    std::cout << "   - va_arg 必须使用提升后的类型" << std::endl;

    std::cout << "\n6. 跨平台：" << std::endl;
    std::cout << "   - 不同平台的调用约定不同" << std::endl;
    std::cout << "   - va_list 的实现会处理差异" << std::endl;
    std::cout << "   - 使用标准宏保证可移植性" << std::endl;

    std::cout << "\n7. 实际应用：" << std::endl;
    std::cout << "   - printf, scanf 系列函数" << std::endl;
    std::cout << "   - 日志函数" << std::endl;
    std::cout << "   - 格式化输出" << std::endl;
    std::cout << "   - 调试函数" << std::endl;

    return 0;
}
