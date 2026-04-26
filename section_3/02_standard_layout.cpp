/**
 * 教学程序：Standard Layout 类型
 * 目标：深入理解什么是 standard layout 类型及其重要性
 *
 * 核心知识点：
 * 1. Standard Layout 的定义
 * 2. std::is_standard_layout_v 的使用
 * 3. 什么样的类型是 standard layout
 * 4. 什么样的类型不是 standard layout
 * 5. reinterpret_cast 到首成员的安全性
 * 6. 与 C 语言的兼容性
 * 7. 内存布局保证
 */

#include <iostream>
#include <type_traits>
#include <cstring>
#include <string>
#include <vector>

// 辅助宏：打印类型特征
#define PRINT_TYPE_TRAIT(type, trait) \
    std::cout << #type << ": " << #trait << " = " << (trait<type>::value ? "true" : "false") << std::endl;

// 辅助宏：打印类型信息
#define PRINT_TYPE_INFO(type) \
    std::cout << "\n=== " << #type << " ===" << std::endl; \
    PRINT_TYPE_TRAIT(type, std::is_standard_layout); \
    PRINT_TYPE_TRAIT(type, std::is_trivial); \
    PRINT_TYPE_TRAIT(type, std::is_trivially_copyable);

// ============================================================================
// 1. 基本类型的 Standard Layout 检查
// ============================================================================

void test_basic_types() {
    std::cout << "\n=== 1. 基本类型的 Standard Layout 检查 ===" << std::endl;

    PRINT_TYPE_INFO(int)
    PRINT_TYPE_INFO(double)
    PRINT_TYPE_INFO(char)
    PRINT_TYPE_INFO(bool)
    PRINT_TYPE_INFO(int*)
    PRINT_TYPE_INFO(const int*)
    PRINT_TYPE_INFO(int[10])

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 所有基本类型都是 standard layout" << std::endl;
    std::cout << "- 指针类型也是 standard layout" << std::endl;
    std::cout << "- 数组类型如果元素是 standard layout，则数组也是" << std::endl;
}

// ============================================================================
// 2. 简单类的 Standard Layout 检查
// ============================================================================

class StandardLayoutClass {
public:
    int x;
    double y;
    char z;
};

class NonStandardLayoutClass {
private:
    int x;  // 私有成员
public:
    double y;
    char z;
};

class MixedAccessClass {
public:
    int x;
private:
    double y;
public:
    char z;
};

void test_simple_classes() {
    std::cout << "\n=== 2. 简单类的 Standard Layout 检查 ===" << std::endl;

    PRINT_TYPE_INFO(StandardLayoutClass)
    PRINT_TYPE_INFO(NonStandardLayoutClass)
    PRINT_TYPE_INFO(MixedAccessClass)

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- StandardLayoutClass: 所有成员都是 public，是 standard layout" << std::endl;
    std::cout << "- NonStandardLayoutClass: 有私有成员，不是 standard layout" << std::endl;
    std::cout << "- MixedAccessClass: 混合访问控制，不是 standard layout" << std::endl;
}

// ============================================================================
// 3. 虚函数对 Standard Layout 的影响
// ============================================================================

class ClassWithVirtualFunction {
public:
    int x;

    virtual void func() {
        std::cout << "Virtual function" << std::endl;
    }

    virtual ~ClassWithVirtualFunction() {}
};

class ClassWithoutVirtualFunction {
public:
    int x;

    void func() {
        std::cout << "Non-virtual function" << std::endl;
    }
};

void test_virtual_functions() {
    std::cout << "\n=== 3. 虚函数对 Standard Layout 的影响 ===" << std::endl;

    PRINT_TYPE_INFO(ClassWithVirtualFunction)
    PRINT_TYPE_INFO(ClassWithoutVirtualFunction)

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- ClassWithVirtualFunction: 有虚函数，不是 standard layout" << std::endl;
    std::cout << "- ClassWithoutVirtualFunction: 无虚函数，是 standard layout" << std::endl;
    std::cout << "- 虚函数需要 vtable，破坏了标准布局" << std::endl;
}

// ============================================================================
// 4. 继承对 Standard Layout 的影响
// ============================================================================

class BaseClass {
public:
    int x;
};

class DerivedFromStandardLayout : public BaseClass {
public:
    int y;
};

class BaseWithVirtual {
public:
    int x;
    virtual void func() {}
};

class DerivedFromNonStandardLayout : public BaseWithVirtual {
public:
    int y;
};

class BaseClass1 {
public:
    int x;
};

class BaseClass2 {
public:
    int y;
};

class MultipleInheritance : public BaseClass1, public BaseClass2 {
public:
    int z;
};

void test_inheritance() {
    std::cout << "\n=== 4. 继承对 Standard Layout 的影响 ===" << std::endl;

    PRINT_TYPE_INFO(BaseClass)
    PRINT_TYPE_INFO(DerivedFromStandardLayout)
    PRINT_TYPE_INFO(BaseWithVirtual)
    PRINT_TYPE_INFO(DerivedFromNonStandardLayout)
    PRINT_TYPE_INFO(MultipleInheritance)

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- BaseClass: 是 standard layout" << std::endl;
    std::cout << "- DerivedFromStandardLayout: 单继承自 standard layout 基类，也是 standard layout" << std::endl;
    std::cout << "- BaseWithVirtual: 有虚函数，不是 standard layout" << std::endl;
    std::cout << "- DerivedFromNonStandardLayout: 继承自非 standard layout 基类，也不是" << std::endl;
    std::cout << "- MultipleInheritance: 多重继承，不是 standard layout" << std::endl;
}

// ============================================================================
// 5. 成员变量对 Standard Layout 的影响
// ============================================================================

class ClassWithStandardLayoutMembers {
public:
    int x;
    double y;
    char z;
};

class ClassWithNonStandardLayoutMember {
public:
    ClassWithVirtualFunction obj;  // 非 standard layout 成员
};

class ClassWithReferenceMember {
public:
    int& ref;  // 引用成员
};

class ClassWithStaticMember {
public:
    int x;
    static int static_var;  // 静态成员不影响 standard layout
};

void test_member_variables() {
    std::cout << "\n=== 5. 成员变量对 Standard Layout 的影响 ===" << std::endl;

    PRINT_TYPE_INFO(ClassWithStandardLayoutMembers)
    PRINT_TYPE_INFO(ClassWithNonStandardLayoutMember)
    PRINT_TYPE_INFO(ClassWithReferenceMember)
    PRINT_TYPE_INFO(ClassWithStaticMember)

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- ClassWithStandardLayoutMembers: 所有成员都是 standard layout，类也是" << std::endl;
    std::cout << "- ClassWithNonStandardLayoutMember: 有非 standard layout 成员，类也不是" << std::endl;
    std::cout << "- ClassWithReferenceMember: 有引用成员，不是 standard layout" << std::endl;
    std::cout << "- ClassWithStaticMember: 静态成员不影响 standard layout" << std::endl;
}

// ============================================================================
// 6. reinterpret_cast 到首成员的安全性
// ============================================================================

struct SafeStruct {
    int x;
    double y;
    char z;
};

struct UnsafeStruct {
private:
    int x;
public:
    double y;
    char z;
};

void test_reinterpret_cast_safety() {
    std::cout << "\n=== 6. reinterpret_cast 到首成员的安全性 ===" << std::endl;

    // Standard layout 类型可以安全地 reinterpret_cast 到首成员
    SafeStruct safe;
    safe.x = 42;
    safe.y = 3.14;
    safe.z = 'A';

    int* safe_ptr = reinterpret_cast<int*>(&safe);
    std::cout << "SafeStruct (standard layout):" << std::endl;
    std::cout << "  对象地址: " << &safe << std::endl;
    std::cout << "  首成员地址: " << &safe.x << std::endl;
    std::cout << "  reinterpret_cast 结果: " << safe_ptr << std::endl;
    std::cout << "  *safe_ptr = " << *safe_ptr << std::endl;
    std::cout << "  安全：✓" << std::endl;

    // 非 standard layout 类型这样做是危险的
    UnsafeStruct unsafe;
    unsafe.y = 2.71;
    unsafe.z = 'B';

    std::cout << "\nUnsafeStruct (非 standard layout):" << std::endl;
    std::cout << "  对象地址: " << &unsafe << std::endl;
    std::cout << "  首成员地址: " << &unsafe.y << std::endl;
    std::cout << "  危险：✗ (不能保证对象地址等于首成员地址)" << std::endl;

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- Standard layout 类型保证对象地址等于首成员地址" << std::endl;
    std::cout << "- 非 standard layout 类型不保证这一点" << std::endl;
    std::cout << "- 这对与 C 代码交互很重要" << std::endl;
}

// ============================================================================
// 7. 与 C 语言的兼容性
// ============================================================================

extern "C" {
    // C 风格的结构体
    struct CStyleStruct {
        int x;
        double y;
        char z;
    };

    // C 风格的函数
    void c_function(const CStyleStruct* s) {
        std::cout << "C function received: x=" << s->x
                  << ", y=" << s->y << ", z=" << s->z << std::endl;
    }
}

struct CompatibleStruct {
    int x;
    double y;
    char z;
};

void test_c_compatibility() {
    std::cout << "\n=== 7. 与 C 语言的兼容性 ===" << std::endl;

    PRINT_TYPE_INFO(CStyleStruct)
    PRINT_TYPE_INFO(CompatibleStruct)

    // C++ 结构体可以传递给 C 函数
    CompatibleStruct cpp_struct;
    cpp_struct.x = 100;
    cpp_struct.y = 1.618;
    cpp_struct.z = 'C';

    std::cout << "C++ struct: x=" << cpp_struct.x
              << ", y=" << cpp_struct.y << ", z=" << cpp_struct.z << std::endl;

    // 安全地传递给 C 函数
    c_function(reinterpret_cast<const CStyleStruct*>(&cpp_struct));

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- Standard layout 类型与 C 结构体兼容" << std::endl;
    std::cout << "- 可以安全地在 C 和 C++ 之间传递" << std::endl;
    std::cout << "- 内存布局相同，可以 reinterpret_cast" << std::endl;
}

// ============================================================================
// 8. 内存布局保证
// ============================================================================

struct LayoutTest1 {
    char a;
    int b;
    char c;
};

struct LayoutTest2 {
    char a;
    char c;
    int b;
};

void test_memory_layout() {
    std::cout << "\n=== 8. 内存布局保证 ===" << std::endl;

    PRINT_TYPE_INFO(LayoutTest1)
    PRINT_TYPE_INFO(LayoutTest2)

    std::cout << "\n内存布局分析：" << std::endl;
    std::cout << "LayoutTest1:" << std::endl;
    std::cout << "  sizeof: " << sizeof(LayoutTest1) << std::endl;
    std::cout << "  offsetof(a): " << offsetof(LayoutTest1, a) << std::endl;
    std::cout << "  offsetof(b): " << offsetof(LayoutTest1, b) << std::endl;
    std::cout << "  offsetof(c): " << offsetof(LayoutTest1, c) << std::endl;

    std::cout << "\nLayoutTest2:" << std::endl;
    std::cout << "  sizeof: " << sizeof(LayoutTest2) << std::endl;
    std::cout << "  offsetof(a): " << offsetof(LayoutTest2, a) << std::endl;
    std::cout << "  offsetof(c): " << offsetof(LayoutTest2, c) << std::endl;
    std::cout << "  offsetof(b): " << offsetof(LayoutTest2, b) << std::endl;

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- Standard layout 类型保证成员按声明顺序排列" << std::endl;
    std::cout << "- 但不保证没有填充（padding）" << std::endl;
    std::cout << "- 成员顺序影响内存占用（对齐）" << std::endl;
    std::cout << "- LayoutTest2 比 LayoutTest1 更紧凑" << std::endl;
}

// ============================================================================
// 9. POD (Plain Old Data) 类型
// ============================================================================

void test_pod_types() {
    std::cout << "\n=== 9. POD (Plain Old Data) 类型 ===" << std::endl;

    std::cout << "POD 类型定义：" << std::endl;
    std::cout << "- 既是 trivial 又是 standard layout" << std::endl;
    std::cout << "- C++11 后 POD 被拆分为这两个概念" << std::endl;

    std::cout << "\n检查 POD 类型：" << std::endl;
    std::cout << "int: trivial=" << std::is_trivial<int>::value
              << ", standard_layout=" << std::is_standard_layout<int>::value
              << ", pod=" << std::is_pod<int>::value << std::endl;

    std::cout << "StandardLayoutClass: trivial=" << std::is_trivial<StandardLayoutClass>::value
              << ", standard_layout=" << std::is_standard_layout<StandardLayoutClass>::value
              << ", pod=" << std::is_pod<StandardLayoutClass>::value << std::endl;

    std::cout << "SafeStruct: trivial=" << std::is_trivial<SafeStruct>::value
              << ", standard_layout=" << std::is_standard_layout<SafeStruct>::value
              << ", pod=" << std::is_pod<SafeStruct>::value << std::endl;

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- POD = trivial + standard layout" << std::endl;
    std::cout << "- C++17 后 std::is_pod 被标记为废弃" << std::endl;
    std::cout << "- 推荐分别检查 trivial 和 standard layout" << std::endl;
}

// ============================================================================
// 10. 实际应用场景
// ============================================================================

void test_practical_applications() {
    std::cout << "\n=== 10. 实际应用场景 ===" << std::endl;

    std::cout << "场景1：与 C 库交互" << std::endl;
    std::cout << "- Standard layout 类型可以安全地传递给 C 函数" << std::endl;
    std::cout << "- 内存布局与 C 结构体兼容" << std::endl;

    std::cout << "\n场景2：二进制文件格式" << std::endl;
    std::cout << "- Standard layout 类型可以安全地序列化" << std::endl;
    std::cout << "- 成员顺序和偏移量可预测" << std::endl;

    std::cout << "\n场景3：网络协议" << std::endl;
    std::cout << "- Standard layout 类型适合定义网络数据包" << std::endl;
    std::cout << "- 可以直接发送和接收" << std::endl;

    std::cout << "\n场景4：共享内存" << std::endl;
    std::cout << "- Standard layout 类型适合共享内存通信" << std::endl;
    std::cout << "- 不同进程可以安全地访问" << std::endl;
}

// ============================================================================
// 主函数
// ============================================================================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Standard Layout 类型" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "编译说明：" << std::endl;
    std::cout << "g++ -g -Wall -Wextra -std=c++17 02_standard_layout.cpp -o 02_standard_layout" << std::endl;

    test_basic_types();
    test_simple_classes();
    test_virtual_functions();
    test_inheritance();
    test_member_variables();
    test_reinterpret_cast_safety();
    test_c_compatibility();
    test_memory_layout();
    test_pod_types();
    test_practical_applications();

    std::cout << "\n========================================" << std::endl;
    std::cout << "总结" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. Standard Layout 定义：" << std::endl;
    std::cout << "   - 所有非静态成员都有相同的访问控制" << std::endl;
    std::cout << "   - 没有虚函数或虚基类" << std::endl;
    std::cout << "   - 所有基类都是 standard layout" << std::endl;
    std::cout << "   - 第一个非静态成员与基类不是同一类型" << std::endl;

    std::cout << "\n2. 什么使类型变为非 Standard Layout：" << std::endl;
    std::cout << "   - 虚函数（需要 vtable）" << std::endl;
    std::cout << "   - 虚基类" << std::endl;
    std::cout << "   - 混合访问控制（public/private/protected）" << std::endl;
    std::cout << "   - 非 standard layout 的基类" << std::endl;
    std::cout << "   - 非 standard layout 的成员变量" << std::endl;
    std::cout << "   - 引用成员" << std::endl;

    std::cout << "\n3. 重要性：" << std::endl;
    std::cout << "   - 与 C 语言兼容" << std::endl;
    std::cout << "   - 可以安全地 reinterpret_cast 到首成员" << std::endl;
    std::cout << "   - 内存布局可预测" << std::endl;
    std::cout << "   - 适用于二进制序列化" << std::endl;

    std::cout << "\n4. 检查方法：" << std::endl;
    std::cout << "   - 使用 std::is_standard_layout_v<T>" << std::endl;
    std::cout << "   - 编译时类型检查" << std::endl;
    std::cout << "   - static_assert 验证" << std::endl;

    std::cout << "\n5. 与 Trivially Copyable 的区别：" << std::endl;
    std::cout << "   - Trivially Copyable: 关注拷贝/移动的安全性" << std::endl;
    std::cout << "   - Standard Layout: 关注内存布局和 C 兼容性" << std::endl;
    std::cout << "   - POD: 两者都是（C++11 前的概念）" << std::endl;

    return 0;
}