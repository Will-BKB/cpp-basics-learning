/**
 * 教学程序：函数重载与 Name Mangling
 * 目标：理解函数重载的工作原理和 name mangling 机制
 *
 * 核心知识点：
 * 1. 函数重载的基本概念
 * 2. Name mangling 的工作原理
 * 3. 如何查看符号名
 * 4. 跨平台的 name mangling 差异
 */

#include <iostream>
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

// 函数重载示例：同名函数，不同参数

void print(int value) {
    std::cout << "int: " << value << std::endl;
}

void print(double value) {
    std::cout << "double: " << value << std::endl;
}

void print(const std::string& value) {
    std::cout << "string: " << value << std::endl;
}

void print(int a, int b) {
    std::cout << "int, int: " << a << ", " << b << std::endl;
}

// 演示函数重载
void demonstrate_function_overloading() {
    std::cout << "========================================" << std::endl;
    std::cout << "函数重载演示" << std::endl;
    std::cout << "========================================" << std::endl;

    // 调用不同的重载函数
    print(42);                    // 调用 print(int)
    print(3.14);                  // 调用 print(double)
    print(std::string("Hello"));  // 调用 print(string)
    print(10, 20);                // 调用 print(int, int)

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 同名函数可以根据参数类型自动选择" << std::endl;
    std::cout << "- 编译器根据参数列表选择合适的函数" << std::endl;
    std::cout << "- 这就是函数重载的基本原理" << std::endl;
    std::cout << std::endl;
}

// 演示函数指针与重载
void demonstrate_function_pointer_overloading() {
    std::cout << "========================================" << std::endl;
    std::cout << "函数指针与重载" << std::endl;
    std::cout << "========================================" << std::endl;

    // 函数指针必须明确指定类型
    void (*int_ptr)(int) = print;
    void (*double_ptr)(double) = print;

    // 调用函数指针
    int_ptr(100);      // 调用 print(int)
    double_ptr(2.71);  // 调用 print(double)

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 函数指针必须明确指定参数类型" << std::endl;
    std::cout << "- 不能直接写：void (*ptr)() = print;" << std::endl;
    std::cout << "- 编译器需要知道具体调用哪个重载版本" << std::endl;
    std::cout << std::endl;
}

// 演示 name mangling
void demonstrate_name_mangling() {
    std::cout << "========================================" << std::endl;
    std::cout << "Name Mangling 演示" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "函数重载在源代码中看起来同名：" << std::endl;
    std::cout << "  void print(int)" << std::endl;
    std::cout << "  void print(double)" << std::endl;
    std::cout << "  void print(string)" << std::endl;
    std::cout << "  void print(int, int)" << std::endl;

    std::cout << "\n但在编译后的符号表中，它们有不同的名字：" << std::endl;
    std::cout << "  使用 nm 命令查看：" << std::endl;
    std::cout << "  nm " << PLATFORM_NAME << " | grep print" << std::endl;

    std::cout << "\nName Mangling 的作用：" << std::endl;
    std::cout << "- 将函数名和参数类型编码到符号名中" << std::endl;
    std::cout << "- 链接器可以区分不同的重载函数" << std::endl;
    std::cout << "- 支持 C++ 的函数重载特性" << std::endl;
    std::cout << std::endl;
}

// 演示 C 和 C++ 的 name mangling 差异
extern "C" {
    // C 风格函数（不进行 name mangling）
    void c_function(int value) {
        std::cout << "C function: " << value << std::endl;
    }
}

void demonstrate_c_vs_cpp_mangling() {
    std::cout << "========================================" << std::endl;
    std::cout << "C vs C++ Name Mangling" << std::endl;
    std::cout << "========================================" << std::endl;

    // 调用 C 风格函数
    c_function(42);

    std::cout << "\nC vs C++ 的差异：" << std::endl;
    std::cout << "- C 函数：不进行 name mangling" << std::endl;
    std::cout << "- C++ 函数：进行 name mangling" << std::endl;
    std::cout << "- extern \"C\"：禁用 C++ 的 name mangling" << std::endl;

    std::cout << "\n符号名对比：" << std::endl;
    std::cout << "- C 函数：c_function" << std::endl;
    std::cout << "- C++ 函数：_Z5printi (示例)" << std::endl;

    std::cout << "\n为什么需要 extern \"C\"：" << std::endl;
    std::cout << "- 与 C 代码交互" << std::endl;
    std::cout << "- 避免链接错误" << std::endl;
    std::cout << "- 保证符号名兼容" << std::endl;
    std::cout << std::endl;
}

// 主函数
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "函数重载与 Name Mangling" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "编译说明：" << std::endl;
    std::cout << "g++ -g -Wall -Wextra -std=c++17 05_name_mangling.cpp -o 05_name_mangling" << std::endl;
    std::cout << std::endl;

    std::cout << "查看符号名：" << std::endl;
    std::cout << "nm 05_name_mangling | grep print" << std::endl;
    std::cout << "nm 05_name_mangling | c++filt" << std::endl;
    std::cout << std::endl;

    std::cout << "平台: " << PLATFORM_NAME << std::endl;
    std::cout << std::endl;

    // 演示函数重载
    demonstrate_function_overloading();

    // 演示函数指针与重载
    demonstrate_function_pointer_overloading();

    // 演示 name mangling
    demonstrate_name_mangling();

    // 演示 C vs C++ 的差异
    demonstrate_c_vs_cpp_mangling();

    std::cout << "========================================" << std::endl;
    std::cout << "总结" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. 函数重载：" << std::endl;
    std::cout << "   - 同名函数，不同参数列表" << std::endl;
    std::cout << "   - 编译器根据参数类型选择合适的函数" << std::endl;
    std::cout << "   - 参数类型、参数个数、参数顺序都可以不同" << std::endl;

    std::cout << "\n2. Name Mangling：" << std::endl;
    std::cout << "   - 将函数名和参数类型编码到符号名中" << std::endl;
    std::cout << "   - 链接器可以区分不同的重载函数" << std::endl;
    std::cout << "   - 支持 C++ 的函数重载特性" << std::endl;

    std::cout << "\n3. Name Mangling 规则：" << std::endl;
    std::cout << "   - 编译器特定的编码规则" << std::endl;
    std::cout << "   - 不同编译器可能使用不同的规则" << std::endl;
    std::cout << "   - GCC/Clang 使用 Itanium C++ ABI" << std::endl;

    std::cout << "\n4. 查看符号名：" << std::endl;
    std::cout << "   - 使用 nm 命令查看符号表" << std::endl;
    std::cout << "   - 使用 c++filt 解码符号名" << std::endl;
    std::cout << "   - 示例：nm program | c++filt" << std::endl;

    std::cout << "\n5. extern \"C\"：" << std::endl;
    std::cout << "   - 禁用 C++ 的 name mangling" << std::endl;
    std::cout << "   - 用于与 C 代码交互" << std::endl;
    std::cout << "   - 保证符号名与 C 兼容" << std::endl;

    std::cout << "\n6. 跨平台注意事项：" << std::endl;
    std::cout << "   - Name mangling 规则可能因平台而异" << std::endl;
    std::cout << "   - 但基本原理相同" << std::endl;
    std::cout << "   - 使用 c++filt 可以解码符号名" << std::endl;

    return 0;
}