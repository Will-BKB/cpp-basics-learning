/**
 * 教学程序：拷贝消除（RVO/NRVO）
 * 目标：深入理解编译器的拷贝消除优化
 *
 * 核心知识点：
 * 1. RVO (Return Value Optimization) - 返回值优化
 * 2. NRVO (Named Return Value Optimization) - 命名返回值优化
 * 3. 拷贝消除的触发条件
 * 4. -fno-elide-constructors 的影响
 * 5. 不同优化级别的差异
 */

#include <iostream>
#include <string>
#include <vector>

// 辅助宏：打印构造/析构信息
#define LOG_CONSTRUCTOR(cls) std::cout << "构造: " << #cls << " @ " << this << std::endl;
#define LOG_DESTRUCTOR(cls) std::cout << "析构: " << #cls << " @ " << this << std::endl;
#define LOG_COPY_CONSTRUCTOR(cls) std::cout << "拷贝构造: " << #cls << " @ " << this << " from " << &other << std::endl;
#define LOG_MOVE_CONSTRUCTOR(cls) std::cout << "移动构造: " << #cls << " @ " << this << " from " << &other << std::endl;

// ============================================================================
// 测试类：带日志的简单类
// ============================================================================

class TestObject {
private:
    std::string data;
    int id;

public:
    TestObject() : data("default"), id(0) {
        LOG_CONSTRUCTOR(TestObject);
    }

    explicit TestObject(const std::string& str) : data(str), id(0) {
        LOG_CONSTRUCTOR(TestObject);
    }

    TestObject(const TestObject& other) : data(other.data), id(other.id) {
        LOG_COPY_CONSTRUCTOR(TestObject);
    }

    TestObject(TestObject&& other) noexcept : data(std::move(other.data)), id(other.id) {
        LOG_MOVE_CONSTRUCTOR(TestObject);
    }

    ~TestObject() {
        LOG_DESTRUCTOR(TestObject);
    }

    const std::string& get_data() const { return data; }
};

// ============================================================================
// 1. RVO (Return Value Optimization) - 返回值优化
// ============================================================================

TestObject create_rvo() {
    std::cout << "  create_rvo: 创建临时对象" << std::endl;
    return TestObject("RVO");  // RVO: 直接在调用者的内存中构造
}

void test_rvo() {
    std::cout << "\n=== 1. RVO (Return Value Optimization) ===" << std::endl;
    std::cout << "场景：函数返回临时对象" << std::endl;

    std::cout << "调用 create_rvo():" << std::endl;
    TestObject obj = create_rvo();  // 理论上应该触发 RVO

    std::cout << "对象创建完成" << std::endl;
    std::cout << "obj.data: " << obj.get_data() << std::endl;

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 理想情况（RVO）：只调用一次构造函数，无拷贝/移动" << std::endl;
    std::cout << "- 实际情况取决于编译器和优化级别" << std::endl;
    std::cout << "- C++17 强制要求在某些情况下进行拷贝消除" << std::endl;
}

// ============================================================================
// 2. NRVO (Named Return Value Optimization) - 命名返回值优化
// ============================================================================

TestObject create_nrvo() {
    std::cout << "  create_nrvo: 创建命名对象" << std::endl;
    TestObject obj("NRVO");  // NRVO: 命名对象直接在调用者的内存中构造
    std::cout << "  create_nrvo: 返回命名对象" << std::endl;
    return obj;
}

void test_nrvo() {
    std::cout << "\n=== 2. NRVO (Named Return Value Optimization) ===" << std::endl;
    std::cout << "场景：函数返回命名对象" << std::endl;

    std::cout << "调用 create_nrvo():" << std::endl;
    TestObject obj = create_nrvo();  // 理论上应该触发 NRVO

    std::cout << "对象创建完成" << std::endl;
    std::cout << "obj.data: " << obj.get_data() << std::endl;

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 理想情况（NRVO）：只调用一次构造函数，无拷贝/移动" << std::endl;
    std::cout << "- NRVO 比 RVO 更复杂，需要编译器分析控制流" << std::endl;
    std::cout << "- 多个返回路径可能阻止 NRVO" << std::endl;
}

// ============================================================================
// 3. 无法触发 RVO/NRVO 的情况
// ============================================================================

TestObject create_no_nrvo(bool flag) {
    std::cout << "  create_no_nrvo: 条件返回命名对象" << std::endl;
    TestObject obj1("path1");
    TestObject obj2("path2");

    if (flag) {
        return obj1;  // NRVO 失败：多个命名对象
    } else {
        return obj2;  // NRVO 失败：多个命名对象
    }
}

void test_no_elision() {
    std::cout << "\n=== 3. 无法触发 RVO/NRVO 的情况 ===" << std::endl;
    std::cout << "场景：函数有多个返回路径，返回命名对象" << std::endl;

    std::cout << "调用 create_no_nrvo(true):" << std::endl;
    TestObject obj1 = create_no_nrvo(true);

    std::cout << "\n调用 create_no_nrvo(false):" << std::endl;
    TestObject obj2 = create_no_nrvo(false);

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 多个命名对象返回会阻止 NRVO" << std::endl;
    std::cout << "- 但 RVO 仍然可能对临时对象生效" << std::endl;
    std::cout << "- 编译器需要更复杂的分析来确定是否可以消除拷贝" << std::endl;
    std::cout << "- 使用 -fno-elide-constructors 可以看到移动构造" << std::endl;
}

// ============================================================================
// 4. 拷贝初始化 vs 直接初始化
// ============================================================================

void test_initialization() {
    std::cout << "\n=== 4. 拷贝初始化 vs 直接初始化 ===" << std::endl;

    std::cout << "情况1：拷贝初始化（可能触发 RVO）" << std::endl;
    TestObject obj1 = TestObject("copy_init");

    std::cout << "\n情况2：直接初始化" << std::endl;
    TestObject obj2(TestObject("direct_init"));

    std::cout << "\n情况3：移动初始化" << std::endl;
    TestObject temp("temp");
    TestObject obj3 = std::move(temp);

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 拷贝初始化和直接初始化在 C++ 中语义相同" << std::endl;
    std::cout << "- 都可能触发 RVO/NRVO" << std::endl;
    std::cout << "- std::move 显式要求移动语义" << std::endl;
}

// ============================================================================
// 5. 容器中的 RVO/NRVO
// ============================================================================

void test_container_rvo() {
    std::cout << "\n=== 5. 容器中的 RVO/NRVO ===" << std::endl;

    std::cout << "情况1：vector::push_back" << std::endl;
    std::vector<TestObject> vec1;
    vec1.push_back(TestObject("push_back"));

    std::cout << "\n情况2：vector::emplace_back" << std::endl;
    std::vector<TestObject> vec2;
    vec2.emplace_back("emplace_back");

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- push_back 可能触发移动构造（如果 RVO 失败）" << std::endl;
    std::cout << "- emplace_back 直接在容器中构造，无临时对象" << std::endl;
    std::cout << "- emplace_back 是避免拷贝/移动的最佳方式" << std::endl;
}

// ============================================================================
// 6. 函数参数中的拷贝消除
// ============================================================================

void take_by_value(TestObject obj) {
    std::cout << "  take_by_value: 接收对象" << std::endl;
    std::cout << "  obj.data: " << obj.get_data() << std::endl;
}

void test_function_arguments() {
    std::cout << "\n=== 6. 函数参数中的拷贝消除 ===" << std::endl;

    std::cout << "情况1：传递临时对象" << std::endl;
    take_by_value(TestObject("temp_arg"));

    std::cout << "\n情况2：传递命名对象" << std::endl;
    TestObject named("named_arg");
    take_by_value(named);

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 传递临时对象可能触发拷贝消除" << std::endl;
    std::cout << "- 传递命名对象会调用拷贝/移动构造" << std::endl;
    std::cout << "- 考虑使用 const 引用避免拷贝" << std::endl;
}

// ============================================================================
// 7. C++17 强制拷贝消除
// ============================================================================

struct Cpp17Elision {
    Cpp17Elision() { std::cout << "  构造 Cpp17Elision" << std::endl; }
    Cpp17Elision(const Cpp17Elision&) { std::cout << "  拷贝构造 Cpp17Elision" << std::endl; }
    Cpp17Elision(Cpp17Elision&&) { std::cout << "  移动构造 Cpp17Elision" << std::endl; }
    ~Cpp17Elision() { std::cout << "  析构 Cpp17Elision" << std::endl; }
};

Cpp17Elision create_cpp17_elision() {
    return Cpp17Elision();  // C++17 强制消除
}

void test_cpp17_elision() {
    std::cout << "\n=== 7. C++17 强制拷贝消除 ===" << std::endl;
    std::cout << "场景：C++17 中的强制拷贝消除" << std::endl;

    std::cout << "调用 create_cpp17_elision():" << std::endl;
    Cpp17Elision obj = create_cpp17_elision();

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- C++17 强制要求消除 prvalue 的拷贝/移动" << std::endl;
    std::cout << "- 即使没有移动构造函数也能编译" << std::endl;
    std::cout << "- 这是 C++17 的重要改进" << std::endl;
}

// ============================================================================
// 8. 不同优化级别的对比
// ============================================================================

void test_optimization_levels() {
    std::cout << "\n=== 8. 不同优化级别的对比 ===" << std::endl;

    std::cout << "测试相同的代码在不同优化级别下的行为：" << std::endl;
    std::cout << "建议使用以下命令编译和对比：" << std::endl;

    std::cout << "\n# 无优化（-O0）" << std::endl;
    std::cout << "g++ -g -O0 -std=c++17 03_copy_elision.cpp -o 03_copy_elision_O0" << std::endl;

    std::cout << "\n# 默认优化（-O1）" << std::endl;
    std::cout << "g++ -g -O1 -std=c++17 03_copy_elision.cpp -o 03_copy_elision_O1" << std::endl;

    std::cout << "\n# 高级优化（-O2）" << std::endl;
    std::cout << "g++ -g -O2 -std=c++17 03_copy_elision.cpp -o 03_copy_elision_O2" << std::endl;

    std::cout << "\n# 禁用拷贝消除（用于对比）" << std::endl;
    std::cout << "g++ -g -O0 -fno-elide-constructors -std=c++17 03_copy_elision.cpp -o 03_copy_elision_no_elision" << std::endl;

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- -O0：可能不进行 RVO/NRVO" << std::endl;
    std::cout << "- -O1/-O2：更积极的优化，更容易触发 RVO/NRVO" << std::endl;
    std::cout << "- -fno-elide-constructors：禁用拷贝消除，用于调试" << std::endl;
}

// ============================================================================
// 主函数
// ============================================================================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "拷贝消除（RVO/NRVO）" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "编译说明：" << std::endl;
    std::cout << "正常编译（可能触发 RVO/NRVO）：" << std::endl;
    std::cout << "  g++ -g -O0 -std=c++17 03_copy_elision.cpp -o 03_copy_elision" << std::endl;
    std::cout << "\n禁用拷贝消除（用于对比）：" << std::endl;
    std::cout << "  g++ -g -O0 -fno-elide-constructors -std=c++17 03_copy_elision.cpp -o 03_copy_elision_no_elision" << std::endl;

    test_rvo();
    test_nrvo();
    test_no_elision();
    test_initialization();
    test_container_rvo();
    test_function_arguments();
    test_cpp17_elision();
    test_optimization_levels();

    std::cout << "\n========================================" << std::endl;
    std::cout << "总结" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. RVO：返回临时对象时直接在调用者内存中构造" << std::endl;
    std::cout << "2. NRVO：返回命名对象时直接在调用者内存中构造" << std::endl;
    std::cout << "3. 拷贝消除：编译器优化，避免不必要的拷贝/移动" << std::endl;
    std::cout << "4. C++17：强制要求某些情况下的拷贝消除" << std::endl;
    std::cout << "5. 优化级别：影响 RVO/NRVO 的触发" << std::endl;
    std::cout << "6. -fno-elide-constructors：禁用拷贝消除用于调试" << std::endl;

    std::cout << "\n反汇编验证方法：" << std::endl;
    std::cout << "g++ -g -O0 -S 03_copy_elision.cpp -o 03_copy_elision.s" << std::endl;
    std::cout << "查看构造/析构函数的调用次数" << std::endl;
    std::cout << "对比 -O0 和 -O2 的汇编代码差异" << std::endl;

    return 0;
}