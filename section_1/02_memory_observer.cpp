/**
 * 实践项目 1：内存观察器
 * 目标：实现一个简单的内存观察工具，理解不同类型变量的内存布局
 *
 * 实现要求：
 * 1. 打印栈、堆、全局变量的地址范围
 * 2. 观察对象对齐和填充
 * 3. 验证引用的底层实现（使用反汇编）
 * 4. 检测当前系统的字节序
 */

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstring>

// 全局变量
int global_var = 42;
const int const_global_var = 100;

// 测试结构体（用于观察对齐和填充）
struct AlignedStruct {
    char c;      // 1 byte
    int i;       // 4 bytes
    short s;     // 2 bytes
    double d;    // 8 bytes
};

// 辅助函数：打印地址
void print_address(const std::string& name, const void* addr) {
    std::cout << std::left << std::setw(30) << name
              << ": " << addr << std::endl;
}

// 辅助函数：打印内存内容
void print_memory(const void* addr, size_t size) {
    const uint8_t* bytes = static_cast<const uint8_t*>(addr);
    std::cout << "  内存内容: ";
    for (size_t i = 0; i < size; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(bytes[i]) << " ";
    }
    std::cout << std::dec << std::setfill(' ') << std::endl;
}

// 1. 打印栈、堆、全局变量的地址范围
void test_address_ranges() {
    std::cout << "\n=== 1. 地址范围测试 ===" << std::endl;

    // 栈变量
    int stack_var = 10;
    char stack_buffer[100];

    // 堆变量
    int* heap_var = new int(20);
    char* heap_buffer = new char[100];

    // 静态变量
    static int static_var = 30;

    print_address("栈变量 stack_var", &stack_var);
    print_address("栈缓冲区 stack_buffer", stack_buffer);
    print_address("堆变量 heap_var", heap_var);
    print_address("堆缓冲区 heap_buffer", heap_buffer);
    print_address("静态变量 static_var", &static_var);
    print_address("全局变量 global_var", &global_var);
    print_address("常量全局变量 const_global_var", &const_global_var);

    // 清理堆内存
    delete heap_var;
    delete[] heap_buffer;

    std::cout << "\n地址范围分析：" << std::endl;
    std::cout << "- 栈地址通常最高（接近 0x7fff...）" << std::endl;
    std::cout << "- 堆地址在中等范围" << std::endl;
    std::cout << "- 全局/静态变量地址较低（接近 0x1000...）" << std::endl;
}

// 2. 观察对象对齐和填充
void test_alignment_and_padding() {
    std::cout << "\n=== 2. 对齐和填充测试 ===" << std::endl;

    AlignedStruct obj;
    obj.c = 'A';
    obj.i = 42;
    obj.s = 100;
    obj.d = 3.14;

    std::cout << "结构体大小: " << sizeof(AlignedStruct) << " bytes" << std::endl;
    std::cout << "各成员大小：" << std::endl;
    std::cout << "  char c:  " << sizeof(char) << " byte" << std::endl;
    std::cout << "  int i:   " << sizeof(int) << " bytes" << std::endl;
    std::cout << "  short s: " << sizeof(short) << " bytes" << std::endl;
    std::cout << "  double d:" << sizeof(double) << " bytes" << std::endl;

    std::cout << "\n各成员地址：" << std::endl;
    print_address("  &obj.c", &obj.c);
    print_address("  &obj.i", &obj.i);
    print_address("  &obj.s", &obj.s);
    print_address("  &obj.d", &obj.d);

    std::cout << "\n地址差值（填充分析）：" << std::endl;
    std::cout << "  &obj.i - &obj.c = " << (reinterpret_cast<char*>(&obj.i) - reinterpret_cast<char*>(&obj.c)) << " bytes" << std::endl;
    std::cout << "  &obj.s - &obj.i = " << (reinterpret_cast<char*>(&obj.s) - reinterpret_cast<char*>(&obj.i)) << " bytes" << std::endl;
    std::cout << "  &obj.d - &obj.s = " << (reinterpret_cast<char*>(&obj.d) - reinterpret_cast<char*>(&obj.s)) << " bytes" << std::endl;

    std::cout << "\n对齐和填充分析：" << std::endl;
    std::cout << "- char 后有 3 bytes 填充（为了对齐 int）" << std::endl;
    std::cout << "- short 后有 6 bytes 填充（为了对齐 double）" << std::endl;
    std::cout << "- 总大小: 1 + 3 + 4 + 2 + 6 + 8 = 24 bytes" << std::endl;

    // 优化成员顺序
    struct OptimizedStruct {
        double d;    // 8 bytes
        int i;       // 4 bytes
        short s;     // 2 bytes
        char c;      // 1 byte
        // 1 byte padding
    };

    OptimizedStruct opt;
    opt.d = 3.14;
    opt.i = 42;
    opt.s = 100;
    opt.c = 'A';

    std::cout << "\n优化后的结构体大小: " << sizeof(OptimizedStruct) << " bytes" << std::endl;
    std::cout << "节省空间: " << (sizeof(AlignedStruct) - sizeof(OptimizedStruct)) << " bytes" << std::endl;
}

// 3. 验证引用的底层实现（使用反汇编）
void test_reference_implementation() {
    std::cout << "\n=== 3. 引用底层实现测试 ===" << std::endl;

    int value = 42;
    int& ref = value;
    int* ptr = &value;

    std::cout << "原始值 value: " << value << std::endl;
    std::cout << "引用 ref: " << ref << std::endl;
    std::cout << "指针 ptr: " << *ptr << std::endl;

    std::cout << "\n地址信息：" << std::endl;
    print_address("&value", &value);
    print_address("&ref", &ref);
    print_address("ptr", &ptr);
    print_address("ptr指向的地址", ptr);

    std::cout << "\n引用和指针的地址关系：" << std::endl;
    std::cout << "&value == &ref: " << (&value == &ref ? "true" : "false") << std::endl;
    std::cout << "&value == ptr: " << (&value == ptr ? "true" : "false") << std::endl;

    std::cout << "\n引用的底层实现分析：" << std::endl;
    std::cout << "- 引用在底层通常实现为常量指针" << std::endl;
    std::cout << "- &ref 和 &value 的地址相同" << std::endl;
    std::cout << "- 引用本身不占用额外空间（编译时优化）" << std::endl;
    std::cout << "- 引用一旦初始化就不能改变指向" << std::endl;

    // 修改值
    ref = 100;
    std::cout << "\n通过引用修改后：" << std::endl;
    std::cout << "value: " << value << std::endl;
    std::cout << "ref: " << ref << std::endl;
    std::cout << "*ptr: " << *ptr << std::endl;

    std::cout << "\n反汇编验证说明：" << std::endl;
    std::cout << "要验证引用的底层实现，可以使用以下命令：" << std::endl;
    std::cout << "  g++ -g -O0 -S 02_memory_observer.cpp -o 02_memory_observer.s" << std::endl;
    std::cout << "  cat 02_memory_observer.s | grep -A 10 'reference_assembly_test'" << std::endl;
    std::cout << "\n在汇编代码中，你会看到：" << std::endl;
    std::cout << "- 引用和指针使用相同的指令（lea, mov）" << std::endl;
    std::cout << "- 引用本质上就是指针的语法糖" << std::endl;
    std::cout << "- 编译器在编译时将引用替换为指针操作" << std::endl;
}

// 4. 检测当前系统的字节序
void test_endianness() {
    std::cout << "\n=== 4. 字节序测试 ===" << std::endl;

    // 方法1：使用 union
    union {
        uint32_t i;
        char c[4];
    } test;

    test.i = 0x12345678;

    std::cout << "方法1：使用 union" << std::endl;
    std::cout << "  整数值: 0x" << std::hex << test.i << std::dec << std::endl;
    std::cout << "  字节顺序: ";
    for (int i = 0; i < 4; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(test.c[i]) << " ";
    }
    std::cout << std::dec << std::setfill(' ') << std::endl;

    if (test.c[0] == 0x78) {
        std::cout << "  系统字节序: Little Endian（小端）" << std::endl;
    } else if (test.c[0] == 0x12) {
        std::cout << "  系统字节序: Big Endian（大端）" << std::endl;
    } else {
        std::cout << "  系统字节序: 未知" << std::endl;
    }

    // 方法2：使用指针
    uint32_t num = 0x12345678;
    uint8_t* bytes = reinterpret_cast<uint8_t*>(&num);

    std::cout << "\n方法2：使用指针" << std::endl;
    std::cout << "  整数值: 0x" << std::hex << num << std::dec << std::endl;
    std::cout << "  字节顺序: ";
    for (int i = 0; i < 4; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(bytes[i]) << " ";
    }
    std::cout << std::dec << std::setfill(' ') << std::endl;

    std::cout << "\n字节序说明：" << std::endl;
    std::cout << "- Little Endian：低位字节存储在低地址（0x78 在地址 0）" << std::endl;
    std::cout << "- Big Endian：高位字节存储在低地址（0x12 在地址 0）" << std::endl;
    std::cout << "- x86 和 x86-64 架构通常是 Little Endian" << std::endl;
    std::cout << "- 网络传输通常使用 Big Endian" << std::endl;
}

// 主函数
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "实践项目 1：内存观察器" << std::endl;
    std::cout << "========================================" << std::endl;

    test_address_ranges();
    test_alignment_and_padding();
    test_reference_implementation();
    test_endianness();

    std::cout << "\n========================================" << std::endl;
    std::cout << "测试完成" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}