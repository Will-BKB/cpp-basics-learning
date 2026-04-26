/**
 * 教学程序：内联函数
 * 目标：理解内联函数的工作原理和优化效果
 *
 * 核心知识点：
 * 1. inline 关键字的作用
 * 2. 内联函数如何避免函数调用开销
 * 3. 编译器优化级别对内联的影响
 * 4. 如何通过反汇编观察内联效果
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

// 普通函数（不使用 inline）
int square_normal(int x) {
    return x * x;
}

// 内联函数（使用 inline）
inline int square_inline(int x) {
    return x * x;
}

// 演示函数调用开销
void demonstrate_function_call() {
    std::cout << "========================================" << std::endl;
    std::cout << "函数调用演示" << std::endl;
    std::cout << "========================================" << std::endl;

    int a = 5;

    // 调用普通函数
    int result1 = square_normal(a);
    std::cout << "square_normal(" << a << ") = " << result1 << std::endl;

    // 调用内联函数
    int result2 = square_inline(a);
    std::cout << "square_inline(" << a << ") = " << result2 << std::endl;

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 两个函数的调用方式看起来相同" << std::endl;
    std::cout << "- 但编译后的汇编代码可能不同" << std::endl;
    std::cout << "- 内联函数可能被直接展开，避免函数调用开销" << std::endl;
    std::cout << std::endl;
}

// 演示内联函数的优势
void demonstrate_inline_advantage() {
    std::cout << "========================================" << std::endl;
    std::cout << "内联函数的优势" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "1. 避免函数调用开销：" << std::endl;
    std::cout << "   - 不需要保存/恢复寄存器" << std::endl;
    std::cout << "   - 不需要压栈/出栈" << std::endl;
    std::cout << "   - 不需要跳转指令" << std::endl;

    std::cout << "\n2. 启用更多优化：" << std::endl;
    std::cout << "   - 编译器可以看到完整的代码" << std::endl;
    std::cout << "   - 可以进行常量折叠" << std::endl;
    std::cout << "   - 可以进行死代码消除" << std::endl;

    std::cout << "\n3. 提高缓存局部性：" << std::endl;
    std::cout << "   - 代码在调用处展开" << std::endl;
    std::cout << "   - 减少指令跳转" << std::endl;
    std::cout << std::endl;
}

// 演示内联函数的劣势
void demonstrate_inline_disadvantage() {
    std::cout << "========================================" << std::endl;
    std::cout << "内联函数的劣势" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "1. 代码膨胀：" << std::endl;
    std::cout << "   - 每次调用都会展开代码" << std::endl;
    std::cout << "   - 增加可执行文件大小" << std::endl;
    std::cout << "   - 可能影响指令缓存" << std::endl;

    std::cout << "\n2. 编译时间增加：" << std::endl;
    std::cout << "   - 需要处理更多代码" << std::endl;
    std::cout << "   - 增加编译时间" << std::endl;

    std::cout << "\n3. 不适合大函数：" << std::endl;
    std::cout << "   - 大函数内联收益小" << std::endl;
    std::cout << "   - 编译器可能拒绝内联" << std::endl;
    std::cout << std::endl;
}

// 演示编译器优化级别的影响
void demonstrate_optimization_levels() {
    std::cout << "========================================" << std::endl;
    std::cout << "编译器优化级别的影响" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "不同优化级别下的行为：" << std::endl;
    std::cout << "-O0 (无优化)：" << std::endl;
    std::cout << "  - inline 关键字可能被忽略" << std::endl;
    std::cout << "  - 普通函数和内联函数都生成 call 指令" << std::endl;

    std::cout << "\n-O2 (中等优化)：" << std::endl;
    std::cout << "  - 编译器会自动内联小函数" << std::endl;
    std::cout << "  - 即使没有 inline 关键字" << std::endl;
    std::cout << "  - inline 关键字只是建议" << std::endl;

    std::cout << "\n-O3 (最高优化)：" << std::endl;
    std::cout << "  - 更激进的优化" << std::endl;
    std::cout << "  - 可能内联更大的函数" << std::endl;
    std::cout << std::endl;
}

// 演示常量折叠优化
void demonstrate_constant_folding() {
    std::cout << "========================================" << std::endl;
    std::cout << "常量折叠优化" << std::endl;
    std::cout << "========================================" << std::endl;

    // 编译时常量
    constexpr int compile_time_const = 5;

    // 内联函数 + 常量 = 编译时计算
    int result = square_inline(compile_time_const);

    std::cout << "square_inline(5) = " << result << std::endl;
    std::cout << "\n优化效果：" << std::endl;
    std::cout << "- 内联函数允许编译器看到完整代码" << std::endl;
    std::cout << "- 常量参数可以在编译时计算" << std::endl;
    std::cout << "- 最终代码可能只是：mov eax, 25" << std::endl;
    std::cout << std::endl;
}

// 主函数
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "内联函数" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "编译说明：" << std::endl;
    std::cout << "# 无优化（观察函数调用）" << std::endl;
    std::cout << "g++ -g -O0 -std=c++17 02_inline_functions.cpp -o 02_inline_O0" << std::endl;
    std::cout << std::endl;
    std::cout << "# 有优化（观察内联效果）" << std::endl;
    std::cout << "g++ -g -O2 -std=c++17 02_inline_functions.cpp -o 02_inline_O2" << std::endl;
    std::cout << std::endl;
    std::cout << "# 反汇编对比" << std::endl;
    std::cout << "objdump -d 02_inline_O0 | grep -A 20 'main'" << std::endl;
    std::cout << "objdump -d 02_inline_O2 | grep -A 20 'main'" << std::endl;
    std::cout << std::endl;

    std::cout << "平台: " << PLATFORM_NAME << std::endl;
    std::cout << std::endl;

    // 演示函数调用
    demonstrate_function_call();

    // 演示内联函数的优势
    demonstrate_inline_advantage();

    // 演示内联函数的劣势
    demonstrate_inline_disadvantage();

    // 演示优化级别的影响
    demonstrate_optimization_levels();

    // 演示常量折叠
    demonstrate_constant_folding();

    std::cout << "========================================" << std::endl;
    std::cout << "总结" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. inline 关键字：" << std::endl;
    std::cout << "   - 只是给编译器的建议" << std::endl;
    std::cout << "   - 编译器可以选择忽略" << std::endl;
    std::cout << "   - 现代编译器会自动内联小函数" << std::endl;

    std::cout << "\n2. 内联函数的优势：" << std::endl;
    std::cout << "   - 避免函数调用开销" << std::endl;
    std::cout << "   - 启用更多优化" << std::endl;
    std::cout << "   - 提高缓存局部性" << std::endl;

    std::cout << "\n3. 内联函数的劣势：" << std::endl;
    std::cout << "   - 代码膨胀" << std::endl;
    std::cout << "   - 编译时间增加" << std::endl;
    std::cout << "   - 不适合大函数" << std::endl;

    std::cout << "\n4. 使用建议：" << std::endl;
    std::cout << "   - 小函数（几行代码）适合内联" << std::endl;
    std::cout << "   - 频繁调用的函数适合内联" << std::endl;
    std::cout << "   - 大函数避免内联" << std::endl;
    std::cout << "   - 信任编译器的优化决策" << std::endl;

    std::cout << "\n5. 验证方法：" << std::endl;
    std::cout << "   - 使用反汇编工具观察 call 指令" << std::endl;
    std::cout << "   - 对比不同优化级别的汇编输出" << std::endl;
    std::cout << "   - 使用 Compiler Explorer (godbolt.org)" << std::endl;

    return 0;
}