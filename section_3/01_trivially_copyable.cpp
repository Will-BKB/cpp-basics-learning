/**
 * 教学程序：Trivially Copyable 类型
 * 目标：深入理解什么是 trivially copyable 类型及其重要性
 *
 * 核心知识点：
 * 1. Trivially Copyable 的定义
 * 2. std::is_trivially_copyable_v 的使用
 * 3. 什么样的类型是 trivially copyable
 * 4. 什么样的类型不是 trivially copyable
 * 5. memcpy 安全性的重要性
 * 6. 手动实现类型特征检查
 */

#include <iostream>
#include <cstring>
#include <type_traits>
#include <string>
#include <vector>

// 辅助宏：打印类型特征
#define PRINT_TYPE_TRAIT(type, trait) \
    std::cout << #type << ": " << #trait << " = " << (trait<type>::value ? "true" : "false") << std::endl;

// 辅助宏：打印类型信息
#define PRINT_TYPE_INFO(type) \
    std::cout << "\n=== " << #type << " ===" << std::endl; \
    PRINT_TYPE_TRAIT(type, std::is_trivially_copyable); \
    PRINT_TYPE_TRAIT(type, std::is_trivial); \
    PRINT_TYPE_TRAIT(type, std::is_standard_layout);

// ============================================================================
// 1. 基本类型的 Trivially Copyable 检查
// ============================================================================

void test_basic_types() {
    std::cout << "\n=== 1. 基本类型的 Trivially Copyable 检查 ===" << std::endl;

    PRINT_TYPE_INFO(int)
    PRINT_TYPE_INFO(double)
    PRINT_TYPE_INFO(char)
    PRINT_TYPE_INFO(bool)
    PRINT_TYPE_INFO(int*)
    PRINT_TYPE_INFO(const int*)
    PRINT_TYPE_INFO(int[10])

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 所有基本类型都是 trivially copyable" << std::endl;
    std::cout << "- 指针类型也是 trivially copyable" << std::endl;
    std::cout << "- 数组类型如果元素是 trivially copyable，则数组也是" << std::endl;
}

// ============================================================================
// 2. 简单类的 Trivially Copyable 检查
// ============================================================================

class TrivialClass {
public:
    int x;
    double y;
    char z;

    // 默认构造函数、拷贝构造函数、析构函数都是 trivial 的
};

class NonTrivialClass {
public:
    int x;
    double y;
    char z;

    // 自定义构造函数（非 trivial）
    NonTrivialClass() : x(0), y(0.0), z('\0') {}

    // 自定义析构函数（非 trivial）
    ~NonTrivialClass() {}
};

void test_simple_classes() {
    std::cout << "\n=== 2. 简单类的 Trivially Copyable 检查 ===" << std::endl;

    PRINT_TYPE_INFO(TrivialClass)
    PRINT_TYPE_INFO(NonTrivialClass)

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- TrivialClass: 没有自定义构造/析构函数，是 trivially copyable" << std::endl;
    std::cout << "- NonTrivialClass: 有自定义构造/析构函数，不是 trivially copyable" << std::endl;
}

// ============================================================================
// 3. 虚函数对 Trivially Copyable 的影响
// ============================================================================

class ClassWithVirtualFunction {
public:
    int x;

    virtual void func() {  // 虚函数使类型变为非 trivial
        std::cout << "Virtual function" << std::endl;
    }

    virtual ~ClassWithVirtualFunction() {}  // 虚析构函数
};

class ClassWithoutVirtualFunction {
public:
    int x;

    void func() {  // 普通函数
        std::cout << "Non-virtual function" << std::endl;
    }

    // 使用默认的析构函数
};

void test_virtual_functions() {
    std::cout << "\n=== 3. 虚函数对 Trivially Copyable 的影响 ===" << std::endl;

    PRINT_TYPE_INFO(ClassWithVirtualFunction)
    PRINT_TYPE_INFO(ClassWithoutVirtualFunction)

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- ClassWithVirtualFunction: 有虚函数，不是 trivially copyable" << std::endl;
    std::cout << "- ClassWithoutVirtualFunction: 无虚函数，是 trivially copyable" << std::endl;
    std::cout << "- 虚函数需要 vtable，使对象变得复杂" << std::endl;
}

// ============================================================================
// 4. 拷贝/移动构造函数的影响
// ============================================================================

class ClassWithCopyConstructor {
public:
    int x;

    ClassWithCopyConstructor() = default;
    ClassWithCopyConstructor(const ClassWithCopyConstructor& other) : x(other.x) {  // 自定义拷贝构造
        std::cout << "Copy constructor called" << std::endl;
    }
};

class ClassWithDefaultCopyConstructor {
public:
    int x;

    ClassWithDefaultCopyConstructor() = default;
    // 使用默认的拷贝构造函数
};

class ClassWithDeletedCopyConstructor {
public:
    int x;

    ClassWithDeletedCopyConstructor() = default;
    ClassWithDeletedCopyConstructor(const ClassWithDeletedCopyConstructor&) = delete;  // 删除拷贝构造
};

void test_copy_constructors() {
    std::cout << "\n=== 4. 拷贝/移动构造函数的影响 ===" << std::endl;

    PRINT_TYPE_INFO(ClassWithCopyConstructor)
    PRINT_TYPE_INFO(ClassWithDefaultCopyConstructor)
    PRINT_TYPE_INFO(ClassWithDeletedCopyConstructor)

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- ClassWithCopyConstructor: 自定义拷贝构造，不是 trivially copyable" << std::endl;
    std::cout << "- ClassWithDefaultCopyConstructor: 默认拷贝构造，是 trivially copyable" << std::endl;
    std::cout << "- ClassWithDeletedCopyConstructor: 删除拷贝构造，但移动构造存在且平凡，仍是 trivially copyable" << std::endl;
}

// ============================================================================
// 5. 继承对 Trivially Copyable 的影响
// ============================================================================

class BaseClass {
public:
    int x;
};

class DerivedFromTrivial : public BaseClass {  // 继承自 trivially copyable 基类
public:
    int y;
};

class NonTrivialBase {
public:
    int x;

    NonTrivialBase() : x(0) {}  // 自定义构造函数（非 trivial）
    ~NonTrivialBase() {}  // 自定义析构函数（非 trivial）
};

class DerivedFromNonTrivial : public NonTrivialBase {  // 继承自非 trivially copyable 基类
public:
    int y;
};

void test_inheritance() {
    std::cout << "\n=== 5. 继承对 Trivially Copyable 的影响 ===" << std::endl;

    PRINT_TYPE_INFO(BaseClass)
    PRINT_TYPE_INFO(DerivedFromTrivial)
    PRINT_TYPE_INFO(NonTrivialBase)
    PRINT_TYPE_INFO(DerivedFromNonTrivial)

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- BaseClass: 是 trivially copyable" << std::endl;
    std::cout << "- DerivedFromTrivial: 继承自 trivially copyable 基类，也是 trivially copyable" << std::endl;
    std::cout << "- NonTrivialBase: 有自定义构造/析构函数，不是 trivially copyable" << std::endl;
    std::cout << "- DerivedFromNonTrivial: 继承自非 trivially copyable 基类，也不是 trivially copyable" << std::endl;
}

// ============================================================================
// 6. 成员变量对 Trivially Copyable 的影响
// ============================================================================

class ClassWithTrivialMember {
public:
    int x;  // trivially copyable 成员
};

class ClassWithNonTrivialMember {
public:
    std::string str;  // std::string 不是 trivially copyable
};

class ClassWithMixedMembers {
public:
    int x;              // trivially copyable
    std::string str;   // 非 trivially copyable
};

void test_member_variables() {
    std::cout << "\n=== 6. 成员变量对 Trivially Copyable 的影响 ===" << std::endl;

    PRINT_TYPE_INFO(ClassWithTrivialMember)
    PRINT_TYPE_INFO(ClassWithNonTrivialMember)
    PRINT_TYPE_INFO(ClassWithMixedMembers)
    PRINT_TYPE_INFO(std::string)

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- ClassWithTrivialMember: 所有成员都是 trivially copyable，类也是" << std::endl;
    std::cout << "- ClassWithNonTrivialMember: 有非 trivially copyable 成员，类也不是" << std::endl;
    std::cout << "- ClassWithMixedMembers: 混合成员，不是 trivially copyable" << std::endl;
    std::cout << "- std::string: 不是 trivially copyable（有动态内存管理）" << std::endl;
}

// ============================================================================
// 7. memcpy 安全性演示
// ============================================================================

void test_memcpy_safety() {
    std::cout << "\n=== 7. memcpy 安全性演示 ===" << std::endl;

    // Trivially copyable 类型可以安全使用 memcpy
    TrivialClass trivial1;
    trivial1.x = 42;
    trivial1.y = 3.14;
    trivial1.z = 'A';

    TrivialClass trivial2;
    std::memcpy(&trivial2, &trivial1, sizeof(TrivialClass));

    std::cout << "TrivialClass memcpy 结果：" << std::endl;
    std::cout << "  trivial1: x=" << trivial1.x << ", y=" << trivial1.y << ", z=" << trivial1.z << std::endl;
    std::cout << "  trivial2: x=" << trivial2.x << ", y=" << trivial2.y << ", z=" << trivial2.z << std::endl;
    std::cout << "  memcpy 安全：✓" << std::endl;

    // 非 trivially copyable 类型使用 memcpy 是危险的
    std::cout << "\n非 Trivially Copyable 类型（std::string）：" << std::endl;
    std::string str1 = "Hello, World!";
    std::string str2;

    std::cout << "  str1: " << str1 << std::endl;
    std::cout << "  str2: " << str2 << std::endl;

    // 危险！不要这样做！
    // std::memcpy(&str2, &str1, sizeof(std::string));
    std::cout << "  memcpy 危险：✗ (会导致双重释放等问题)" << std::endl;

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- Trivially copyable 类型可以安全使用 memcpy" << std::endl;
    std::cout << "- 非 trivially copyable 类型使用 memcpy 会导致未定义行为" << std::endl;
    std::cout << "- std::string 有内部指针，memcpy 会复制指针而非内容" << std::endl;
}

// ============================================================================
// 8. 手动实现简化的 is_trivially_copyable
// ============================================================================

template<typename T>
struct is_trivially_copyable_manual {
    static constexpr bool value =
        std::is_trivially_copy_constructible<T>::value &&
        std::is_trivially_move_constructible<T>::value &&
        std::is_trivially_copy_assignable<T>::value &&
        std::is_trivially_move_assignable<T>::value &&
        std::is_trivially_destructible<T>::value;
};

void test_manual_implementation() {
    std::cout << "\n=== 8. 手动实现简化的 is_trivially_copyable ===" << std::endl;

    std::cout << "对比标准库和手动实现：" << std::endl;

    std::cout << "\nTrivialClass:" << std::endl;
    std::cout << "  标准库: " << (std::is_trivially_copyable<TrivialClass>::value ? "true" : "false") << std::endl;
    std::cout << "  手动实现: " << (is_trivially_copyable_manual<TrivialClass>::value ? "true" : "false") << std::endl;

    std::cout << "\nNonTrivialClass:" << std::endl;
    std::cout << "  标准库: " << (std::is_trivially_copyable<NonTrivialClass>::value ? "true" : "false") << std::endl;
    std::cout << "  手动实现: " << (is_trivially_copyable_manual<NonTrivialClass>::value ? "true" : "false") << std::endl;

    std::cout << "\nstd::string:" << std::endl;
    std::cout << "  标准库: " << (std::is_trivially_copyable<std::string>::value ? "true" : "false") << std::endl;
    std::cout << "  手动实现: " << (is_trivially_copyable_manual<std::string>::value ? "true" : "false") << std::endl;

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 手动实现与标准库结果一致" << std::endl;
    std::cout << "- is_trivially_copyable 由多个 trivial 特征组成" << std::endl;
}

// ============================================================================
// 9. 实际应用场景
// ============================================================================

void test_practical_applications() {
    std::cout << "\n=== 9. 实际应用场景 ===" << std::endl;

    std::cout << "场景1：高性能数据传输" << std::endl;
    std::cout << "- trivially copyable 类型可以直接 memcpy" << std::endl;
    std::cout << "- 适用于网络传输、文件 I/O、共享内存" << std::endl;

    std::cout << "\n场景2：容器优化" << std::endl;
    std::cout << "- std::vector 对 trivially copyable 类型有特殊优化" << std::endl;
    std::cout << "- 可以使用 memcpy 批量移动元素" << std::endl;

    std::cout << "\n场景3：序列化/反序列化" << std::endl;
    std::cout << "- trivially copyable 类型可以安全地二进制序列化" << std::endl;
    std::cout << "- 非 trivially copyable 类型需要自定义序列化逻辑" << std::endl;

    std::cout << "\n场景4：内存池" << std::endl;
    std::cout << "- trivially copyable 类型适合使用内存池" << std::endl;
    std::cout << "- 可以直接复制内存块而不调用构造函数" << std::endl;
}

// ============================================================================
// 主函数
// ============================================================================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Trivially Copyable 类型" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "编译说明：" << std::endl;
    std::cout << "g++ -g -Wall -Wextra -std=c++17 01_trivially_copyable.cpp -o 01_trivially_copyable" << std::endl;

    test_basic_types();
    test_simple_classes();
    test_virtual_functions();
    test_copy_constructors();
    test_inheritance();
    test_member_variables();
    test_memcpy_safety();
    test_manual_implementation();
    test_practical_applications();

    std::cout << "\n========================================" << std::endl;
    std::cout << "总结" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. Trivially Copyable 定义：" << std::endl;
    std::cout << "   - 拥有平凡的拷贝/移动构造函数" << std::endl;
    std::cout << "   - 拥有平凡的拷贝/移动赋值运算符" << std::endl;
    std::cout << "   - 拥有平凡的析构函数" << std::endl;
    std::cout << "   - 至少有一个非删除的拷贝/移动构造函数" << std::endl;

    std::cout << "\n2. 什么使类型变为非 Trivially Copyable：" << std::endl;
    std::cout << "   - 虚函数（需要 vtable）" << std::endl;
    std::cout << "   - 自定义拷贝/移动构造函数" << std::endl;
    std::cout << "   - 自定义拷贝/移动赋值运算符" << std::endl;
    std::cout << "   - 自定义析构函数" << std::endl;
    std::cout << "   - 非 trivially copyable 的成员变量" << std::endl;
    std::cout << "   - 非 trivially copyable 的基类" << std::endl;

    std::cout << "\n3. 重要性：" << std::endl;
    std::cout << "   - 可以安全使用 memcpy" << std::endl;
    std::cout << "   - 适用于二进制序列化" << std::endl;
    std::cout << "   - 容器可以优化内存操作" << std::endl;
    std::cout << "   - 适合高性能场景" << std::endl;

    std::cout << "\n4. 检查方法：" << std::endl;
    std::cout << "   - 使用 std::is_trivially_copyable_v<T>" << std::endl;
    std::cout << "   - 编译时类型检查" << std::endl;
    std::cout << "   - static_assert 验证" << std::endl;

    return 0;
}