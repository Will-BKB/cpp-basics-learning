/**
 * 教学程序：函数指针与回调
 * 目标：理解函数指针的基本概念和回调机制
 *
 * 核心知识点：
 * 1. 函数指针的声明和使用
 * 2. 函数指针作为参数（回调）
 * 3. 函数指针的底层实现
 * 4. 跨平台的函数指针行为
 */

#include <iostream>

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

// 简单的数学运算函数
int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

// 函数指针类型定义
typedef int (*OperationFunc)(int, int);

// 使用函数指针的函数（回调）
int calculate(OperationFunc operation, int a, int b) {
    return operation(a, b);
}

// 演示函数指针的基本使用
void demonstrate_function_pointer() {
    std::cout << "========================================" << std::endl;
    std::cout << "函数指针的基本使用" << std::endl;
    std::cout << "========================================" << std::endl;

    // 声明函数指针
    OperationFunc func_ptr;

    // 函数指针赋值
    func_ptr = add;
    std::cout << "函数指针指向 add 函数" << std::endl;
    std::cout << "func_ptr(3, 4) = " << func_ptr(3, 4) << std::endl;

    // 函数指针重新赋值
    func_ptr = subtract;
    std::cout << "函数指针指向 subtract 函数" << std::endl;
    std::cout << "func_ptr(10, 3) = " << func_ptr(10, 3) << std::endl;

    // 直接使用函数名
    func_ptr = multiply;
    std::cout << "函数指针指向 multiply 函数" << std::endl;
    std::cout << "func_ptr(6, 7) = " << func_ptr(6, 7) << std::endl;

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 函数指针可以指向不同的函数" << std::endl;
    std::cout << "- 通过函数指针调用函数" << std::endl;
    std::cout << "- 函数指针的调用方式与普通函数相同" << std::endl;
    std::cout << std::endl;
}

// 演示回调函数
void demonstrate_callback() {
    std::cout << "========================================" << std::endl;
    std::cout << "回调函数" << std::endl;
    std::cout << "========================================" << std::endl;

    // 使用不同的回调函数
    int result1 = calculate(add, 5, 3);
    std::cout << "calculate(add, 5, 3) = " << result1 << std::endl;

    int result2 = calculate(subtract, 10, 4);
    std::cout << "calculate(subtract, 10, 4) = " << result2 << std::endl;

    int result3 = calculate(multiply, 6, 7);
    std::cout << "calculate(multiply, 6, 7) = " << result3 << std::endl;

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 函数指针作为参数传递" << std::endl;
    std::cout << "- 同一个函数可以执行不同的操作" << std::endl;
    std::cout << "- 这就是回调机制的基本原理" << std::endl;
    std::cout << std::endl;
}

// 演示函数指针的地址
void demonstrate_function_address() {
    std::cout << "========================================" << std::endl;
    std::cout << "函数指针的地址" << std::endl;
    std::cout << "========================================" << std::endl;

    // 打印函数地址
    std::cout << "add 函数地址: " << (void*)add << std::endl;
    std::cout << "subtract 函数地址: " << (void*)subtract << std::endl;
    std::cout << "multiply 函数地址: " << (void*)multiply << std::endl;

    // 函数指针的值
    OperationFunc ptr1 = add;
    OperationFunc ptr2 = subtract;
    OperationFunc ptr3 = multiply;

    std::cout << "\n函数指针的值：" << std::endl;
    std::cout << "ptr1 = " << (void*)ptr1 << std::endl;
    std::cout << "ptr2 = " << (void*)ptr2 << std::endl;
    std::cout << "ptr3 = " << (void*)ptr3 << std::endl;

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 函数名本身就是函数的地址" << std::endl;
    std::cout << "- 函数指针存储的是函数的地址" << std::endl;
    std::cout << "- 不同函数有不同的地址" << std::endl;
    std::cout << std::endl;
}

// 演示函数指针数组
void demonstrate_function_pointer_array() {
    std::cout << "========================================" << std::endl;
    std::cout << "函数指针数组" << std::endl;
    std::cout << "========================================" << std::endl;

    // 函数指针数组
    OperationFunc operations[] = {add, subtract, multiply};

    // 通过数组调用不同的函数
    for (int i = 0; i < 3; i++) {
        int result = operations[i](10, 5);
        std::cout << "operations[" << i << "](10, 5) = " << result << std::endl;
    }

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 函数指针可以存储在数组中" << std::endl;
    std::cout << "- 可以通过索引选择不同的函数" << std::endl;
    std::cout << "- 这在实现状态机或命令模式时很有用" << std::endl;
    std::cout << std::endl;
}

// 主函数
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "函数指针与回调" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "编译说明：" << std::endl;
    std::cout << "g++ -g -Wall -Wextra -std=c++17 03_function_pointer.cpp -o 03_function_pointer" << std::endl;
    std::cout << std::endl;

    std::cout << "平台: " << PLATFORM_NAME << std::endl;
    std::cout << std::endl;

    // 演示函数指针的基本使用
    demonstrate_function_pointer();

    // 演示回调函数
    demonstrate_callback();

    // 演示函数指针的地址
    demonstrate_function_address();

    // 演示函数指针数组
    demonstrate_function_pointer_array();

    std::cout << "========================================" << std::endl;
    std::cout << "总结" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. 函数指针声明：" << std::endl;
    std::cout << "   - 使用 typedef 简化声明" << std::endl;
    std::cout << "   - 格式：返回类型 (*指针名)(参数类型)" << std::endl;
    std::cout << "   - 示例：typedef int (*OperationFunc)(int, int)" << std::endl;

    std::cout << "\n2. 函数指针赋值：" << std::endl;
    std::cout << "   - 函数名本身就是函数的地址" << std::endl;
    std::cout << "   - 可以直接赋值：ptr = function_name" << std::endl;
    std::cout << "   - 可以重新赋值指向不同的函数" << std::endl;

    std::cout << "\n3. 函数指针调用：" << std::endl;
    std::cout << "   - 调用方式与普通函数相同" << std::endl;
    std::cout << "   - 示例：ptr(arg1, arg2)" << std::endl;
    std::cout << "   - 编译器会生成适当的调用指令" << std::endl;

    std::cout << "\n4. 回调函数：" << std::endl;
    std::cout << "   - 函数指针作为参数传递" << std::endl;
    std::cout << "   - 允许调用者指定具体的行为" << std::endl;
    std::cout << "   - 常用于事件处理、排序算法等" << std::endl;

    std::cout << "\n5. 底层实现：" << std::endl;
    std::cout << "   - 函数指针存储的是函数的地址" << std::endl;
    std::cout << "   - x86-64: 使用 call *ptr 指令" << std::endl;
    std::cout << "   - ARM64: 使用 blr x0 指令" << std::endl;
    std::cout << "   - 跨平台行为一致" << std::endl;

    std::cout << "\n6. 实际应用：" << std::endl;
    std::cout << "   - 回调函数（事件处理）" << std::endl;
    std::cout << "   - 策略模式（算法选择）" << std::endl;
    std::cout << "   - 与 C 代码交互" << std::endl;
    std::cout << "   - 函数指针数组（状态机）" << std::endl;

    return 0;
}