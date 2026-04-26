# C/C++ 底层原理学习清单

---

## 第一层：内存与数据表示（基石）

| 优先级 | 知识点 | 核心问题 | 验证手段 |
|--------|--------|----------|----------|
| ⭐⭐⭐ | 栈 vs 堆内存 | 分配/释放开销、生命周期、大小限制 | 打印地址看区间，`/proc/self/maps` |
| ⭐⭐⭐ | 指针与引用的底层 | 引用真的不占空间吗？ | 反汇编：`lea` vs 直接操作 |
| ⭐⭐ | 全局/静态变量存储 | `.data` / `.bss` / `.rodata` 的区别 | `readelf -S`，观察未初始化变量 |
| ⭐⭐ | 对象对齐与填充 | `sizeof` 为何比成员和大？ | 修改成员顺序，打印地址差值 |
| ⭐ | 字节序（大端/小端） | 多字节整数在内存中的排列 | `union { int i; char c[4]; }` |
| ⭐ | `void*` 与函数指针 | 函数指针如何调用 | 汇编 `call *(%rax)` |

### 🛠️ 实践项目 1：内存观察器
**目标**：实现一个简单的内存观察工具，理解不同类型变量的内存布局

```cpp
// 实现要求：
// 1. 打印栈、堆、全局变量的地址范围
// 2. 观察对象对齐和填充
// 3. 验证引用的底层实现
// 4. 检测当前系统的字节序

// 提示：使用 reinterpret_cast 和 uintptr_t
```

---

## 第二层：对象生命周期（C++ 核心）

| 优先级 | 知识点 | 核心问题 | 验证手段 |
|--------|--------|----------|----------|
| ⭐⭐⭐ | 构造/析构顺序 | 成员、基类、派生类的顺序 | 打印日志 + 反汇编看隐式调用 |
| ⭐⭐⭐ | 拷贝 elision（RVO/NRVO） | 函数返回对象为何"无拷贝"？ | `-O0` vs `-O2` 汇编对比 |
| ⭐⭐⭐ | 临时对象的产生与销毁 | `A a = 10;` 构造几次？ | `-fno-elide-constructors` 对比 |
| ⭐⭐ | 零/默认/值初始化 | `int a;` `int a{}` `int a=int()` 差异 | 汇编是否产生 `movl $0` |

### 🛠️ 实践项目 2：对象生命周期追踪器
**目标**：实现一个自动追踪对象生命周期的基类

```cpp
// 实现要求：
// 1. 创建一个 TrackedObject 基类，自动打印构造/析构信息
// 2. 支持追踪拷贝构造、移动构造
// 3. 验证 RVO/NRVO 优化
// 4. 观察临时对象的生命周期

```

---

## 第三层：类型系统（陷阱高发区）

| 优先级 | 知识点 | 核心问题 | 验证手段 |
|--------|--------|----------|----------|
| ⭐⭐⭐ | trivially copyable | `memcpy` 安全的类型条件 | `std::is_trivially_copyable_v` + 汇编 |
| ⭐⭐⭐ | standard-layout | `reinterpret_cast` 到首成员的安全保证 | 对比有虚函数/不同访问控制 |
| ⭐⭐ | 类型别名（`typedef`/`using`） | 是否产生新类型？ | `typeid(T).name()` 对比 |
| ⭐ | POD（纯旧数据） | C++11 后为何被拆分 | 检查 `trivial` + `standard-layout` |
| ⭐ | 不完整类型 | 为何只能定义指针/引用？ | `struct T; T obj;` 编译报错 |

### 🛠️ 实践项目 3：类型特征检查器
**目标**：实现一个简单的类型特征检查工具

```cpp
// 实现要求：
// 1. 手动实现 is_trivially_copyable
// 2. 手动实现 is_standard_layout
// 3. 验证不同类型的特征
// 4. 理解类型别名的影响

// 提示：使用 std::is_* 特征和 static_assert
```

---

## 第四层：函数调用与栈帧（底层执行模型）

| 优先级 | 知识点 | 核心问题 | 验证手段 |
|--------|--------|----------|----------|
| ⭐⭐⭐ | 调用约定（x86-64） | 参数在哪些寄存器？超过6个怎么办？ | 反汇编：`mov edi` / `push` |
| ⭐⭐⭐ | 栈帧结构与返回地址 | `rbp`/`rsp` 的关系 | 观察 `leave` / `ret` |
| ⭐⭐ | 尾调用优化（TCO） | 递归可能不爆栈？ | `-O2` 下 `jmp` 而非 `call` |
| ⭐⭐ | 内联函数 | 真的"不调用"吗？ | 看有无 `call` 指令 |
| ⭐ | 默认参数 | 谁填充的？ | 反汇编：调用者压入默认值 |
| ⭐ | 函数重载与 name mangling | C++ 如何区分同名函数 | `nm` 查看符号名 |

### 🛠️ 实践项目 4：栈帧分析器
**目标**：实现一个简单的栈帧分析工具

```cpp
// 实现要求：
// 1. 打印当前函数的栈帧地址
// 2. 观察递归调用的栈帧变化
// 3. 验证尾调用优化
// 4. 分析不同调用约定的影响

// 提示：使用 __builtin_frame_address(0) 获取栈帧地址
```

---

## 第五层：虚函数与多态（面向对象核心）

| 优先级 | 知识点 | 核心问题 | 验证手段 |
|--------|--------|----------|----------|
| ⭐⭐⭐ | 虚函数表（vtable）布局 | 对象首地址存 vptr；同一类型共享 | 打印 `*(void**)obj`，跨对象对比 |
| ⭐⭐⭐ | 虚析构函数 | 为何基类析构函数要 virtual？ | 不用 virtual：`delete base_ptr` 只调基类 |
| ⭐⭐ | 单继承下的虚函数覆盖 | 子类如何替换父类虚函数地址 | 观察 vtable 中对应槽位变化 |
| ⭐⭐ | 多重继承与 this 调整 | 几个 vptr？子类转父类时指针偏移 | 打印地址，观察转换后的值 |
| ⭐ | 纯虚函数与抽象类 | vtable 中填 `__cxa_pure_virtual` | 构造函数中调用纯虚函数崩溃 |
| ⭐ | 虚继承 | 共享基类如何实现 | `-fdump-class-hierarchy` 查看布局 |

### 🛠️ 实践项目 5：虚函数表探索器
**目标**：手动探索虚函数表的内存布局

```cpp
// 实现要求：
// 1. 打印对象的虚函数表地址
// 2. 遍历虚函数表中的函数指针
// 3. 验证单继承和多继承的 vtable 布局
// 4. 观察虚析构函数的作用

// 提示：使用 reinterpret_cast 和函数指针类型
```

---

## 第六层：内存分配与智能指针（资源管理）

| 优先级 | 知识点 | 核心问题 | 验证手段 |
|--------|--------|----------|----------|
| ⭐⭐⭐ | `new`/`delete` 完整流程 | `operator new` + 构造；析构 + `operator delete` | 重载 `operator new/delete` 打印 |
| ⭐⭐⭐ | `std::unique_ptr` 零开销 | 与裸指针大小相同 | `sizeof` 对比 |
| ⭐⭐⭐ | `std::shared_ptr` 控制块 | `make_shared` 一次分配 | 自定义 `operator new` 看调用次数 |
| ⭐⭐ | `weak_ptr` 与循环引用 | `lock()` 为何线程安全？ | 打印强/弱引用计数 |
| ⭐ | `new[]`/`delete[]` 数组长度 | 编译器如何知道析构几次 | 观察分配大小比预期多几字节 |
| ⭐ | 分配器（allocator） | `vector` 如何自定义内存策略 | 实现打印日志的分配器 |

### 🛠️ 实践项目 6：手动实现智能指针
**目标**：手动实现简单的智能指针，理解 RAII 和引用计数

```cpp
// 实现要求：
// 1. 实现 SimpleUniquePtr（独占所有权）
// 2. 实现 SimpleSharedPtr（共享所有权，引用计数）
// 3. 实现 SimpleWeakPtr（弱引用）
// 4. 验证零开销抽象

// 提示：使用 delete 和 delete[]，注意异常安全
```

---

## 第七层：移动语义与完美转发（现代 C++ 核心）

| 优先级 | 知识点 | 核心问题 | 验证手段 |
|--------|--------|----------|----------|
| ⭐⭐⭐ | 左值/右值/将亡值 | 地址性 vs 可移动性 | `decltype((x))` + 反汇编 |
| ⭐⭐⭐ | `std::move` 的实质 | 只是 `static_cast<T&&>` | 对比有无 `move` 的汇编（无差异） |
| ⭐⭐⭐ | 移动构造与 `noexcept` | 为何要 `noexcept`？ | `vector` 扩容看调用拷贝还是移动 |
| ⭐⭐ | 引用折叠 | `T& &&` 是什么 | 模板推导后的实际类型 |
| ⭐⭐ | 完美转发 `std::forward` | 如何保持值类别 | 手写简化版 `forward` |
| ⭐ | 万能引用（universal reference） | `T&&` 在模板中的双重含义 | 传入左值和右值看实例化 |

### 🛠️ 实践项目 7：移动语义实验
**目标**：实现一个支持移动语义的容器类

```cpp
// 实现要求：
// 1. 实现 SimpleVector（动态数组）
// 2. 支持移动构造和移动赋值
// 3. 验证 noexcept 对性能的影响
// 4. 实现完美转发的 emplace_back

// 提示：使用 std::move 和 std::forward，注意异常安全
```

---

## 第八层：异常与运行时机制

| 优先级 | 知识点 | 核心问题 | 验证手段 |
|--------|--------|----------|----------|
| ⭐⭐ | 异常栈展开（unwinding） | `throw` 到 `catch` 如何析构局部对象 | 带日志析构 + `throw` 观察 |
| ⭐⭐ | `noexcept` 承诺 | 违反导致 `std::terminate` | 标记 `noexcept` 但抛出异常 |
| ⭐ | 异常开销 | 无异常抛出的代码是否变慢？ | 对比有/无 `try` 的汇编 |
| ⭐ | 析构函数不应抛异常 | 若抛异常导致 `std::terminate` | 析构中 `throw` 触发 |
| ⭐ | RTTI（运行时类型识别） | `typeid`/`dynamic_cast` 依赖虚表 | 查看 vtable 中的 `type_info` |

### 🛠️ 实践项目 8：异常安全观察器
**目标**：实现一个观察异常行为的工具类

```cpp
// 实现要求：
// 1. 创建 ExceptionTracked 类，追踪异常抛出
// 2. 验证栈展开时的析构顺序
// 3. 测试 noexcept 的作用
// 4. 观察 RTTI 的开销

// 提示：使用 std::exception 和 std::current_exception
```

---

## 第九层：C 与 C++ 混编

| 优先级 | 知识点 | 核心问题 | 验证手段 |
|--------|--------|----------|----------|
| ⭐⭐ | `extern "C"` 的作用 | 关闭 name mangling | `nm` 对比符号名 |
| ⭐ | C 头文件封装 | `#ifdef __cplusplus extern "C" {` | 实现 C 库供 C++ 调用 |
| ⭐ | 回调函数中的 `this` 问题 | C 回调如何调用 C++ 成员函数 | 静态函数 + 用户参数传入 `this` |

### 🛠️ 实践项目 9：C/C++ 混编示例
**目标**：实现一个 C 库并提供 C++ 封装

```cpp
// 实现要求：
// 1. 用 C 实现一个简单的数据结构（如链表）
// 2. 用 extern "C" 导出接口
// 3. 在 C++ 中封装成 RAII 类
// 4. 实现回调函数支持

// 提示：注意 C 和 C++ 的类型兼容性
```

---

## 第十层：经典未定义行为（UB 实验）

| 优先级 | 知识点 | 核心问题 | 验证手段 |
|--------|--------|----------|----------|
| ⭐⭐⭐ | 有符号整数溢出 | UB，不是回绕 | `-O2` 下可能被优化掉分支 |
| ⭐⭐⭐ | 空指针解引用 | 一定会崩溃？不一定 | 大多数 OS 段错误 |
| ⭐⭐ | 悬挂引用（dangling reference） | 返回局部变量引用 | 编译器警告 + 运行时诡异值 |
| ⭐⭐ | 修改字符串字面量 | `char* s = "hello"; s[0]='H'` | 常量区 `.rodata`，写崩溃 |
| ⭐ | 越界访问 | 为什么有时不崩？ | 栈布局：破坏相邻变量 |
| ⭐ | 违规别名（strict aliasing） | 不同类型指针指向同一内存 | `-fno-strict-aliasing` 改变行为 |

### 🛠️ 实践项目 10：UB 检测器
**目标**：实现一个简单的 UB 检测工具

```cpp
// 实现要求：
// 1. 实现有符号整数溢出检测
// 2. 实现空指针检查
// 3. 实现悬挂引用检测（使用标记值）
// 4. 理解不同优化级别下的 UB 行为

// 提示：使用 __builtin_add_overflow 等内置函数
```

---

## 学习路线建议

### 第一阶段（第 1-2 周）：打好底层基础
- **第一层**：内存与数据表示 + 实践项目 1
- **第二层**：对象生命周期 + 实践项目 2
- **第四层**：函数调用与栈帧 + 实践项目 4

**阶段目标**：理解 C++ 程序的内存模型和执行模型

### 第二阶段（第 2-4 周）：掌握 C++ 核心机制
- **第三层**：类型系统 + 实践项目 3
- **第五层**：虚函数与多态 + 实践项目 5
- **第六层**：内存分配与智能指针 + 实践项目 6
- **第七层**：移动语义与完美转发 + 实践项目 7

**阶段目标**：掌握 C++ 的面向对象特性和现代 C++ 核心机制

### 第三阶段（第 5-6 周）：进阶与避坑
- **第八层**：异常与运行时机制 + 实践项目 8
- **第九层**：C 与 C++ 混编 + 实践项目 9
- **第十层**：经典未定义行为 + 实践项目 10

**阶段目标**：理解异常处理、混编场景和常见 UB 陷阱

---

## 实验通用命令

```bash
# 生成汇编（推荐 -O0 看完整逻辑）
g++ -g -O0 -fno-elide-constructors test.cpp -S -o test.s

# 带优化对比
g++ -g -O2 test.cpp -S -o test_O2.s

# 查看符号表
nm test | c++filt

# 查看段布局
readelf -S test

# GDB 汇编级调试
gdb ./a.out
(gdb) disas /m
(gdb) stepi
(gdb) info registers

# 查看类层次结构
g++ -fdump-class-hierarchy test.cpp

# 开启所有警告
g++ -Wall -Wextra -pedantic test.cpp

# 检测未定义行为（需要安装 UBSan）
g++ -fsanitize=undefined test.cpp
```

---

## 实践项目提示

### 项目 1：内存观察器
```cpp
#include <iostream>
#include <cstdint>

class MemoryObserver {
public:
    static void print_stack_info() {
        int stack_var;
        std::cout << "Stack address: " << &stack_var << std::endl;
    }

    static void print_heap_info() {
        int* heap_var = new int;
        std::cout << "Heap address: " << heap_var << std::endl;
        delete heap_var;
    }

    static void check_endian() {
        uint32_t num = 0x12345678;
        uint8_t* bytes = reinterpret_cast<uint8_t*>(&num);
        if (bytes[0] == 0x78) {
            std::cout << "Little endian" << std::endl;
        } else {
            std::cout << "Big endian" << std::endl;
        }
    }
};
```

### 项目 2：对象生命周期追踪器
```cpp
#include <iostream>

class TrackedObject {
public:
    TrackedObject() {
        std::cout << "Construct: " << __PRETTY_FUNCTION__ << std::endl;
    }

    TrackedObject(const TrackedObject&) {
        std::cout << "Copy construct: " << __PRETTY_FUNCTION__ << std::endl;
    }

    TrackedObject(TrackedObject&&) noexcept {
        std::cout << "Move construct: " << __PRETTY_FUNCTION__ << std::endl;
    }

    ~TrackedObject() {
        std::cout << "Destruct: " << __PRETTY_FUNCTION__ << std::endl;
    }
};
```

### 项目 6：手动实现智能指针
```cpp
template<typename T>
class SimpleUniquePtr {
private:
    T* ptr;

public:
    explicit SimpleUniquePtr(T* p = nullptr) : ptr(p) {}

    ~SimpleUniquePtr() {
        delete ptr;
    }

    // 禁止拷贝
    SimpleUniquePtr(const SimpleUniquePtr&) = delete;
    SimpleUniquePtr& operator=(const SimpleUniquePtr&) = delete;

    // 支持移动
    SimpleUniquePtr(SimpleUniquePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    SimpleUniquePtr& operator=(SimpleUniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    T* get() const { return ptr; }
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
};
```

---

## 学习建议

1. **每个知识点都要动手验证**：不要只看理论，一定要写代码验证
2. **对比不同优化级别**：`-O0` 和 `-O2` 的汇编差异很大
3. **使用工具辅助**：GDB、nm、readelf 等工具是理解底层的好帮手
4. **关注编译器警告**：`-Wall -Wextra` 能发现很多潜在问题
5. **理解"为什么"**：不仅要知道"是什么"，更要理解"为什么这样设计"
6. **实践项目循序渐进**：先完成基本功能，再逐步完善

---

## 推荐资源

- **书籍**：《深度探索 C++ 对象模型》、《Inside the C++ Object Model》
- **在线资源**：cppreference.com、Compiler Explorer (godbolt.org)
- **工具**：GDB、Valgrind、AddressSanitizer、UBSanitizer
- **编译器**：GCC、Clang（都支持丰富的调试和诊断功能）