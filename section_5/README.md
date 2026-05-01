# Section 5: 虚函数与多态（面向对象核心）

## 教学目标

深入理解 C++ 虚函数表的工作原理，掌握多态的实现机制，理解虚函数表的内存布局和调用过程。

## 测试文件

### 01_vtable.cpp
演示虚函数表的基本结构和工作原理，包括 vptr 的位置、虚函数覆盖、虚析构函数等

## 编译和运行

```bash
cd section_5

# 编译虚函数表程序
g++ -g -Wall -Wextra -std=c++17 01_vtable.cpp -o 01_vtable
./01_vtable

# 查看虚函数表符号
nm 01_vtable | grep vtable
```

## 核心概念

### 什么是虚函数表？

虚函数表（Virtual Table，简称 vtable）是 C++ 实现多态的关键机制。每个有虚函数的类都有一个虚函数表，表中存储了该类所有虚函数的地址。

### 虚函数表的基本结构

```
类对象内存布局：
┌─────────────────────────┐
│   vptr (虚函数表指针)    │ ← 指向虚函数表
├─────────────────────────┤
│   成员变量 1             │
├─────────────────────────┤
│   成员变量 2             │
├─────────────────────────┤
│   ...                    │
└─────────────────────────┘

虚函数表内存布局：
┌─────────────────────────┐
│   虚函数 1 地址          │
├─────────────────────────┤
│   虚函数 2 地址          │
├─────────────────────────┤
│   虚函数 3 地址          │
├─────────────────────────┤
│   ...                    │
└─────────────────────────┘
```

### vptr 的位置和作用

#### vptr 的位置

```cpp
class Base {
public:
    virtual void func1();
    virtual void func2();
    int data;
};

// 对象内存布局（大多数实现）：
// [vptr][data]
```

**重要特性**：
- vptr 通常位于对象的首地址
- 不同编译器的实现可能略有差异
- 但基本原理相同

#### vptr 的作用

```cpp
Base* ptr = new Derived();
ptr->func1();  // 虚函数调用

// 编译器生成的伪代码：
// 1. 获取对象的 vptr
// void* vptr = *(void**)ptr;
// 2. 在虚函数表中查找 func1 的地址
// void* func_addr = vtable[0];  // func1 在表中的索引
// 3. 调用函数
// ((void(*)())func_addr)();
```

### 虚函数表的内容

#### 基类的虚函数表

```cpp
class Base {
public:
    virtual void func1() { std::cout << "Base::func1()" << std::endl; }
    virtual void func2() { std::cout << "Base::func2()" << std::endl; }
    virtual ~Base() {}
};

// Base 的虚函数表：
// [0] Base::func1
// [1] Base::func2
// [2] Base::~Base
```

#### 派生类的虚函数表

```cpp
class Derived : public Base {
public:
    void func1() override { std::cout << "Derived::func1()" << std::endl; }
    void func3() { std::cout << "Derived::func3()" << std::endl; }
    ~Derived() override {}
};

// Derived 的虚函数表：
// [0] Derived::func1  (覆盖了 Base::func1)
// [1] Base::func2     (继承自 Base)
// [2] Derived::~Derived (覆盖了 Base::~Base)
// [3] Derived::func3  (新增的虚函数)
```

### 虚函数调用的过程

#### 静态绑定（普通函数）

```cpp
class Base {
public:
    void normal_func() { std::cout << "Base::normal_func()" << std::endl; }
};

Base* ptr = new Base();
ptr->normal_func();  // 直接调用，编译时确定

// 生成的汇编（简化）：
// call Base::normal_func
```

#### 动态绑定（虚函数）

```cpp
class Base {
public:
    virtual void virtual_func() { std::cout << "Base::virtual_func()" << std::endl; }
};

Base* ptr = new Derived();
ptr->virtual_func();  // 虚函数调用，运行时确定

// 生成的汇编（简化）：
// mov rax, [ptr]        ; 获取 vptr
// mov rax, [rax]        ; 获取虚函数表地址
// call [rax + offset]   ; 调用虚函数
```

### 单继承下的虚函数覆盖

#### 虚函数覆盖的机制

```cpp
class Base {
public:
    virtual void func() { std::cout << "Base::func()" << std::endl; }
};

class Derived : public Base {
public:
    void func() override { std::cout << "Derived::func()" << std::endl; }
};

// 虚函数表变化：
// Base 的虚函数表：[Base::func]
// Derived 的虚函数表：[Derived::func]  (覆盖了 Base::func)
```

#### 多态的实现

```cpp
Base* ptr = new Derived();
ptr->func();  // 调用 Derived::func()

// 实现过程：
// 1. ptr 指向 Derived 对象
// 2. Derived 对象的 vptr 指向 Derived 的虚函数表
// 3. 在 Derived 的虚函数表中查找 func
// 4. 找到 Derived::func，调用它
```

### 虚析构函数的作用

#### 为什么需要虚析构函数？

```cpp
class Base {
public:
    // virtual ~Base() {}  // 如果不是虚函数
    ~Base() { std::cout << "Base::~Base()" << std::endl; }
};

class Derived : public Base {
public:
    ~Derived() { std::cout << "Derived::~Derived()" << std::endl; }
};

Base* ptr = new Derived();
delete ptr;  // 只会调用 Base::~Base()，不会调用 Derived::~Derived()
```

#### 虚析构函数的正确使用

```cpp
class Base {
public:
    virtual ~Base() { std::cout << "Base::~Base()" << std::endl; }
};

class Derived : public Base {
public:
    ~Derived() override { std::cout << "Derived::~Derived()" << std::endl; }
};

Base* ptr = new Derived();
delete ptr;  // 先调用 Derived::~Derived()，再调用 Base::~Base()
```

**重要提示**：
- 如果基类析构函数不是虚函数，delete 基类指针时只会调用基类的析构函数
- 这会导致派生类的资源没有被正确释放
- 基类析构函数应该声明为虚函数

### 同一类型共享虚函数表

#### 虚函数表的共享机制

```cpp
Base obj1, obj2, obj3;

// obj1, obj2, obj3 的 vptr 都指向同一个虚函数表
// 虚函数表是类级别的，不是对象级别的
```

#### 内存节省

```cpp
// 如果每个对象都有自己的虚函数表：
// 1000 个对象 × 100 个虚函数 × 8 字节 = 800,000 字节

// 如果共享虚函数表：
// 1 个虚函数表 × 100 个虚函数 × 8 字节 = 800 字节
// 1000 个对象 × 1 个 vptr × 8 字节 = 8,000 字节
// 总计：8,800 字节
```

### 对象的内存布局

#### 单继承的对象布局

```cpp
class Base {
public:
    virtual void func1();
    virtual void func2();
    int base_data;
};

class Derived : public Base {
public:
    void func1() override;
    void func3();
    int derived_data;
};

// Base 对象布局：
// [vptr][base_data]

// Derived 对象布局：
// [vptr][base_data][derived_data]
```

#### 内存大小

```cpp
sizeof(Base);     // 16 bytes (8 bytes vptr + 4 bytes data + 4 bytes padding)
sizeof(Derived); // 16 bytes (8 bytes vptr + 4 bytes base_data + 4 bytes derived_data)
```

### 跨平台注意事项

#### 不同平台的 vptr 位置

| 平台 | vptr 位置 | 说明 |
|------|-----------|------|
| **大多数编译器** | 对象首地址 | vptr 是对象的第一个成员 |
| **某些编译器** | 对象末尾 | vptr 可能位于对象的末尾 |

#### 不同平台的虚函数表布局

| 平台 | 虚函数表顺序 | 说明 |
|------|-------------|------|
| **大多数编译器** | 声明顺序 | 按照虚函数声明的顺序排列 |
| **某些编译器** | 优化顺序 | 可能根据优化需求重新排列 |

#### 跨平台兼容性

```cpp
// 不要依赖 vptr 的具体位置
// 不要依赖虚函数表的具体布局
// 使用标准的多态机制，不要直接操作 vptr

// 错误的做法：
void* vptr = *(void**)obj;  // 平台相关

// 正确的做法：
obj->virtual_func();  // 使用标准的多态机制
```

## 程序说明

### 01_vtable.cpp 功能

1. **虚函数表结构演示**：展示 vptr 的位置和作用
2. **虚函数表内容演示**：展示虚函数表的内容和变化
3. **虚函数覆盖演示**：展示单继承下的虚函数覆盖
4. **虚析构函数演示**：展示虚析构函数的作用
5. **对象布局演示**：展示对象的内存布局
6. **虚函数表共享演示**：展示同一类型共享虚函数表

### 关键函数

```cpp
// 演示虚函数表的基本结构
void demonstrate_vtable_structure();

// 演示虚函数表的内容
void demonstrate_vtable_content();

// 演示单继承下的虚函数覆盖
void demonstrate_virtual_override();

// 演示虚析构函数的作用
void demonstrate_virtual_destructor();

// 演示对象的内存布局
void demonstrate_object_layout();

// 演示同一类型共享虚函数表
void demonstrate_shared_vtable();
```

## 实验观察

### 编译和反汇编

```bash
# 编译
g++ -g -O0 -std=c++17 01_vtable.cpp -o 01_vtable

# 反汇编（Linux/macOS）
objdump -d 01_vtable | less

# macOS 专用
otool -tV 01_vtable
```

### GDB 调试

```bash
gdb ./01_vtable
(gdb) break main
(gdb) run
(gdb) print base
(gdb) print *(void**)&base
(gdb) x/10x ((void**)&base)
```

### 查看虚函数表符号

```bash
# 查看虚函数表符号
nm 01_vtable | grep vtable

# 使用 c++filt 解码符号名
nm 01_vtable | c++filt | grep vtable
```

## 学习要点

1. **虚函数表结构**：理解虚函数表的基本结构和 vptr 的位置
2. **虚函数调用**：理解虚函数调用的过程和动态绑定机制
3. **虚函数覆盖**：理解派生类如何覆盖基类的虚函数
4. **虚析构函数**：理解虚析构函数的作用和重要性
5. **虚函数表共享**：理解同一类型共享虚函数表的机制
6. **对象布局**：理解对象的内存布局和大小计算

## 扩展阅读

- 《深度探索 C++ 对象模型》Chapter 3: 虚函数表
- 《Inside the C++ Object Model》Chapter 4: 虚函数机制
- cppreference.com: 虚函数
- 《Effective C++》Item 7: 为多态基类声明虚析构函数

## 实践建议

1. **反汇编观察**：使用 `objdump` 或 `otool` 查看虚函数调用的汇编代码
2. **GDB 调试**：使用 GDB 观察 vptr 和虚函数表的内容
3. **内存布局分析**：使用 `sizeof` 分析对象的内存布局
4. **虚函数表符号查看**：使用 `nm` 查看虚函数表符号
5. **跨平台对比**：在不同平台上编译和运行，观察差异

理解虚函数表是理解 C++ 多态机制的基础，对于深入理解面向对象编程和 C++ 底层原理都非常重要。