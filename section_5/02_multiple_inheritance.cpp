/**
 * 教学程序：多重继承与 this 指针
 * 目标：理解多重继承下的虚函数表布局和 this 指针调整
 *
 * 核心知识点：
 * 1. 多重继承的对象内存布局
 * 2. 多个 vptr 的存在
 * 3. this 指针调整机制
 * 4. 类型转换时的指针偏移
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

// 基类1
class Base1 {
public:
    virtual void func1() {
        std::cout << "  Base1::func1(), this = " << reinterpret_cast<void*>(this) << std::endl;
    }

    virtual void func2() {
        std::cout << "  Base1::func2(), this = " << reinterpret_cast<void*>(this) << std::endl;
    }

    virtual ~Base1() {
        std::cout << "  Base1::~Base1()" << std::endl;
    }

    int base1_data = 100;
};

// 基类2
class Base2 {
public:
    virtual void func3() {
        std::cout << "  Base2::func3(), this = " << reinterpret_cast<void*>(this) << std::endl;
    }

    virtual void func4() {
        std::cout << "  Base2::func4(), this = " << reinterpret_cast<void*>(this) << std::endl;
    }

    virtual ~Base2() {
        std::cout << "  Base2::~Base2()" << std::endl;
    }

    int base2_data = 200;
};

// 派生类（多重继承）
class Derived : public Base1, public Base2 {
public:
    void func1() override {
        std::cout << "  Derived::func1(), this = " << reinterpret_cast<void*>(this) << std::endl;
    }

    void func3() override {
        std::cout << "  Derived::func3(), this = " << reinterpret_cast<void*>(this) << std::endl;
    }

    virtual void func5() {
        std::cout << "  Derived::func5(), this = " << reinterpret_cast<void*>(this) << std::endl;
    }

    ~Derived() override {
        std::cout << "  Derived::~Derived()" << std::endl;
    }

    int derived_data = 300;
};

// 演示对象的大小和布局
void demonstrate_object_layout() {
    std::cout << "========================================" << std::endl;
    std::cout << "多重继承的对象大小与布局" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "类型大小：" << std::endl;
    std::cout << "  sizeof(Base1):   " << sizeof(Base1) << " bytes" << std::endl;
    std::cout << "  sizeof(Base2):   " << sizeof(Base2) << " bytes" << std::endl;
    std::cout << "  sizeof(Derived): " << sizeof(Derived) << " bytes" << std::endl;

    std::cout << "\n预期布局：" << std::endl;
    std::cout << "  [vptr1 | base1_data] [vptr2 | base2_data] [derived_data]" << std::endl;

    Derived d;
    Base1* b1 = &d;
    Base2* b2 = &d;

    std::cout << "\nDerived 对象的子对象地址：" << std::endl;
    std::cout << "  Derived 首地址: " << &d << std::endl;
    std::cout << "  Base1 子对象:   " << b1 << " (偏移: 0)" << std::endl;
    std::cout << "  Base2 子对象:   " << b2
              << " (偏移: " << (reinterpret_cast<char*>(b2) - reinterpret_cast<char*>(&d)) << ")" << std::endl;

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- Base1 子对象位于开头，Base1* 与 Derived* 指向同一地址" << std::endl;
    std::cout << "- Base2 子对象偏移 sizeof(Base1)，需要指针调整" << std::endl;
    std::cout << "- derived_data 在最后" << std::endl;
    std::cout << std::endl;
}

// 演示多个 vptr
void demonstrate_multiple_vptrs() {
    std::cout << "========================================" << std::endl;
    std::cout << "多重继承中的多个 vptr" << std::endl;
    std::cout << "========================================" << std::endl;

    Derived d;

    void* vptr1 = *(void**)static_cast<Base1*>(&d);
    void* vptr2 = *(void**)static_cast<Base2*>(&d);

    std::cout << "Derived 对象中的 vptr：" << std::endl;
    std::cout << "  Base1 子对象的 vptr: " << vptr1 << std::endl;
    std::cout << "  Base2 子对象的 vptr: " << vptr2 << std::endl;

    Base1 b1;
    Base2 b2;
    std::cout << "\n与基类 vtable 对比：" << std::endl;
    std::cout << "  Base1 类的 vtable: " << *(void**)&b1 << std::endl;
    std::cout << "  Base2 类的 vtable: " << *(void**)&b2 << std::endl;

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- Derived 对象有 2 个 vptr（每个基类一个）" << std::endl;
    std::cout << "- 每个 vptr 指向不同的虚函数表" << std::endl;
    std::cout << std::endl;
}

// 演示 this 指针调整
void demonstrate_this_adjustment() {
    std::cout << "========================================" << std::endl;
    std::cout << "this 指针调整" << std::endl;
    std::cout << "========================================" << std::endl;

    Derived d;

    std::cout << "各种指针的值：" << std::endl;
    std::cout << "  Derived*: " << &d << std::endl;
    std::cout << "  Base1*:   " << static_cast<Base1*>(&d) << std::endl;
    std::cout << "  Base2*:   " << static_cast<Base2*>(&d) << std::endl;

    std::cout << "\n通过不同指针调用虚函数，观察 this 的值：" << std::endl;
    std::cout << "--- 通过 Base1* 调用 func1() ---" << std::endl;
    Base1* b1 = &d;
    b1->func1();

    std::cout << "\n--- 通过 Base2* 调用 Derived::func3() ---" << std::endl;
    Base2* b2 = &d;
    b2->func3();

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 通过 Base1* 调用 func1()，this 指向 Derived 首地址（偏移 0）" << std::endl;
    std::cout << "- 通过 Base2* 调用 func3()，传入的 b2 指向 Base2 子对象" << std::endl;
    std::cout << "- 但 Derived::func3() 收到的 this 已被调整回 Derived 首地址" << std::endl;
    std::cout << "- 编译器通过 thunk 函数自动完成 this 调整（减掉偏移量）" << std::endl;
    std::cout << std::endl;
}

// 演示类型转换时的指针偏移
void demonstrate_pointer_conversion() {
    std::cout << "========================================" << std::endl;
    std::cout << "类型转换时的指针偏移" << std::endl;
    std::cout << "========================================" << std::endl;

    Derived d;

    std::cout << "正向转换（Derived* -> Base*）：" << std::endl;
    Base1* b1 = &d;
    Base2* b2 = &d;
    std::cout << "  Derived* -> Base1*: " << &d << " -> " << b1
              << " (偏移: " << (reinterpret_cast<char*>(b1) - reinterpret_cast<char*>(&d)) << ")" << std::endl;
    std::cout << "  Derived* -> Base2*: " << &d << " -> " << b2
              << " (偏移: " << (reinterpret_cast<char*>(b2) - reinterpret_cast<char*>(&d)) << ")" << std::endl;

    std::cout << "\n逆向转换（Base2* -> Derived*）：" << std::endl;
    Base2* b2_ptr = &d;
    Derived* back = static_cast<Derived*>(b2_ptr);
    std::cout << "  Base2* -> Derived*: " << b2_ptr << " -> " << back
              << " (偏移: " << (reinterpret_cast<char*>(back) - reinterpret_cast<char*>(b2_ptr)) << ")" << std::endl;

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- Derived* -> Base1*: 偏移为 0" << std::endl;
    std::cout << "- Derived* -> Base2*: 偏移为 sizeof(Base1)" << std::endl;
    std::cout << "- 编译器自动计算偏移，逆向转换同样正确" << std::endl;
    std::cout << std::endl;
}

// 演示虚析构与析构顺序
void demonstrate_destructor_order() {
    std::cout << "========================================" << std::endl;
    std::cout << "多重继承下的析构顺序" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "创建并销毁 Derived 对象：" << std::endl;
    {
        Derived d;
    }

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 析构顺序与构造顺序相反" << std::endl;
    std::cout << "- ~Derived() -> ~Base2() -> ~Base1()" << std::endl;
    std::cout << std::endl;
}

// 主函数
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "多重继承与 this 指针调整" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "编译说明：" << std::endl;
    std::cout << "g++ -g -Wall -Wextra -std=c++17 02_multiple_inheritance.cpp -o 02_multiple_inheritance" << std::endl;
    std::cout << std::endl;

    std::cout << "平台: " << PLATFORM_NAME << std::endl;
    std::cout << std::endl;

    demonstrate_object_layout();

    demonstrate_multiple_vptrs();

    demonstrate_this_adjustment();

    demonstrate_pointer_conversion();

    demonstrate_destructor_order();

    std::cout << "========================================" << std::endl;
    std::cout << "总结" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. 多重继承的内存布局：" << std::endl;
    std::cout << "   - 派生类包含所有基类的子对象" << std::endl;
    std::cout << "   - 每个基类子对象有自己的 vptr" << std::endl;
    std::cout << "   - 第二个及后续基类有非零偏移" << std::endl;

    std::cout << "\n2. 多个 vptr：" << std::endl;
    std::cout << "   - 有几个基类就有几个 vptr" << std::endl;
    std::cout << "   - 每个 vptr 指向不同的虚函数表" << std::endl;
    std::cout << "   - 各虚函数表管理对应基类的虚函数逻辑" << std::endl;

    std::cout << "\n3. this 指针调整：" << std::endl;
    std::cout << "   - 通过 Base2* 调用虚函数时，vtable 中的 thunk 函数先调整 this" << std::endl;
    std::cout << "   - thunk 将 Base2 子对象地址减偏移量得到 Derived 首地址" << std::endl;
    std::cout << "   - 确保派生类实现函数收到的 this 指向完整的派生类对象" << std::endl;

    std::cout << "\n4. 类型转换的指针偏移：" << std::endl;
    std::cout << "   - Derived* -> Base1*: 偏移为 0" << std::endl;
    std::cout << "   - Derived* -> Base2*: 偏移为 sizeof(Base1)" << std::endl;
    std::cout << "   - 转换可逆，逆向转换会正确调回" << std::endl;

    return 0;
}
