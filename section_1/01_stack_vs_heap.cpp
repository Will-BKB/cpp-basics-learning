/**
 * 测试知识点：栈 vs 堆内存
 * 核心问题：分配/释放开销、生命周期、大小限制
 * 验证手段：打印地址看区间，/proc/self/maps
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

#ifdef __APPLE__
#include <mach/mach.h>
#include <mach/vm_region.h>
#include <mach/vm_statistics.h>
#endif

// 全局变量（存储在 .data 或 .bss 段）
int global_var = 42;
static int static_var = 100;

// 常量全局变量（存储在 .rodata 段）
const int const_global_var = 200;

// 未初始化的全局变量（存储在 .bss 段）
int uninitialized_global_var;

// 函数：打印内存地址信息
void print_memory_info(const std::string& name, const void* addr) {
    std::cout << std::left << std::setw(30) << name
              << "Address: " << addr << std::endl;
}

// 函数：分析地址所在的内存区域
void analyze_memory_region(const void* addr) {
#ifdef __linux__
    std::ifstream maps("/proc/self/maps");
    if (!maps.is_open()) {
        std::cout << "  (无法打开 /proc/self/maps)" << std::endl;
        return;
    }

    std::string line;
    uintptr_t target_addr = reinterpret_cast<uintptr_t>(addr);

    while (std::getline(maps, line)) {
        std::istringstream iss(line);
        std::string range, perms, offset, dev, inode, pathname;

        iss >> range >> perms >> offset >> dev >> inode;
        // 读取剩余部分作为路径名
        std::string remaining;
        while (iss >> remaining) {
            if (!pathname.empty()) pathname += " ";
            pathname += remaining;
        }

        // 解析地址范围
        size_t dash_pos = range.find('-');
        if (dash_pos != std::string::npos) {
            uintptr_t start = std::stoull(range.substr(0, dash_pos), nullptr, 16);
            uintptr_t end = std::stoull(range.substr(dash_pos + 1), nullptr, 16);

            if (target_addr >= start && target_addr < end) {
                std::cout << "  区域: " << range << std::endl;
                std::cout << "  权限: " << perms << std::endl;
                std::cout << "  路径: " << (pathname.empty() ? "[匿名]" : pathname) << std::endl;
                return;
            }
        }
    }

    std::cout << "  (未找到对应的内存区域)" << std::endl;

#elif __APPLE__
    // macOS 使用 vm_region API
    mach_port_t task = mach_task_self();
    vm_address_t address = reinterpret_cast<vm_address_t>(addr);
    vm_size_t size = 0;
    vm_region_basic_info_data_64_t info;
    mach_msg_type_number_t count = VM_REGION_BASIC_INFO_COUNT_64;
    memory_object_name_t object;
    kern_return_t result = vm_region_64(task, &address, &size, VM_REGION_BASIC_INFO_64,
                                         reinterpret_cast<vm_region_info_t>(&info), &count, &object);

    if (result == KERN_SUCCESS) {
        std::cout << "  区域: 0x" << std::hex << address << " - 0x" << (address + size) << std::dec << std::endl;
        std::cout << "  大小: " << size << " bytes" << std::endl;

        // 解析权限
        std::string perms;
        if (info.protection & VM_PROT_READ) perms += "r";
        else perms += "-";
        if (info.protection & VM_PROT_WRITE) perms += "w";
        else perms += "-";
        if (info.protection & VM_PROT_EXECUTE) perms += "x";
        else perms += "-";
        std::cout << "  权限: " << perms << std::endl;

        // 判断内存类型
        std::string type;
        if (info.reserved) {
            type = "保留";
        } else if (info.shared) {
            type = "共享";
        } else {
            type = "私有";
        }
        std::cout << "  类型: " << type << std::endl;
    } else {
        std::cout << "  (无法获取内存区域信息)" << std::endl;
    }

#else
    std::cout << "  (不支持的操作系统)" << std::endl;
#endif
}

// 函数：测试栈内存
void test_stack_memory() {
    std::cout << "\n=== 栈内存测试 ===" << std::endl;

    // 栈上的局部变量
    int stack_var = 10;
    int stack_array[10];
    char stack_buffer[100];

    print_memory_info("局部变量 stack_var", &stack_var);
    analyze_memory_region(&stack_var);

    print_memory_info("局部数组 stack_array", stack_array);
    print_memory_info("局部缓冲区 stack_buffer", stack_buffer);

    // 栈上的大数组（可能导致栈溢出）
    // int large_stack_array[1000000]; // 取消注释可能导致栈溢出

    std::cout << "\n栈内存特点：" << std::endl;
    std::cout << "- 自动分配和释放（函数调用时分配，返回时释放）" << std::endl;
    std::cout << "- 分配速度快（只需移动栈指针）" << std::endl;
    std::cout << "- 大小有限（通常几MB）" << std::endl;
    std::cout << "- 地址从高向低增长" << std::endl;
}

// 函数：测试堆内存
void test_heap_memory() {
    std::cout << "\n=== 堆内存测试 ===" << std::endl;

    // 堆上的变量
    int* heap_var = new int(20);
    int* heap_array = new int[10];
    char* heap_buffer = new char[100];

    print_memory_info("堆变量 heap_var", heap_var);
    analyze_memory_region(heap_var);

    print_memory_info("堆数组 heap_array", heap_array);
    print_memory_info("堆缓冲区 heap_buffer", heap_buffer);

    // 堆上的大数组（堆可以分配更大的内存）
    int* large_heap_array = new int[1000000];
    print_memory_info("大堆数组 large_heap_array", large_heap_array);

    // 释放堆内存
    delete heap_var;
    delete[] heap_array;
    delete[] heap_buffer;
    delete[] large_heap_array;

    std::cout << "\n堆内存特点：" << std::endl;
    std::cout << "- 手动分配和释放（new/delete）" << std::endl;
    std::cout << "- 分配速度较慢（需要查找合适的空闲块）" << std::endl;
    std::cout << "- 大小较大（受限于虚拟内存）" << std::endl;
    std::cout << "- 地址从低向高增长" << std::endl;
    std::cout << "- 容易产生内存泄漏" << std::endl;
}

// 函数：测试全局/静态内存
void test_global_static_memory() {
    std::cout << "\n=== 全局/静态内存测试 ===" << std::endl;

    print_memory_info("全局变量 global_var", &global_var);
    analyze_memory_region(&global_var);

    print_memory_info("静态变量 static_var", &static_var);
    print_memory_info("常量全局变量 const_global_var", &const_global_var);
    print_memory_info("未初始化全局变量 uninitialized_global_var",
                      &uninitialized_global_var);

    // 函数内的静态变量
    static int function_static_var = 300;
    print_memory_info("函数内静态变量 function_static_var",
                      &function_static_var);

    std::cout << "\n全局/静态内存特点：" << std::endl;
    std::cout << "- 程序启动时分配，结束时释放" << std::endl;
    std::cout << "- .data 段：已初始化的全局/静态变量" << std::endl;
    std::cout << "- .bss 段：未初始化的全局/静态变量" << std::endl;
    std::cout << "- .rodata 段：只读数据（常量）" << std::endl;
}

// 普通函数
void normal_function() {
    std::cout << "这是普通函数" << std::endl;
}

// 静态函数（文件作用域）
static void static_function() {
    std::cout << "这是静态函数" << std::endl;
}

// 内联函数
inline void inline_function() {
    std::cout << "这是内联函数" << std::endl;
}

// 类成员函数
class TestClass {
public:
    void member_function() {
        std::cout << "这是成员函数" << std::endl;
    }

    static void static_member_function() {
        std::cout << "这是静态成员函数" << std::endl;
    }
};

// 函数：测试不同类型函数的地址
void test_function_addresses() {
    std::cout << "\n=== 函数地址测试 ===" << std::endl;

    // 普通函数地址
    std::cout << "普通函数地址:           " << (void*)&normal_function << std::endl;
    analyze_memory_region((void*)&normal_function);

    // 静态函数地址
    std::cout << "静态函数地址:           " << (void*)&static_function << std::endl;
    analyze_memory_region((void*)&static_function);

    // 内联函数地址
    std::cout << "内联函数地址:           " << (void*)&inline_function << std::endl;
    analyze_memory_region((void*)&inline_function);

    // Lambda 函数地址
    auto lambda = []() {
        std::cout << "这是 lambda 函数" << std::endl;
    };
    std::cout << "Lambda 函数地址:        " << (void*)&lambda << std::endl;
    analyze_memory_region((void*)&lambda);

    // 成员函数地址（需要特殊处理）
    std::cout << "成员函数地址:           (成员函数指针，不能直接转换为 void*)" << std::endl;
    // 注意：成员函数指针不能直接转换为 void*，需要特殊处理

    // 静态成员函数地址
    std::cout << "静态成员函数地址:       " << (void*)&TestClass::static_member_function << std::endl;
    analyze_memory_region((void*)&TestClass::static_member_function);

    // 函数指针
    void (*func_ptr)() = normal_function;
    std::cout << "函数指针地址:           " << (void*)&func_ptr << std::endl;
    std::cout << "函数指针指向的地址:     " << (void*)func_ptr << std::endl;

    std::cout << "\n函数地址特点分析：" << std::endl;
    std::cout << "1. 普通函数：" << std::endl;
    std::cout << "   - 存储在代码段（Text 段）" << std::endl;
    std::cout << "   - 地址在编译时确定" << std::endl;
    std::cout << "   - 可以通过函数指针调用" << std::endl;

    std::cout << "2. 静态函数：" << std::endl;
    std::cout << "   - 存储在代码段（Text 段）" << std::endl;
    std::cout << "   - 只在当前文件内可见" << std::endl;
    std::cout << "   - 不会与其他文件的函数冲突" << std::endl;

    std::cout << "3. 内联函数：" << std::endl;
    std::cout << "   - 通常不生成独立的函数代码" << std::endl;
    std::cout << "   - 代码直接嵌入调用点" << std::endl;
    std::cout << "   - 取地址时可能强制生成函数体" << std::endl;
    std::cout << "   - 优化级别影响是否真正内联" << std::endl;

    std::cout << "4. Lambda 函数：" << std::endl;
    std::cout << "   - 本质上是函数对象（functor）" << std::endl;
    std::cout << "   - 编译器生成一个唯一的类" << std::endl;
    std::cout << "   - 地址是函数对象的地址，不是函数代码地址" << std::endl;
    std::cout << "   - 可以捕获变量（存储在对象中）" << std::endl;

    std::cout << "5. 成员函数：" << std::endl;
    std::cout << "   - 存储在代码段（Text 段）" << std::endl;
    std::cout << "   - 需要 this 指针作为隐式参数" << std::endl;
    std::cout << "   - 静态成员函数不需要 this 指针" << std::endl;

    std::cout << "6. 函数指针：" << std::endl;
    std::cout << "   - 本身是存储在栈或堆上的变量" << std::endl;
    std::cout << "   - 指向代码段中的函数地址" << std::endl;
    std::cout << "   - 可以动态改变指向的函数" << std::endl;
}

// 函数：测试内存地址布局
void test_memory_layout() {
    std::cout << "\n=== 内存地址布局分析 ===" << std::endl;

    int stack_var;
    int* heap_var = new int;
    static int static_var;

    std::cout << "栈地址:     " << &stack_var << std::endl;
    std::cout << "堆地址:     " << heap_var << std::endl;
    std::cout << "静态变量地址: " << &static_var << std::endl;
    std::cout << "全局变量地址: " << &global_var << std::endl;
    std::cout << "代码段地址:  " << (void*)&test_memory_layout << std::endl;

    delete heap_var;

    std::cout << "\n典型内存布局（从高到低）：" << std::endl;
    std::cout << "1. 内核空间" << std::endl;
    std::cout << "2. 栈（向下增长）" << std::endl;
    std::cout << "3. （空闲空间）" << std::endl;
    std::cout << "4. 堆（向上增长）" << std::endl;
    std::cout << "5. BSS 段（未初始化数据）" << std::endl;
    std::cout << "6. Data 段（已初始化数据）" << std::endl;
    std::cout << "7. Text 段（代码）" << std::endl;
}

// 函数：测试栈溢出
void test_stack_overflow() {
    std::cout << "\n=== 栈溢出测试 ===" << std::endl;
    std::cout << "注意：以下代码可能导致程序崩溃，已注释" << std::endl;

    // 递归函数可能导致栈溢出
    /*
    void recursive_function(int depth) {
        int buffer[1000];  // 每次调用分配 4KB
        std::cout << "Depth: " << depth << ", Stack: 0x" << &buffer << std::endl;
        recursive_function(depth + 1);
    }
    recursive_function(0);
    */

    std::cout << "栈溢出原因：" << std::endl;
    std::cout << "- 深度递归" << std::endl;
    std::cout << "- 局部数组过大" << std::endl;
    std::cout << "- 解决方法：使用堆内存或迭代代替递归" << std::endl;
}

// 函数：测试内存泄漏
void test_memory_leak() {
    std::cout << "\n=== 内存泄漏测试 ===" << std::endl;
    std::cout << "注意：以下代码会产生内存泄漏，已注释" << std::endl;

    // 内存泄漏示例
    /*
    int* leaked = new int(42);
    // 忘记 delete leaked;
    */

    std::cout << "内存泄漏原因：" << std::endl;
    std::cout << "- 忘记释放堆内存" << std::endl;
    std::cout << "- 异常导致跳过 delete" << std::endl;
    std::cout << "- 解决方法：使用智能指针（unique_ptr, shared_ptr）" << std::endl;
}

// 主函数
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "C/C++ 底层原理：栈 vs 堆内存" << std::endl;
    std::cout << "========================================" << std::endl;

    test_stack_memory();
    test_heap_memory();
    test_global_static_memory();
    test_function_addresses();
    test_memory_layout();
    test_stack_overflow();
    test_memory_leak();

    std::cout << "\n========================================" << std::endl;
    std::cout << "测试完成" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}