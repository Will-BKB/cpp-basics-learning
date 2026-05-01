/**
 * 教学程序：volatile 关键字
 * 目标：理解 volatile 如何禁止编译器优化内存访问
 *
 * 核心知识点：
 * 1. volatile 关键字的作用
 * 2. volatile 如何防止编译器优化
 * 3. volatile 的实际应用场景
 * 4. volatile 与 const 的区别
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

// 演示 volatile 的基本作用
void demonstrate_volatile_basic() {
    std::cout << "========================================" << std::endl;
    std::cout << "volatile 的基本作用" << std::endl;
    std::cout << "========================================" << std::endl;

    // 普通变量
    int normal = 0;

    // volatile 变量
    volatile int vol = 0;

    std::cout << "普通变量: " << normal << std::endl;
    std::cout << "volatile 变量: " << vol << std::endl;

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 两个变量的声明方式不同" << std::endl;
    std::cout << "- volatile 告诉编译器不要优化对这个变量的访问" << std::endl;
    std::cout << "- 每次访问 volatile 变量都会从内存中读取" << std::endl;
    std::cout << std::endl;
}

// 演示 volatile 防止优化
void demonstrate_volatile_optimization() {
    std::cout << "========================================" << std::endl;
    std::cout << "volatile 防止优化" << std::endl;
    std::cout << "========================================" << std::endl;

    // 普通变量 - 可能被优化
    int normal = 10;
    for (int i = 0; i < 5; i++) {
        normal = normal;  // 编译器可能优化掉这行代码
    }

    // volatile 变量 - 不会被优化
    volatile int vol = 10;
    for (int i = 0; i < 5; i++) {
        vol = vol;  // 编译器必须执行这行代码
    }

    std::cout << "普通变量循环后: " << normal << std::endl;
    std::cout << "volatile 变量循环后: " << vol << std::endl;

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 普通变量的赋值可能被编译器优化掉" << std::endl;
    std::cout << "- volatile 变量的赋值必须执行" << std::endl;
    std::cout << "- 使用反汇编工具可以观察到差异" << std::endl;
    std::cout << std::endl;
}

// 演示 volatile 的实际应用场景
void demonstrate_volatile_use_cases() {
    std::cout << "========================================" << std::endl;
    std::cout << "volatile 的实际应用场景" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "1. 硬件寄存器访问：" << std::endl;
    std::cout << "   volatile uint32_t* status_reg = (uint32_t*)0x4000;" << std::endl;
    std::cout << "   while (*status_reg & 0x01) { /* 等待 */ }" << std::endl;
    std::cout << "   - 硬件寄存器的值可能被硬件改变" << std::endl;
    std::cout << "   - 编译器不能优化掉对寄存器的读取" << std::endl;

    std::cout << "\n2. 中断服务程序中的共享变量：" << std::endl;
    std::cout << "   volatile int interrupt_flag = 0;" << std::endl;
    std::cout << "   - 中断可能修改变量的值" << std::endl;
    std::cout << "   - 主程序需要看到最新的值" << std::endl;

    std::cout << "\n3. 多线程环境中的共享变量（注意：不是线程安全的）：" << std::endl;
    std::cout << "   volatile bool stop_flag = false;" << std::endl;
    std::cout << "   - volatile 保证可见性，但不保证原子性" << std::endl;
    std::cout << "   - 多线程编程应该使用 std::atomic" << std::endl;

    std::cout << "\n4. 内存映射 I/O：" << std::endl;
    std::cout << "   volatile uint8_t* mmio = (uint8_t*)0x1000;" << std::endl;
    std::cout << "   - MMIO 区域的值可能被外部设备改变" << std::endl;
    std::cout << "   - 每次访问都必须从内存读取" << std::endl;
    std::cout << std::endl;
}

// 演示 volatile 与 const 的区别
void demonstrate_volatile_vs_const() {
    std::cout << "========================================" << std::endl;
    std::cout << "volatile 与 const 的区别" << std::endl;
    std::cout << "========================================" << std::endl;

    // const 变量 - 不能修改
    const int const_val = 10;
    // const_val = 20;  // 编译错误

    // volatile 变量 - 可以修改，但访问不被优化
    volatile int vol_val = 10;
    vol_val = 20;  // 可以修改

    // const volatile 变量 - 不能通过这个变量修改，但访问不被优化
    const volatile int const_vol_val = 10;
    // const_vol_val = 20;  // 编译错误

    std::cout << "const 变量: " << const_val << std::endl;
    std::cout << "volatile 变量: " << vol_val << std::endl;
    std::cout << "const volatile 变量: " << const_vol_val << std::endl;

    std::cout << "\n区别总结：" << std::endl;
    std::cout << "- const: 变量是只读的，编译器可以优化访问" << std::endl;
    std::cout << "- volatile: 变量可能被外部改变，编译器不能优化访问" << std::endl;
    std::cout << "- const volatile: 变量是只读的，但可能被外部改变" << std::endl;
    std::cout << std::endl;
}

// 演示 volatile 指针
void demonstrate_volatile_pointer() {
    std::cout << "========================================" << std::endl;
    std::cout << "volatile 指针" << std::endl;
    std::cout << "========================================" << std::endl;

    int value = 10;

    // 指向 volatile int 的指针
    volatile int* ptr_to_volatile = &value;

    // volatile 指针（指针本身是 volatile）
    int* volatile volatile_ptr = &value;

    // volatile 指针指向 volatile int
    volatile int* volatile ptr_both = &value;

    std::cout << "指向 volatile int 的指针: " << *ptr_to_volatile << std::endl;
    std::cout << "volatile 指针: " << *volatile_ptr << std::endl;
    std::cout << "volatile 指针指向 volatile int: " << *ptr_both << std::endl;

    std::cout << "\n区别：" << std::endl;
    std::cout << "- volatile int*: 指向的数据是 volatile 的" << std::endl;
    std::cout << "- int* volatile: 指针本身是 volatile 的" << std::endl;
    std::cout << "- volatile int* volatile: 两者都是 volatile 的" << std::endl;
    std::cout << std::endl;
}

// 主函数
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "volatile 关键字" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "编译说明：" << std::endl;
    std::cout << "g++ -g -O0 -std=c++17 03_volatile.cpp -o 03_volatile_O0" << std::endl;
    std::cout << "g++ -g -O2 -std=c++17 03_volatile.cpp -o 03_volatile_O2" << std::endl;
    std::cout << std::endl;

    std::cout << "反汇编对比：" << std::endl;
    std::cout << "objdump -d 03_volatile_O0 | grep -A 10 'demonstrate_volatile_optimization'" << std::endl;
    std::cout << "objdump -d 03_volatile_O2 | grep -A 10 'demonstrate_volatile_optimization'" << std::endl;
    std::cout << std::endl;

    std::cout << "平台: " << PLATFORM_NAME << std::endl;
    std::cout << std::endl;

    // 演示 volatile 的基本作用
    demonstrate_volatile_basic();

    // 演示 volatile 防止优化
    demonstrate_volatile_optimization();

    // 演示 volatile 的实际应用场景
    demonstrate_volatile_use_cases();

    // 演示 volatile 与 const 的区别
    demonstrate_volatile_vs_const();

    // 演示 volatile 指针
    demonstrate_volatile_pointer();

    std::cout << "========================================" << std::endl;
    std::cout << "总结" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. volatile 关键字的作用：" << std::endl;
    std::cout << "   - 告诉编译器不要优化对这个变量的访问" << std::endl;
    std::cout << "   - 每次访问都从内存中读取" << std::endl;
    std::cout << "   - 防止编译器进行某些优化" << std::endl;

    std::cout << "\n2. volatile 防止的优化：" << std::endl;
    std::cout << "   - 内存访问优化（缓存到寄存器）" << std::endl;
    std::cout << "   - 死代码消除" << std::endl;
    std::cout << "   - 循环优化" << std::endl;

    std::cout << "\n3. 实际应用场景：" << std::endl;
    std::cout << "   - 硬件寄存器访问" << std::endl;
    std::cout << "   - 中断服务程序中的共享变量" << std::endl;
    std::cout << "   - 内存映射 I/O" << std::endl;
    std::cout << "   - 注意：多线程应该使用 std::atomic" << std::endl;

    std::cout << "\n4. volatile 与 const 的区别：" << std::endl;
    std::cout << "   - const: 变量是只读的" << std::endl;
    std::cout << "   - volatile: 变量可能被外部改变" << std::endl;
    std::cout << "   - 可以同时使用 const 和 volatile" << std::endl;

    std::cout << "\n5. volatile 指针：" << std::endl;
    std::cout << "   - volatile int*: 指向的数据是 volatile 的" << std::endl;
    std::cout << "   - int* volatile: 指针本身是 volatile 的" << std::endl;
    std::cout << "   - volatile int* volatile: 两者都是 volatile 的" << std::endl;

    std::cout << "\n6. 注意事项：" << std::endl;
    std::cout << "   - volatile 不是线程安全的" << std::endl;
    std::cout << "   - 多线程编程应该使用 std::atomic" << std::endl;
    std::cout << "   - volatile 只保证可见性，不保证原子性" << std::endl;
    std::cout << "   - 过度使用 volatile 会影响性能" << std::endl;

    return 0;
}
