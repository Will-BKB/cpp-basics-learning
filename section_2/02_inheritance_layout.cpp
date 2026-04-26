/**
 * 教学程序：类继承的内存布局
 * 目标：深入理解继承如何影响对象的内存布局
 *
 * 核心知识点：
 * 1. 单继承的内存布局
 * 2. 多重继承的内存布局
 * 3. 虚继承的内存布局
 * 4. 虚函数表（vtable）的布局
 * 5. this 指针的调整
 * 6. 内存对齐和填充
 */

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstring>

// 辅助宏：打印内存布局
#define PRINT_LAYOUT(cls) \
    std::cout << "=== " << #cls << " 内存布局 ===" << std::endl; \
    std::cout << "大小: " << sizeof(cls) << " bytes" << std::endl; \
    std::cout << "对齐: " << alignof(cls) << " bytes" << std::endl;

// 辅助函数：打印对象的内存内容
void print_object_memory(const void* obj, size_t size, const std::string& name) {
    const uint8_t* bytes = static_cast<const uint8_t*>(obj);

    std::cout << name << " 内存内容:" << std::endl;
    std::cout << "  地址: " << obj << std::endl;

    // 按 8 字节一行打印
    for (size_t i = 0; i < size; ++i) {
        if (i % 8 == 0) {
            std::cout << "  " << std::hex << std::setw(16) << std::setfill('0')
                      << reinterpret_cast<uintptr_t>(bytes + i) << ": ";
        }
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(bytes[i]) << " ";
        if ((i + 1) % 8 == 0 || i == size - 1) {
            std::cout << std::dec << std::setfill(' ') << std::endl;
        }
    }
}

// ============================================================================
// 1. 基础类的内存布局
// ============================================================================

class BaseClass {
private:
    int base_int;
    char base_char;
    double base_double;

public:
    BaseClass() : base_int(1), base_char('A'), base_double(1.1) {}

    void print_info() {
        std::cout << "  base_int: " << base_int << " @ " << &base_int << std::endl;
        std::cout << "  base_char: " << base_char << " @ " << static_cast<void*>(&base_char) << std::endl;
        std::cout << "  base_double: " << base_double << " @ " << &base_double << std::endl;
    }
};

void test_basic_class_layout() {
    std::cout << "\n=== 1. 基础类内存布局 ===" << std::endl;

    PRINT_LAYOUT(BaseClass);

    BaseClass obj;
    obj.print_info();

    print_object_memory(&obj, sizeof(BaseClass), "BaseClass");

    std::cout << "\n布局分析：" << std::endl;
    std::cout << "- 成员按照声明顺序排列" << std::endl;
    std::cout << "- 可能有填充（padding）用于对齐" << std::endl;
    std::cout << "- sizeof(BaseClass) = " << sizeof(BaseClass) << " bytes" << std::endl;
}

// ============================================================================
// 2. 单继承的内存布局
// ============================================================================

class DerivedSingle : public BaseClass {
private:
    int derived_int;
    char derived_char;

public:
    DerivedSingle() : derived_int(2), derived_char('B') {}

    void print_info() {
        BaseClass::print_info();
        std::cout << "  derived_int: " << derived_int << " @ " << &derived_int << std::endl;
        std::cout << "  derived_char: " << derived_char << " @ " << static_cast<void*>(&derived_char) << std::endl;
    }
};

void test_single_inheritance_layout() {
    std::cout << "\n=== 2. 单继承内存布局 ===" << std::endl;

    PRINT_LAYOUT(BaseClass);
    PRINT_LAYOUT(DerivedSingle);

    DerivedSingle obj;
    obj.print_info();

    print_object_memory(&obj, sizeof(DerivedSingle), "DerivedSingle");

    std::cout << "\n布局分析：" << std::endl;
    std::cout << "- 基类成员在前，派生类成员在后" << std::endl;
    std::cout << "- 内存布局：[BaseClass 成员] [DerivedSingle 成员]" << std::endl;
    std::cout << "- sizeof(DerivedSingle) = sizeof(BaseClass) + 派生类成员大小" << std::endl;
    std::cout << "- 基类指针可以指向派生类对象（向上转型）" << std::endl;

    // 验证指针转换
    BaseClass* base_ptr = &obj;
    DerivedSingle* derived_ptr = &obj;

    std::cout << "\n指针转换验证：" << std::endl;
    std::cout << "  &obj: " << &obj << std::endl;
    std::cout << "  base_ptr: " << base_ptr << " (相同地址)" << std::endl;
    std::cout << "  derived_ptr: " << derived_ptr << " (相同地址)" << std::endl;
}

// ============================================================================
// 3. 带虚函数的单继承
// ============================================================================

class BaseWithVirtual {
private:
    int base_data;

public:
    BaseWithVirtual() : base_data(10) {}

    virtual void virtual_func() {
        std::cout << "  BaseWithVirtual::virtual_func()" << std::endl;
    }

    void non_virtual_func() {
        std::cout << "  BaseWithVirtual::non_virtual_func()" << std::endl;
    }

    int get_data() const { return base_data; }
};

class DerivedWithVirtual : public BaseWithVirtual {
private:
    int derived_data;

public:
    DerivedWithVirtual() : derived_data(20) {}

    void virtual_func() override {
        std::cout << "  DerivedWithVirtual::virtual_func()" << std::endl;
    }

    int get_derived_data() const { return derived_data; }
};

void test_virtual_function_layout() {
    std::cout << "\n=== 3. 虚函数内存布局 ===" << std::endl;

    PRINT_LAYOUT(BaseWithVirtual);
    PRINT_LAYOUT(DerivedWithVirtual);

    BaseWithVirtual base_obj;
    DerivedWithVirtual derived_obj;

    std::cout << "\n对象大小分析：" << std::endl;
    std::cout << "  BaseWithVirtual: " << sizeof(BaseWithVirtual) << " bytes" << std::endl;
    std::cout << "  DerivedWithVirtual: " << sizeof(DerivedWithVirtual) << " bytes" << std::endl;
    std::cout << "  差异: " << (sizeof(DerivedWithVirtual) - sizeof(BaseWithVirtual)) << " bytes" << std::endl;

    std::cout << "\n虚函数表（vtable）分析：" << std::endl;
    std::cout << "- 每个带虚函数的类都有一个虚函数表" << std::endl;
    std::cout << "- 对象中存储指向虚函数表的指针（vptr）" << std::endl;
    std::cout << "- vptr 通常在对象的起始位置" << std::endl;
    std::cout << "- 虚函数表包含虚函数的地址" << std::endl;

    std::cout << "\n多态调用验证：" << std::endl;
    BaseWithVirtual* poly_ptr = &derived_obj;
    std::cout << "  通过基类指针调用虚函数：" << std::endl;
    poly_ptr->virtual_func();  // 调用 DerivedWithVirtual::virtual_func()

    std::cout << "  通过基类指针调用非虚函数：" << std::endl;
    poly_ptr->non_virtual_func();  // 调用 BaseWithVirtual::non_virtual_func()

    // 查看虚函数表指针
    std::cout << "\n虚函数表指针位置：" << std::endl;
    print_object_memory(&derived_obj, sizeof(DerivedWithVirtual), "DerivedWithVirtual");
}

// ============================================================================
// 4. 多重继承的内存布局
// ============================================================================

class Base1 {
private:
    int base1_data;

public:
    Base1() : base1_data(100) {}
    void func1() { std::cout << "  Base1::func1()" << std::endl; }
    int get_data() const { return base1_data; }
};

class Base2 {
private:
    int base2_data;

public:
    Base2() : base2_data(200) {}
    void func2() { std::cout << "  Base2::func2()" << std::endl; }
    int get_data() const { return base2_data; }
};

class MultipleDerived : public Base1, public Base2 {
private:
    int derived_data;

public:
    MultipleDerived() : derived_data(300) {}
    void derived_func() { std::cout << "  MultipleDerived::derived_func()" << std::endl; }
    int get_derived_data() const { return derived_data; }
};

void test_multiple_inheritance_layout() {
    std::cout << "\n=== 4. 多重继承内存布局 ===" << std::endl;

    PRINT_LAYOUT(Base1);
    PRINT_LAYOUT(Base2);
    PRINT_LAYOUT(MultipleDerived);

    MultipleDerived obj;

    std::cout << "\n布局分析：" << std::endl;
    std::cout << "- 内存布局：[Base1] [Base2] [MultipleDerived]" << std::endl;
    std::cout << "- sizeof(MultipleDerived) = sizeof(Base1) + sizeof(Base2) + 派生类成员" << std::endl;
    std::cout << "- sizeof(MultipleDerived) = " << sizeof(MultipleDerived) << " bytes" << std::endl;

    print_object_memory(&obj, sizeof(MultipleDerived), "MultipleDerived");

    std::cout << "\n指针转换和 this 调整：" << std::endl;
    MultipleDerived* derived_ptr = &obj;
    Base1* base1_ptr = &obj;
    Base2* base2_ptr = &obj;

    std::cout << "  &obj: " << &obj << std::endl;
    std::cout << "  derived_ptr: " << derived_ptr << std::endl;
    std::cout << "  base1_ptr: " << base1_ptr << " (与 derived_ptr 相同)" << std::endl;
    std::cout << "  base2_ptr: " << base2_ptr << " (需要 this 调整)" << std::endl;

    std::cout << "  base2_ptr - derived_ptr = " << (reinterpret_cast<char*>(base2_ptr) - reinterpret_cast<char*>(derived_ptr)) << " bytes" << std::endl;

    std::cout << "\nthis 指针调整说明：" << std::endl;
    std::cout << "- Base1 是第一个基类，地址相同" << std::endl;
    std::cout << "- Base2 是第二个基类，地址偏移 sizeof(Base1)" << std::endl;
    std::cout << "- 编译器在调用 Base2 方法时自动调整 this 指针" << std::endl;
}

// ============================================================================
// 5. 虚继承的内存布局
// ============================================================================

class VirtualBase {
private:
    int virtual_base_data;

public:
    VirtualBase() : virtual_base_data(1000) {}
    void virtual_base_func() { std::cout << "  VirtualBase::virtual_base_func()" << std::endl; }
    int get_data() const { return virtual_base_data; }
};

class VirtualDerived1 : public virtual VirtualBase {
private:
    int derived1_data;

public:
    VirtualDerived1() : derived1_data(2000) {}
    void derived1_func() { std::cout << "  VirtualDerived1::derived1_func()" << std::endl; }
    int get_derived1_data() const { return derived1_data; }
};

class VirtualDerived2 : public virtual VirtualBase {
private:
    int derived2_data;

public:
    VirtualDerived2() : derived2_data(3000) {}
    void derived2_func() { std::cout << "  VirtualDerived2::derived2_func()" << std::endl; }
    int get_derived2_data() const { return derived2_data; }
};

class DiamondDerived : public VirtualDerived1, public VirtualDerived2 {
private:
    int diamond_data;

public:
    DiamondDerived() : diamond_data(4000) {}
    void diamond_func() { std::cout << "  DiamondDerived::diamond_func()" << std::endl; }
    int get_diamond_data() const { return diamond_data; }
};

void test_virtual_inheritance_layout() {
    std::cout << "\n=== 5. 虚继承内存布局 ===" << std::endl;

    PRINT_LAYOUT(VirtualBase);
    PRINT_LAYOUT(VirtualDerived1);
    PRINT_LAYOUT(VirtualDerived2);
    PRINT_LAYOUT(DiamondDerived);

    DiamondDerived obj;

    std::cout << "\n布局分析：" << std::endl;
    std::cout << "- 虚基类只存在一份，避免重复" << std::endl;
    std::cout << "- 使用虚基类表（vbtable）来定位虚基类" << std::endl;
    std::cout << "- 每个虚继承的类都有虚基类指针" << std::endl;

    print_object_memory(&obj, sizeof(DiamondDerived), "DiamondDerived");

    std::cout << "\n菱形继承问题解决：" << std::endl;
    std::cout << "- VirtualBase 只构造一次" << std::endl;
    std::cout << "- VirtualDerived1 和 VirtualDerived2 共享同一个 VirtualBase" << std::endl;
    std::cout << "- 避免了数据冗余和二义性" << std::endl;

    std::cout << "\n虚基类访问：" << std::endl;
    std::cout << "  通过 VirtualDerived1 访问: " << obj.get_data() << std::endl;
    std::cout << "  通过 VirtualDerived2 访问: " << obj.get_data() << std::endl;
    std::cout << "  (两者访问同一个 VirtualBase)" << std::endl;
}

// ============================================================================
// 6. 虚函数 + 多重继承
// ============================================================================

class VirtualBase1 {
private:
    int data1;

public:
    VirtualBase1() : data1(1) {}
    virtual void func1() { std::cout << "  VirtualBase1::func1()" << std::endl; }
};

class VirtualBase2 {
private:
    int data2;

public:
    VirtualBase2() : data2(2) {}
    virtual void func2() { std::cout << "  VirtualBase2::func2()" << std::endl; }
};

class VirtualMultiDerived : public VirtualBase1, public VirtualBase2 {
private:
    int derived_data;

public:
    VirtualMultiDerived() : derived_data(3) {}
    void func1() override { std::cout << "  VirtualMultiDerived::func1()" << std::endl; }
    void func2() override { std::cout << "  VirtualMultiDerived::func2()" << std::endl; }
};

void test_virtual_multiple_inheritance() {
    std::cout << "\n=== 6. 虚函数 + 多重继承 ===" << std::endl;

    PRINT_LAYOUT(VirtualBase1);
    PRINT_LAYOUT(VirtualBase2);
    PRINT_LAYOUT(VirtualMultiDerived);

    VirtualMultiDerived obj;

    std::cout << "\n布局分析：" << std::endl;
    std::cout << "- 每个带虚函数的基类都有自己的 vptr" << std::endl;
    std::cout << "- VirtualMultiDerived 有多个 vptr" << std::endl;
    std::cout << "- sizeof(VirtualMultiDerived) = " << sizeof(VirtualMultiDerived) << " bytes" << std::endl;

    print_object_memory(&obj, sizeof(VirtualMultiDerived), "VirtualMultiDerived");

    std::cout << "\n多态调用验证：" << std::endl;
    VirtualBase1* base1_ptr = &obj;
    VirtualBase2* base2_ptr = &obj;

    std::cout << "  base1_ptr: " << base1_ptr << std::endl;
    std::cout << "  base2_ptr: " << base2_ptr << std::endl;

    std::cout << "  通过 base1_ptr 调用: ";
    base1_ptr->func1();

    std::cout << "  通过 base2_ptr 调用: ";
    base2_ptr->func2();
}

// ============================================================================
// 7. 内存对齐和填充
// ============================================================================

class PaddedClass {
    char c1;    // 1 byte + 7 bytes padding
    double d;    // 8 bytes
    char c2;    // 1 byte + 7 bytes padding
    int i;       // 4 bytes + 4 bytes padding
};

class OptimizedClass {
    double d;    // 8 bytes
    int i;       // 4 bytes
    char c1;     // 1 byte
    char c2;     // 1 byte
    // 2 bytes padding
};

void test_memory_alignment() {
    std::cout << "\n=== 7. 内存对齐和填充 ===" << std::endl;

    PRINT_LAYOUT(PaddedClass);
    PRINT_LAYOUT(OptimizedClass);

    PaddedClass padded;
    OptimizedClass optimized;

    std::cout << "\n布局对比：" << std::endl;
    std::cout << "  PaddedClass: " << sizeof(PaddedClass) << " bytes" << std::endl;
    std::cout << "  OptimizedClass: " << sizeof(OptimizedClass) << " bytes" << std::endl;
    std::cout << "  节省: " << (sizeof(PaddedClass) - sizeof(OptimizedClass)) << " bytes" << std::endl;

    std::cout << "\n对齐规则：" << std::endl;
    std::cout << "- 每个成员都有自己的对齐要求" << std::endl;
    std::cout << "- char: 1 byte 对齐" << std::endl;
    std::cout << "- int: 4 bytes 对齐" << std::endl;
    std::cout << "- double: 8 bytes 对齐" << std::endl;
    std::cout << "- 编译器会插入填充以满足对齐要求" << std::endl;

    std::cout << "\n优化建议：" << std::endl;
    std::cout << "- 按照对齐要求从大到小排列成员" << std::endl;
    std::cout << "- 减少填充，节省内存" << std::endl;
    std::cout << "- 提高缓存命中率" << std::endl;
}

// ============================================================================
// 8. 空类和空基类优化
// ============================================================================

class EmptyClass {};

class EmptyBase {};

class DerivedFromEmpty : public EmptyBase {
    int data;
};

void test_empty_class_optimization() {
    std::cout << "\n=== 8. 空类和空基类优化 ===" << std::endl;

    PRINT_LAYOUT(EmptyClass);
    PRINT_LAYOUT(EmptyBase);
    PRINT_LAYOUT(DerivedFromEmpty);

    std::cout << "\n空类优化（EBO - Empty Base Optimization）：" << std::endl;
    std::cout << "- 空类大小至少为 1 byte（保证不同对象地址不同）" << std::endl;
    std::cout << "- 空基类优化：空基类不占用派生类空间" << std::endl;
    std::cout << "- sizeof(DerivedFromEmpty) = sizeof(int) = " << sizeof(DerivedFromEmpty) << " bytes" << std::endl;

    std::cout << "\n应用场景：" << std::endl;
    std::cout << "- 使用空基类提供类型标签" << std::endl;
    std::cout << "- 实现策略模式（Strategy Pattern）" << std::endl;
    std::cout << "- 混入（Mixin）模式" << std::endl;
}

// ============================================================================
// 主函数
// ============================================================================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "类继承的内存布局" << std::endl;
    std::cout << "========================================" << std::endl;

    test_basic_class_layout();
    test_single_inheritance_layout();
    test_virtual_function_layout();
    test_multiple_inheritance_layout();
    test_virtual_inheritance_layout();
    test_virtual_multiple_inheritance();
    test_memory_alignment();
    test_empty_class_optimization();

    std::cout << "\n========================================" << std::endl;
    std::cout << "总结" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. 单继承：基类成员在前，派生类成员在后" << std::endl;
    std::cout << "2. 虚函数：增加 vptr，支持多态" << std::endl;
    std::cout << "3. 多重继承：按声明顺序排列基类" << std::endl;
    std::cout << "4. 虚继承：共享虚基类，使用 vbtable" << std::endl;
    std::cout << "5. this 指针：多重继承时需要调整" << std::endl;
    std::cout << "6. 内存对齐：影响布局和大小" << std::endl;
    std::cout << "7. 空基类优化：EBO 减少空间占用" << std::endl;

    std::cout << "\n查看内存布局的方法：" << std::endl;
    std::cout << "1. 使用 sizeof 和 alignof" << std::endl;
    std::cout << "2. 打印对象的内存内容" << std::endl;
    std::cout << "3. 使用反汇编查看编译器生成的代码" << std::endl;
    std::cout << "4. 使用 GDB 查看对象的内存布局" << std::endl;

    std::cout << "\n反汇编命令：" << std::endl;
    std::cout << "g++ -g -O0 -S 02_inheritance_layout.cpp -o 02_inheritance_layout.s" << std::endl;
    std::cout << "cat 02_inheritance_layout.s | grep -A 20 'test_multiple_inheritance'" << std::endl;

    return 0;
}