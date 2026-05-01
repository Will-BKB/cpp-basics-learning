/**
 * 教学程序：虚函数表（vtable）
 * 目标：理解虚函数表的工作原理和内存布局
 *
 * 核心知识点：
 * 1. 虚函数表的基本结构
 * 2. vptr 的位置和作用
 * 3. 单继承下的虚函数覆盖
 * 4. 虚析构函数的作用
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

// 基类
class Base {
public:
    virtual void func1() {
        std::cout << "Base::func1()" << std::endl;
    }

    virtual void func2() {
        std::cout << "Base::func2()" << std::endl;
    }

    virtual ~Base() {
        std::cout << "Base::~Base()" << std::endl;
    }

    int base_data = 100;
};

// 派生类
class Derived : public Base {
public:
    void func1() override {  // 覆盖基类的虚函数
        std::cout << "Derived::func1()" << std::endl;
    }

    void func3() {  // 新增的虚函数
        std::cout << "Derived::func3()" << std::endl;
    }

    ~Derived() override {
        std::cout << "Derived::~Derived()" << std::endl;
    }

    int derived_data = 200;
};

// 演示虚函数表的基本结构
void demonstrate_vtable_structure() {
    std::cout << "========================================" << std::endl;
    std::cout << "虚函数表的基本结构" << std::endl;
    std::cout << "========================================" << std::endl;

    Base base;
    Derived derived;

    std::cout << "Base 对象地址: " << &base << std::endl;
    std::cout << "Derived 对象地址: " << &derived << std::endl;

    // 获取 vptr（虚函数表指针）
    // 注意：这是平台相关的，不同平台 vptr 的位置可能不同
    void* base_vptr = *(void**)&base;
    void* derived_vptr = *(void**)&derived;

    std::cout << "Base vptr: " << base_vptr << std::endl;
    std::cout << "Derived vptr: " << derived_vptr << std::endl;

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 每个对象都有一个 vptr（虚函数表指针）" << std::endl;
    std::cout << "- vptr 指向该类的虚函数表" << std::endl;
    std::cout << "- 同一类型的对象共享同一个虚函数表" << std::endl;
    std::cout << std::endl;
}

// 演示虚函数表的内容
void demonstrate_vtable_content() {
    std::cout << "========================================" << std::endl;
    std::cout << "虚函数表的内容" << std::endl;
    std::cout << "========================================" << std::endl;

    Base base;
    Derived derived;

    std::cout << "调用 Base 的虚函数：" << std::endl;
    base.func1();
    base.func2();

    std::cout << "\n调用 Derived 的虚函数：" << std::endl;
    derived.func1();
    derived.func2();
    derived.func3();

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- Base 的虚函数表包含 Base::func1, Base::func2" << std::endl;
    std::cout << "- Derived 的虚函数表包含 Derived::func1, Base::func2, Derived::func3" << std::endl;
    std::cout << "- 虚函数调用通过 vtable 查找函数地址" << std::endl;
    std::cout << std::endl;
}

// 演示单继承下的虚函数覆盖
void demonstrate_virtual_override() {
    std::cout << "========================================" << std::endl;
    std::cout << "单继承下的虚函数覆盖" << std::endl;
    std::cout << "========================================" << std::endl;

    Base* base_ptr = new Derived();
    base_ptr->func1();  // 调用 Derived::func1()
    base_ptr->func2();  // 调用 Base::func2()

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 基类指针指向派生类对象" << std::endl;
    std::cout << "- func1() 被派生类覆盖，调用 Derived::func1()" << std::endl;
    std::cout << "- func2() 未被覆盖，调用 Base::func2()" << std::endl;
    std::cout << "- 这就是多态的基本原理" << std::endl;

    delete base_ptr;
    std::cout << std::endl;
}

// 演示虚析构函数的作用
void demonstrate_virtual_destructor() {
    std::cout << "========================================" << std::endl;
    std::cout << "虚析构函数的作用" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "使用虚析构函数：" << std::endl;
    Base* ptr = new Derived();
    delete ptr;  // 会调用 Derived::~Derived() 和 Base::~Base()

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 基类析构函数是虚函数" << std::endl;
    std::cout << "- delete 基类指针时，会正确调用派生类的析构函数" << std::endl;
    std::cout << "- 如果基类析构函数不是虚函数，只会调用基类的析构函数" << std::endl;
    std::cout << "- 这会导致派生类的资源没有被正确释放" << std::endl;
    std::cout << std::endl;
}

// 演示对象的内存布局
void demonstrate_object_layout() {
    std::cout << "========================================" << std::endl;
    std::cout << "对象的内存布局" << std::endl;
    std::cout << "========================================" << std::endl;

    Base base;
    Derived derived;

    std::cout << "Base 对象大小: " << sizeof(Base) << " bytes" << std::endl;
    std::cout << "Derived 对象大小: " << sizeof(Derived) << " bytes" << std::endl;

    std::cout << "\nBase 对象布局：" << std::endl;
    std::cout << "  vptr: " << sizeof(void*) << " bytes" << std::endl;
    std::cout << "  base_data: " << sizeof(int) << " bytes" << std::endl;

    std::cout << "\nDerived 对象布局：" << std::endl;
    std::cout << "  vptr: " << sizeof(void*) << " bytes" << std::endl;
    std::cout << "  base_data: " << sizeof(int) << " bytes" << std::endl;
    std::cout << "  derived_data: " << sizeof(int) << " bytes" << std::endl;

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- vptr 通常位于对象的首地址" << std::endl;
    std::cout << "- 派生类对象包含基类的所有成员" << std::endl;
    std::cout << "- 派生类对象还包含自己的成员" << std::endl;
    std::cout << std::endl;
}

// 演示同一类型共享虚函数表
void demonstrate_shared_vtable() {
    std::cout << "========================================" << std::endl;
    std::cout << "同一类型共享虚函数表" << std::endl;
    std::cout << "========================================" << std::endl;

    Base base1, base2, base3;
    Derived derived1, derived2;

    void* base1_vptr = *(void**)&base1;
    void* base2_vptr = *(void**)&base2;
    void* base3_vptr = *(void**)&base3;

    void* derived1_vptr = *(void**)&derived1;
    void* derived2_vptr = *(void**)&derived2;

    std::cout << "Base 对象的 vptr：" << std::endl;
    std::cout << "  base1: " << base1_vptr << std::endl;
    std::cout << "  base2: " << base2_vptr << std::endl;
    std::cout << "  base3: " << base3_vptr << std::endl;

    std::cout << "\nDerived 对象的 vptr：" << std::endl;
    std::cout << "  derived1: " << derived1_vptr << std::endl;
    std::cout << "  derived2: " << derived2_vptr << std::endl;

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 同一类型的所有对象共享同一个虚函数表" << std::endl;
    std::cout << "- base1, base2, base3 的 vptr 相同" << std::endl;
    std::cout << "- derived1, derived2 的 vptr 相同" << std::endl;
    std::cout << "- 不同类型的对象有不同的虚函数表" << std::endl;
    std::cout << std::endl;
}

// 主函数
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "虚函数表（vtable）" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "编译说明：" << std::endl;
    std::cout << "g++ -g -Wall -Wextra -std=c++17 01_vtable.cpp -o 01_vtable" << std::endl;
    std::cout << std::endl;

    std::cout << "查看虚函数表：" << std::endl;
    std::cout << "nm 01_vtable | grep vtable" << std::endl;
    std::cout << std::endl;

    std::cout << "平台: " << PLATFORM_NAME << std::endl;
    std::cout << std::endl;

    // 演示虚函数表的基本结构
    demonstrate_vtable_structure();

    // 演示虚函数表的内容
    demonstrate_vtable_content();

    // 演示单继承下的虚函数覆盖
    demonstrate_virtual_override();

    // 演示虚析构函数的作用
    demonstrate_virtual_destructor();

    // 演示对象的内存布局
    demonstrate_object_layout();

    // 演示同一类型共享虚函数表
    demonstrate_shared_vtable();

    std::cout << "========================================" << std::endl;
    std::cout << "总结" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. 虚函数表的基本结构：" << std::endl;
    std::cout << "   - 每个有虚函数的类都有一个虚函数表" << std::endl;
    std::cout << "   - 每个对象都有一个 vptr 指向虚函数表" << std::endl;
    std::cout << "   - vptr 通常位于对象的首地址" << std::endl;

    std::cout << "\n2. 虚函数表的内容：" << std::endl;
    std::cout << "   - 包含该类所有虚函数的地址" << std::endl;
    std::cout << "   - 派生类覆盖虚函数时，修改对应的表项" << std::endl;
    std::cout << "   - 派生类新增虚函数时，添加新的表项" << std::endl;

    std::cout << "\n3. 虚函数调用的过程：" << std::endl;
    std::cout << "   - 通过 vptr 找到虚函数表" << std::endl;
    std::cout << "   - 在虚函数表中查找函数地址" << std::endl;
    std::cout << "   - 调用对应的函数" << std::endl;

    std::cout << "\n4. 虚析构函数的作用：" << std::endl;
    std::cout << "   - 保证 delete 基类指针时正确调用派生类析构函数" << std::endl;
    std::cout << "   - 防止资源泄漏" << std::endl;
    std::cout << "   - 基类析构函数应该声明为虚函数" << std::endl;

    std::cout << "\n5. 同一类型共享虚函数表：" << std::endl;
    std::cout << "   - 同一类型的所有对象共享同一个虚函数表" << std::endl;
    std::cout << "   - 虚函数表是类级别的，不是对象级别的" << std::endl;
    std::cout << "   - 节省内存空间" << std::endl;

    std::cout << "\n6. 对象的内存布局：" << std::endl;
    std::cout << "   - vptr 通常位于对象的首地址" << std::endl;
    std::cout << "   - 派生类对象包含基类的所有成员" << std::endl;
    std::cout << "   - 派生类对象还包含自己的成员" << std::endl;

    std::cout << "\n7. 跨平台注意事项：" << std::endl;
    std::cout << "   - 不同平台的 vptr 位置可能不同" << std::endl;
    std::cout << "   - 不同平台的虚函数表布局可能不同" << std::endl;
    std::cout << "   - 但基本原理相同" << std::endl;

    return 0;
}
