/**
 * 教学程序：Lambda 表达式的底层实现
 * 目标：理解 lambda 表达式如何转换为函数对象
 *
 * 核心知识点：
 * 1. Lambda 表达式的基本语法
 * 2. Lambda 表达式如何转换为函数对象
 * 3. 捕获变量的存储方式
 * 4. Lambda 表达式的类型
 */

#include <iostream>
#include <functional>

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

// 演示基本的 lambda 表达式
void demonstrate_basic_lambda() {
    std::cout << "========================================" << std::endl;
    std::cout << "基本的 Lambda 表达式" << std::endl;
    std::cout << "========================================" << std::endl;

    // Lambda 表达式
    auto add = [](int a, int b) {
        return a + b;
    };

    // 调用 lambda
    int result = add(3, 4);
    std::cout << "add(3, 4) = " << result << std::endl;

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- Lambda 表达式看起来像函数" << std::endl;
    std::cout << "- 实际上是一个函数对象（仿函数）" << std::endl;
    std::cout << "- 编译器会自动生成一个类" << std::endl;
    std::cout << std::endl;
}

// 演示 lambda 捕获变量
void demonstrate_lambda_capture() {
    std::cout << "========================================" << std::endl;
    std::cout << "Lambda 捕获变量" << std::endl;
    std::cout << "========================================" << std::endl;

    int x = 10;
    int y = 20;

    // 捕获外部变量
    auto add_with_capture = [x, y]() {
        return x + y;
    };

    int result = add_with_capture();
    std::cout << "add_with_capture() = " << result << std::endl;
    std::cout << "x = " << x << ", y = " << y << std::endl;

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- Lambda 可以捕获外部变量" << std::endl;
    std::cout << std::endl;
}

// 演示 lambda 的类型
void demonstrate_lambda_type() {
    std::cout << "========================================" << std::endl;
    std::cout << "Lambda 的类型" << std::endl;
    std::cout << "========================================" << std::endl;

    // Lambda 表达式
    auto lambda1 = [](int a, int b) {
        return a + b;
    };

    auto lambda2 = [](int a, int b) {
        return a + b;
    };

    std::cout << "lambda1 和 lambda2 类型相同吗？" << std::endl;
    std::cout << "typeid(lambda1).name() = " << typeid(lambda1).name() << std::endl;
    std::cout << "typeid(lambda2).name() = " << typeid(lambda2).name() << std::endl;

    // 每个 lambda 都有唯一的类型
    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 每个 lambda 表达式都有唯一的类型" << std::endl;
    std::cout << "- 即使语法相同，类型也不同" << std::endl;
    std::cout << "- 编译器为每个 lambda 生成唯一的类" << std::endl;
    std::cout << std::endl;
}

// 演示 lambda 作为函数参数
void demonstrate_lambda_as_parameter() {
    std::cout << "========================================" << std::endl;
    std::cout << "Lambda 作为函数参数" << std::endl;
    std::cout << "========================================" << std::endl;

    // 接受函数对象的函数
    auto apply_operation = [](std::function<int(int, int)> operation, int a, int b) {
        return operation(a, b);
    };

    // 传递 lambda
    int result1 = apply_operation([](int a, int b) { return a + b; }, 5, 3);
    std::cout << "apply_operation(add, 5, 3) = " << result1 << std::endl;

    int result2 = apply_operation([](int a, int b) { return a * b; }, 4, 6);
    std::cout << "apply_operation(multiply, 4, 6) = " << result2 << std::endl;

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- Lambda 可以作为参数传递" << std::endl;
    std::cout << "- 使用 std::function 作为参数类型" << std::endl;
    std::cout << "- 这与函数指针类似，但更灵活" << std::endl;
    std::cout << std::endl;
}

// 演示 lambda 的大小
void demonstrate_lambda_size() {
    std::cout << "========================================" << std::endl;
    std::cout << "Lambda 的大小" << std::endl;
    std::cout << "========================================" << std::endl;

    // 不捕获变量的 lambda
    auto lambda_no_capture = [](int a, int b) {
        return a + b;
    };

    // 捕获变量的 lambda
    int x = 10;
    int y = 20;
    auto lambda_with_capture = [x, y]() {
        return x + y;
    };

    std::cout << "不捕获变量的 lambda 大小: " << sizeof(lambda_no_capture) << " bytes" << std::endl;
    std::cout << "捕获变量的 lambda 大小: " << sizeof(lambda_with_capture) << " bytes" << std::endl;

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 不捕获变量的 lambda 大小通常为 1 byte" << std::endl;
    std::cout << "- 捕获变量的 lambda 大小取决于捕获的变量" << std::endl;
    std::cout << "- 捕获的变量存储在 lambda 对象中" << std::endl;
    std::cout << std::endl;
}

// 主函数
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Lambda 表达式的底层实现" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "编译说明：" << std::endl;
    std::cout << "g++ -g -Wall -Wextra -std=c++17 04_lambda_implementation.cpp -o 04_lambda_implementation" << std::endl;
    std::cout << std::endl;

    std::cout << "平台: " << PLATFORM_NAME << std::endl;
    std::cout << std::endl;

    // 演示基本的 lambda 表达式
    demonstrate_basic_lambda();

    // 演示 lambda 捕获变量
    demonstrate_lambda_capture();

    // 演示 lambda 的类型
    demonstrate_lambda_type();

    // 演示 lambda 作为函数参数
    demonstrate_lambda_as_parameter();

    // 演示 lambda 的大小
    demonstrate_lambda_size();

    std::cout << "========================================" << std::endl;
    std::cout << "总结" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. Lambda 表达式语法：" << std::endl;
    std::cout << "   - 基本形式：[capture](parameters) -> return_type { body }" << std::endl;
    std::cout << "   - 示例：[](int a, int b) { return a + b; }" << std::endl;
    std::cout << "   - 捕获列表：[] 不捕获，[x] 捕获 x，[&x] 捕获 x 的引用" << std::endl;

    std::cout << "\n2. 底层实现：" << std::endl;
    std::cout << "   - Lambda 表达式被转换为函数对象（仿函数）" << std::endl;
    std::cout << "   - 编译器自动生成一个类" << std::endl;
    std::cout << "   - 类中包含 operator() 重载" << std::endl;
    std::cout << "   - 捕获的变量成为类的成员变量" << std::endl;

    std::cout << "\n3. Lambda 的类型：" << std::endl;
    std::cout << "   - 每个 lambda 都有唯一的类型" << std::endl;
    std::cout << "   - 类型由编译器自动生成" << std::endl;
    std::cout << "   - 使用 auto 或 std::function 存储" << std::endl;

    std::cout << "\n4. 捕获机制：" << std::endl;
    std::cout << "   - 值捕获：复制变量的值到 lambda 对象" << std::endl;
    std::cout << "   - 引用捕获：存储变量的引用" << std::endl;
    std::cout << "   - 捕获的变量影响 lambda 对象的大小" << std::endl;

    std::cout << "\n5. 与函数指针的区别：" << std::endl;
    std::cout << "   - Lambda 是函数对象，不是函数指针" << std::endl;
    std::cout << "   - Lambda 可以捕获状态，函数指针不能" << std::endl;
    std::cout << "   - Lambda 更灵活，性能通常更好" << std::endl;

    std::cout << "\n6. 实际应用：" << std::endl;
    std::cout << "   - 算法参数（std::sort, std::for_each）" << std::endl;
    std::cout << "   - 回调函数" << std::endl;
    std::cout << "   - 事件处理" << std::endl;
    std::cout << "   - 延迟执行" << std::endl;

    return 0;
}