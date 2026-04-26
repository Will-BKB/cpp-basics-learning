/**
 * 教学程序：零初始化、默认初始化、值初始化
 * 目标：深入理解 C++ 中不同初始化方式的差异
 *
 * 核心知识点：
 * 1. 零初始化（Zero Initialization）
 * 2. 默认初始化（Default Initialization）
 * 3. 值初始化（Value Initialization）
 * 4. 不同初始化方式的性能影响
 * 5. 未定义行为的风险
 */

#include <iostream>
#include <string>
#include <vector>

// 辅助宏：打印变量信息
#define PRINT_VAR(name, value) std::cout << #name << " = " << value << " @ " << &name << std::endl;

// ============================================================================
// 1. 基本类型的初始化差异
// ============================================================================

void test_basic_types() {
    std::cout << "\n=== 1. 基本类型的初始化差异 ===" << std::endl;

    // 默认初始化（可能未初始化，值不确定）
    int a;                      // 未初始化，值不确定（UB）
    int b = int();              // 值初始化，零初始化为 0
    int c{};                    // 值初始化，零初始化为 0（C++11）
    int d = 0;                  // 直接初始化，值为 0
    int e{0};                   // 列表初始化，值为 0（C++11）

    std::cout << "int 类型的不同初始化方式：" << std::endl;
    PRINT_VAR(a, a);  // 未定义行为！
    PRINT_VAR(b, b);
    PRINT_VAR(c, c);
    PRINT_VAR(d, d);
    PRINT_VAR(e, e);

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- int a: 默认初始化，值不确定（未定义行为）" << std::endl;
    std::cout << "- int b = int(): 值初始化，零初始化为 0" << std::endl;
    std::cout << "- int c{}: 值初始化，零初始化为 0（推荐）" << std::endl;
    std::cout << "- int d = 0: 直接初始化，值为 0" << std::endl;
    std::cout << "- int e{0}: 列表初始化，值为 0" << std::endl;
}

// ============================================================================
// 2. 指针类型的初始化差异
// ============================================================================

void test_pointer_types() {
    std::cout << "\n=== 2. 指针类型的初始化差异 ===" << std::endl;

    // 默认初始化（可能未初始化，值不确定）
    int* p1;                     // 未初始化，野指针（UB）
    int* p2 = nullptr;          // 初始化为 nullptr
    int* p3{};                  // 初始化为 nullptr（C++11）
    int* p4 = nullptr;          // 初始化为 nullptr
    int* p5{nullptr};           // 初始化为 nullptr（C++11）

    std::cout << "指针类型的不同初始化方式：" << std::endl;
    std::cout << "p1 = " << p1 << " (未初始化，野指针！)" << std::endl;
    std::cout << "p2 = " << p2 << std::endl;
    std::cout << "p3 = " << p3 << std::endl;
    std::cout << "p4 = " << p4 << std::endl;
    std::cout << "p5 = " << p5 << std::endl;

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- int* p1: 默认初始化，野指针（危险！）" << std::endl;
    std::cout << "- int* p2 = nullptr: 初始化为空指针" << std::endl;
    std::cout << "- int* p3{}: 初始化为 nullptr（推荐）" << std::endl;
}

// ============================================================================
// 3. 类类型的初始化差异
// ============================================================================

class SimpleClass {
public:
    int x;
    double y;

    // 默认构造函数（不初始化成员）
    SimpleClass() {}  // 成员 x, y 未初始化

    void print() const {
        std::cout << "SimpleClass { x=" << x << ", y=" << y << " }" << std::endl;
    }
};

class InitializedClass {
public:
    int x;
    double y;

    // 默认构造函数（使用初始化列表）
    InitializedClass() : x(0), y(0.0) {}

    void print() const {
        std::cout << "InitializedClass { x=" << x << ", y=" << y << " }" << std::endl;
    }
};

void test_class_types() {
    std::cout << "\n=== 3. 类类型的初始化差异 ===" << std::endl;

    // SimpleClass 的默认构造函数不初始化成员
    SimpleClass s1;              // 默认初始化，成员未初始化
    SimpleClass s2 = SimpleClass();  // 值初始化，但成员仍未初始化
    SimpleClass s3{};            // 值初始化，但成员仍未初始化

    std::cout << "SimpleClass（默认构造函数不初始化成员）：" << std::endl;
    std::cout << "s1: "; s1.print();
    std::cout << "s2: "; s2.print();
    std::cout << "s3: "; s3.print();

    // InitializedClass 的默认构造函数初始化成员
    InitializedClass i1;        // 默认初始化，成员被初始化
    InitializedClass i2 = InitializedClass();  // 值初始化，成员被初始化
    InitializedClass i3{};       // 值初始化，成员被初始化

    std::cout << "\nInitializedClass（默认构造函数初始化成员）：" << std::endl;
    std::cout << "i1: "; i1.print();
    std::cout << "i2: "; i2.print();
    std::cout << "i3: "; i3.print();

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 类的初始化行为取决于构造函数" << std::endl;
    std::cout << "- 如果构造函数不初始化成员，成员值不确定" << std::endl;
    std::cout << "- 值初始化对类类型的影响取决于构造函数" << std::endl;
}

// ============================================================================
// 4. 数组的初始化差异
// ============================================================================

void test_array_initialization() {
    std::cout << "\n=== 4. 数组的初始化差异 ===" << std::endl;

    // 默认初始化（可能未初始化）
    int arr1[5];                // 未初始化，值不确定
    int arr2[5] = {};           // 值初始化，所有元素为 0
    int arr3[5] = {1, 2, 3};    // 部分初始化，其余为 0
    int arr4[5] = {0};          // 第一个元素为 0，其余为 0

    std::cout << "int arr1[5]（默认初始化）: ";
    for (int i = 0; i < 5; ++i) std::cout << arr1[i] << " ";
    std::cout << "(值不确定)" << std::endl;

    std::cout << "int arr2[5] = {}（值初始化）: ";
    for (int i = 0; i < 5; ++i) std::cout << arr2[i] << " ";
    std::cout << std::endl;

    std::cout << "int arr3[5] = {1, 2, 3}（部分初始化）: ";
    for (int i = 0; i < 5; ++i) std::cout << arr3[i] << " ";
    std::cout << std::endl;

    std::cout << "int arr4[5] = {0}（第一个为0）: ";
    for (int i = 0; i < 5; ++i) std::cout << arr4[i] << " ";
    std::cout << std::endl;

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- int arr[5]: 默认初始化，值不确定" << std::endl;
    std::cout << "- int arr[5] = {}: 值初始化，所有元素为 0" << std::endl;
    std::cout << "- int arr[5] = {1, 2}: 部分初始化，其余为 0" << std::endl;
}

// ============================================================================
// 5. 容器的初始化差异
// ============================================================================

void test_container_initialization() {
    std::cout << "\n=== 5. 容器的初始化差异 ===" << std::endl;

    // vector 的默认构造函数创建空容器
    std::vector<int> v1;                    // 空容器
    std::vector<int> v2(5);                 // 5 个元素，值初始化为 0
    std::vector<int> v3(5, 42);             // 5 个元素，值为 42
    std::vector<int> v4{1, 2, 3, 4, 5};     // 列表初始化

    std::cout << "std::vector<int> v1（默认构造）: size=" << v1.size() << std::endl;
    std::cout << "std::vector<int> v2(5)（5个元素）: ";
    for (int x : v2) std::cout << x << " ";
    std::cout << std::endl;

    std::cout << "std::vector<int> v3(5, 42)（5个42）: ";
    for (int x : v3) std::cout << x << " ";
    std::cout << std::endl;

    std::cout << "std::vector<int> v4{1,2,3,4,5}（列表初始化）: ";
    for (int x : v4) std::cout << x << " ";
    std::cout << std::endl;

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 容器的默认构造函数会值初始化元素" << std::endl;
    std::cout << "- std::vector<int> v(5) 创建 5 个值为 0 的元素" << std::endl;
}

// ============================================================================
// 6. 不同存储期的初始化
// ============================================================================

int global_var;              // 全局变量，零初始化
int global_var_init = 10;    // 全局变量，显式初始化

static int static_var;       // 静态变量，零初始化
static int static_var_init = 20;  // 静态变量，显式初始化

void test_storage_duration() {
    std::cout << "\n=== 6. 不同存储期的初始化 ===" << std::endl;

    static int local_static_var;        // 局部静态变量，零初始化
    static int local_static_var_init = 30;  // 局部静态变量，显式初始化

    int local_var;              // 局部变量，未初始化（UB）
    int local_var_init = 40;    // 局部变量，显式初始化

    std::cout << "全局变量：" << std::endl;
    PRINT_VAR(global_var, global_var);
    PRINT_VAR(global_var_init, global_var_init);

    std::cout << "\n静态变量：" << std::endl;
    PRINT_VAR(static_var, static_var);
    PRINT_VAR(static_var_init, static_var_init);
    PRINT_VAR(local_static_var, local_static_var);
    PRINT_VAR(local_static_var_init, local_static_var_init);

    std::cout << "\n局部变量：" << std::endl;
    PRINT_VAR(local_var, local_var);  // 未定义行为！
    PRINT_VAR(local_var_init, local_var_init);

    std::cout << "\n观察结果：" << std::endl;
    std::cout << "- 全局/静态变量：零初始化为 0" << std::endl;
    std::cout << "- 局部变量：默认初始化，值不确定（UB）" << std::endl;
    std::cout << "- 局部静态变量：零初始化为 0" << std::endl;
}

// ============================================================================
// 7. 最佳实践
// ============================================================================

void test_best_practices() {
    std::cout << "\n=== 8. 最佳实践 ===" << std::endl;

    // 推荐的初始化方式
    int recommended1{};        // 推荐：值初始化
    int recommended2 = 0;      // 可接受：直接初始化
    int* recommended3 = nullptr;  // 推荐：指针初始化

    // 不推荐的初始化方式
    // int not_recommended1;    // 不推荐：未初始化
    // int* not_recommended2;    // 不推荐：野指针

    std::cout << "推荐的初始化方式：" << std::endl;
    PRINT_VAR(recommended1, recommended1);
    std::cout << "recommended3 = " << recommended3 << std::endl;

    std::cout << "\n最佳实践建议：" << std::endl;
    std::cout << "1. 基本类型：使用 int x{} 或 int x = 0" << std::endl;
    std::cout << "2. 指针类型：使用 int* p = nullptr 或 int* p{}" << std::endl;
    std::cout << "3. 类类型：依赖构造函数，确保正确初始化" << std::endl;
    std::cout << "4. 容器类型：使用容器的构造函数" << std::endl;
    std::cout << "5. 避免未初始化的变量，防止未定义行为" << std::endl;
}

// ============================================================================
// 主函数
// ============================================================================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "零初始化、默认初始化、值初始化" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "编译说明：" << std::endl;
    std::cout << "g++ -g -O0 -std=c++17 04_initialization.cpp -o 04_initialization" << std::endl;

    std::cout << "\n警告：以下代码包含未定义行为（UB）" << std::endl;
    std::cout << "未初始化的变量值是不确定的，可能导致程序崩溃" << std::endl;

    test_basic_types();
    test_pointer_types();
    test_class_types();
    test_array_initialization();
    test_container_initialization();
    test_storage_duration();
    test_best_practices();

    std::cout << "\n========================================" << std::endl;
    std::cout << "总结" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. 零初始化：静态/全局变量自动初始化为 0" << std::endl;
    std::cout << "2. 默认初始化：局部变量不初始化，值不确定（UB）" << std::endl;
    std::cout << "3. 值初始化：使用 T() 或 T{} 初始化为 0" << std::endl;
    std::cout << "4. int a: 未初始化，危险！" << std::endl;
    std::cout << "5. int a{}: 值初始化，安全！" << std::endl;
    std::cout << "6. int a = int(): 值初始化，安全！" << std::endl;
    std::cout << "7. 性能影响：初始化开销可忽略，但安全性大幅提升" << std::endl;

    std::cout << "\n验证方法（可选）：" << std::endl;
    std::cout << "如需验证汇编层面的差异，可以生成汇编文件：" << std::endl;
    std::cout << "g++ -g -O0 -std=c++17 -S 04_initialization.cpp -o 04_initialization.s" << std::endl;
    std::cout << "注意：不同平台的汇编代码可能不同" << std::endl;

    return 0;
}