/**
 * 教学程序：对象生命周期 - 构造/析构顺序
 * 目标：深入理解 C++ 中对象的构造和析构顺序
 *
 * 核心知识点：
 * 1. 成员变量的构造/析构顺序
 * 2. 基类和派生类的构造/析构顺序
 * 3. 多重继承中的构造/析构顺序
 * 4. 虚继承中的构造/析构顺序
 * 5. 临时对象的构造/析构
 * 6. 拷贝构造和移动构造的调用时机
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>

// 辅助宏：打印构造/析构信息
#define LOG_CONSTRUCTOR(cls) std::cout << "构造: " << #cls << " @ " << this << std::endl;
#define LOG_DESTRUCTOR(cls) std::cout << "析构: " << #cls << " @ " << this << std::endl;
#define LOG_COPY_CONSTRUCTOR(cls) std::cout << "拷贝构造: " << #cls << " @ " << this << " from " << &other << std::endl;
#define LOG_MOVE_CONSTRUCTOR(cls) std::cout << "移动构造: " << #cls << " @ " << this << " from " << &other << std::endl;
#define LOG_COPY_ASSIGNMENT(cls) std::cout << "拷贝赋值: " << #cls << " @ " << this << " from " << &other << std::endl;
#define LOG_MOVE_ASSIGNMENT(cls) std::cout << "移动赋值: " << #cls << " @ " << this << " from " << &other << std::endl;

// ============================================================================
// 1. 成员变量的构造/析构顺序
// ============================================================================

class MemberA {
public:
    MemberA() { LOG_CONSTRUCTOR(MemberA); }
    ~MemberA() { LOG_DESTRUCTOR(MemberA); }
};

class MemberB {
public:
    MemberB() { LOG_CONSTRUCTOR(MemberB); }
    ~MemberB() { LOG_DESTRUCTOR(MemberB); }
};

class MemberC {
public:
    MemberC() { LOG_CONSTRUCTOR(MemberC); }
    ~MemberC() { LOG_DESTRUCTOR(MemberC); }
};

class Container {
private:
    MemberA a;
    MemberB b;
    MemberC c;

public:
    Container() {
        std::cout << "Container 构造开始" << std::endl;
        LOG_CONSTRUCTOR(Container);
    }

    ~Container() {
        LOG_DESTRUCTOR(Container);
        std::cout << "Container 析构结束" << std::endl;
    }
};

void test_member_construction_order() {
    std::cout << "\n=== 1. 成员变量构造/析构顺序 ===" << std::endl;
    std::cout << "规则：成员变量按照声明顺序构造，按相反顺序析构" << std::endl;

    {
        Container obj;
        std::cout << "对象使用中..." << std::endl;
    }

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 构造顺序：MemberA -> MemberB -> MemberC -> Container" << std::endl;
    std::cout << "- 析构顺序：Container -> MemberC -> MemberB -> MemberA" << std::endl;
    std::cout << "- 析构顺序与构造顺序相反（栈的后进先出特性）" << std::endl;
}

// ============================================================================
// 2. 基类和派生类的构造/析构顺序
// ============================================================================

class Base {
public:
    Base() {
        std::cout << "  ";
        LOG_CONSTRUCTOR(Base);
    }

    virtual ~Base() {
        std::cout << "  ";
        LOG_DESTRUCTOR(Base);
    }
};

class Derived : public Base {
private:
    MemberA member;

public:
    Derived() {
        std::cout << "  ";
        LOG_CONSTRUCTOR(Derived);
    }

    ~Derived() override {
        std::cout << "  ";
        LOG_DESTRUCTOR(Derived);
    }
};

void test_base_derived_order() {
    std::cout << "\n=== 2. 基类和派生类构造/析构顺序 ===" << std::endl;
    std::cout << "规则：先构造基类，再构造成员，最后构造派生类" << std::endl;

    {
        std::cout << "创建 Derived 对象：" << std::endl;
        Derived obj;
        std::cout << "对象使用中..." << std::endl;
    }

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 构造顺序：Base -> MemberA -> Derived" << std::endl;
    std::cout << "- 析构顺序：Derived -> MemberA -> Base" << std::endl;
    std::cout << "- 基类先构造，后析构" << std::endl;
}

// ============================================================================
// 3. 多重继承中的构造/析构顺序
// ============================================================================

class Base1 {
public:
    Base1() {
        std::cout << "  ";
        LOG_CONSTRUCTOR(Base1);
    }

    virtual ~Base1() {
        std::cout << "  ";
        LOG_DESTRUCTOR(Base1);
    }
};

class Base2 {
public:
    Base2() {
        std::cout << "  ";
        LOG_CONSTRUCTOR(Base2);
    }

    virtual ~Base2() {
        std::cout << "  ";
        LOG_DESTRUCTOR(Base2);
    }
};

class MultiDerived : public Base1, public Base2 {
public:
    MultiDerived() {
        std::cout << "  ";
        LOG_CONSTRUCTOR(MultiDerived);
    }

    ~MultiDerived() override {
        std::cout << "  ";
        LOG_DESTRUCTOR(MultiDerived);
    }
};

void test_multiple_inheritance_order() {
    std::cout << "\n=== 3. 多重继承构造/析构顺序 ===" << std::endl;
    std::cout << "规则：基类按照继承声明顺序构造，按相反顺序析构" << std::endl;

    {
        std::cout << "创建 MultiDerived 对象：" << std::endl;
        MultiDerived obj;
        std::cout << "对象使用中..." << std::endl;
    }

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 构造顺序：Base1 -> Base2 -> MultiDerived" << std::endl;
    std::cout << "- 析构顺序：MultiDerived -> Base2 -> Base1" << std::endl;
    std::cout << "- 基类按照声明顺序构造" << std::endl;
}

// ============================================================================
// 4. 虚继承中的构造/析构顺序
// ============================================================================

class VirtualBase {
public:
    VirtualBase() {
        std::cout << "  ";
        LOG_CONSTRUCTOR(VirtualBase);
    }

    virtual ~VirtualBase() {
        std::cout << "  ";
        LOG_DESTRUCTOR(VirtualBase);
    }
};

class VirtualDerived1 : public virtual VirtualBase {
public:
    VirtualDerived1() {
        std::cout << "  ";
        LOG_CONSTRUCTOR(VirtualDerived1);
    }

    ~VirtualDerived1() override {
        std::cout << "  ";
        LOG_DESTRUCTOR(VirtualDerived1);
    }
};

class VirtualDerived2 : public virtual VirtualBase {
public:
    VirtualDerived2() {
        std::cout << "  ";
        LOG_CONSTRUCTOR(VirtualDerived2);
    }

    ~VirtualDerived2() override {
        std::cout << "  ";
        LOG_DESTRUCTOR(VirtualDerived2);
    }
};

class DiamondDerived : public VirtualDerived1, public VirtualDerived2 {
public:
    DiamondDerived() {
        std::cout << "  ";
        LOG_CONSTRUCTOR(DiamondDerived);
    }

    ~DiamondDerived() override {
        std::cout << "  ";
        LOG_DESTRUCTOR(DiamondDerived);
    }
};

void test_virtual_inheritance_order() {
    std::cout << "\n=== 4. 虚继承构造/析构顺序 ===" << std::endl;
    std::cout << "规则：虚基类只构造一次，在最开始构造" << std::endl;

    {
        std::cout << "创建 DiamondDerived 对象：" << std::endl;
        DiamondDerived obj;
        std::cout << "对象使用中..." << std::endl;
    }

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 构造顺序：VirtualBase -> VirtualDerived1 -> VirtualDerived2 -> DiamondDerived" << std::endl;
    std::cout << "- 析构顺序：DiamondDerived -> VirtualDerived2 -> VirtualDerived1 -> VirtualBase" << std::endl;
    std::cout << "- VirtualBase 只构造一次（避免菱形继承问题）" << std::endl;
}

// ============================================================================
// 5. 临时对象的构造/析构
// ============================================================================

class TempObject {
public:
    TempObject() {
        std::cout << "  ";
        LOG_CONSTRUCTOR(TempObject);
    }

    TempObject(const TempObject& other) {
        LOG_COPY_CONSTRUCTOR(TempObject);
    }

    TempObject(TempObject&& other) noexcept {
        LOG_MOVE_CONSTRUCTOR(TempObject);
    }

    ~TempObject() {
        std::cout << "  ";
        LOG_DESTRUCTOR(TempObject);
    }
};

TempObject create_temp_object() {
    std::cout << "  函数内创建临时对象：" << std::endl;
    TempObject obj;
    return obj;  // 可能触发 RVO/NRVO
}

void test_temporary_objects() {
    std::cout << "\n=== 5. 临时对象构造/析构 ===" << std::endl;
    std::cout << "规则：临时对象在表达式结束时析构" << std::endl;

    std::cout << "情况1：直接创建临时对象" << std::endl;
    TempObject();  // 临时对象，立即析构
    std::cout << "  临时对象已析构" << std::endl;

    std::cout << "\n情况2：函数返回临时对象（可能触发 RVO）" << std::endl;
    TempObject obj = create_temp_object();
    std::cout << "  对象使用中..." << std::endl;

    std::cout << "\n情况3：const 引用延长临时对象生命周期" << std::endl;
    {
        const TempObject& ref = TempObject();
        std::cout << "  临时对象生命周期延长到 ref 作用域结束" << std::endl;
    }
    std::cout << "  ref 作用域结束，临时对象析构" << std::endl;

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 临时对象在表达式结束时析构" << std::endl;
    std::cout << "- const 引用可以延长临时对象的生命周期" << std::endl;
    std::cout << "- RVO/NRVO 可以减少不必要的拷贝/移动" << std::endl;
}

// ============================================================================
// 6. 拷贝构造和移动构造的调用时机
// ============================================================================

class CopyMoveObject {
private:
    std::string data;

public:
    CopyMoveObject() : data("default") {
        std::cout << "  ";
        LOG_CONSTRUCTOR(CopyMoveObject);
    }

    CopyMoveObject(const std::string& str) : data(str) {
        std::cout << "  ";
        LOG_CONSTRUCTOR(CopyMoveObject);
    }

    CopyMoveObject(const CopyMoveObject& other) : data(other.data) {
        LOG_COPY_CONSTRUCTOR(CopyMoveObject);
    }

    CopyMoveObject(CopyMoveObject&& other) noexcept : data(std::move(other.data)) {
        LOG_MOVE_CONSTRUCTOR(CopyMoveObject);
    }

    CopyMoveObject& operator=(const CopyMoveObject& other) {
        LOG_COPY_ASSIGNMENT(CopyMoveObject);
        if (this != &other) {
            data = other.data;
        }
        return *this;
    }

    CopyMoveObject& operator=(CopyMoveObject&& other) noexcept {
        LOG_MOVE_ASSIGNMENT(CopyMoveObject);
        if (this != &other) {
            data = std::move(other.data);
        }
        return *this;
    }

    ~CopyMoveObject() {
        std::cout << "  ";
        LOG_DESTRUCTOR(CopyMoveObject);
    }
};

void test_copy_move_semantics() {
    std::cout << "\n=== 6. 拷贝和移动构造调用时机 ===" << std::endl;

    std::cout << "情况1：拷贝构造" << std::endl;
    {
        CopyMoveObject obj1("original");
        std::cout << "  拷贝构造 obj2：" << std::endl;
        CopyMoveObject obj2 = obj1;  // 拷贝构造
    }

    std::cout << "\n情况2：移动构造" << std::endl;
    {
        std::cout << "  创建临时对象并移动：" << std::endl;
        CopyMoveObject obj3 = CopyMoveObject("temporary");  // 可能触发移动构造
    }

    std::cout << "\n情况3：std::move 强制移动" << std::endl;
    {
        CopyMoveObject obj4("source");
        std::cout << "  移动构造 obj5：" << std::endl;
        CopyMoveObject obj5 = std::move(obj4);  // 移动构造
    }

    std::cout << "\n情况4：拷贝赋值和移动赋值" << std::endl;
    {
        CopyMoveObject obj6("first");
        CopyMoveObject obj7("second");

        std::cout << "  拷贝赋值：" << std::endl;
        obj6 = obj7;  // 拷贝赋值

        std::cout << "  移动赋值：" << std::endl;
        obj6 = std::move(obj7);  // 移动赋值
    }

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 拷贝构造：用左值初始化新对象" << std::endl;
    std::cout << "- 移动构造：用右值（临时对象或 std::move）初始化新对象" << std::endl;
    std::cout << "- 编译器优化可能影响实际调用的构造函数" << std::endl;
}

// ============================================================================
// 7. 容器中的对象构造/析构
// ============================================================================

void test_container_objects() {
    std::cout << "\n=== 7. 容器中对象的构造/析构 ===" << std::endl;

    std::cout << "情况1：vector 扩容时的拷贝/移动" << std::endl;
    {
        std::vector<CopyMoveObject> vec;
        vec.reserve(2);  // 预留空间，避免立即扩容

        std::cout << "  添加第一个元素：" << std::endl;
        vec.emplace_back("first");

        std::cout << "  添加第二个元素（可能触发扩容）：" << std::endl;
        vec.emplace_back("second");

        std::cout << "  添加第三个元素（触发扩容）：" << std::endl;
        vec.emplace_back("third");
    }

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- vector 扩容时会重新分配内存" << std::endl;
    std::cout << "- 旧元素会被移动或拷贝到新内存" << std::endl;
    std::cout << "- 旧元素会被析构" << std::endl;
}

// ============================================================================
// 8. 智能指针和对象生命周期
// ============================================================================

void test_smart_pointers() {
    std::cout << "\n=== 8. 智能指针和对象生命周期 ===" << std::endl;

    std::cout << "情况1：unique_ptr" << std::endl;
    {
        auto ptr = std::make_unique<CopyMoveObject>("unique");
        std::cout << "  unique_ptr 管理对象中..." << std::endl;
    }
    std::cout << "  unique_ptr 离开作用域，对象自动析构" << std::endl;

    std::cout << "\n情况2：shared_ptr" << std::endl;
    {
        auto ptr1 = std::make_shared<CopyMoveObject>("shared");
        std::cout << "  引用计数: " << ptr1.use_count() << std::endl;

        {
            auto ptr2 = ptr1;
            std::cout << "  引用计数: " << ptr1.use_count() << std::endl;
            std::cout << "  ptr2 离开作用域" << std::endl;
        }

        std::cout << "  引用计数: " << ptr1.use_count() << std::endl;
    }
    std::cout << "  最后一个 shared_ptr 离开作用域，对象析构" << std::endl;
}

// ============================================================================
// 主函数
// ============================================================================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "对象生命周期：构造/析构顺序" << std::endl;
    std::cout << "========================================" << std::endl;

    test_member_construction_order();
    test_base_derived_order();
    test_multiple_inheritance_order();
    test_virtual_inheritance_order();
    test_temporary_objects();
    test_copy_move_semantics();
    test_container_objects();
    test_smart_pointers();

    std::cout << "\n========================================" << std::endl;
    std::cout << "总结" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. 成员变量：按声明顺序构造，相反顺序析构" << std::endl;
    std::cout << "2. 继承关系：基类先构造，后析构" << std::endl;
    std::cout << "3. 多重继承：按声明顺序构造基类" << std::endl;
    std::cout << "4. 虚继承：虚基类只构造一次" << std::endl;
    std::cout << "5. 临时对象：表达式结束时析构" << std::endl;
    std::cout << "6. 拷贝/移动：根据值类别选择构造函数" << std::endl;
    std::cout << "7. 容器扩容：可能触发移动构造" << std::endl;
    std::cout << "8. 智能指针：自动管理对象生命周期" << std::endl;

    std::cout << "\n反汇编验证方法：" << std::endl;
    std::cout << "g++ -g -O0 -S 01_object_lifecycle.cpp -o 01_object_lifecycle.s" << std::endl;
    std::cout << "查看构造/析构函数的调用顺序" << std::endl;

    return 0;
}