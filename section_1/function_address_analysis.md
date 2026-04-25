# 函数地址分析

## 测试结果

### 不同类型函数的地址

```
=== 函数地址测试 ===
普通函数地址:           0x102e4d6b4
  区域: 0x102e4c000 - 0x102e50000
  大小: 16384 bytes
  权限: r-x
  类型: 私有

静态函数地址:           0x102e4db00
  区域: 0x102e4c000 - 0x102e50000
  大小: 16384 bytes
  权限: r-x
  类型: 私有

内联函数地址:           0x102e4db30
  区域: 0x102e4c000 - 0x102e50000
  大小: 16384 bytes
  权限: r-x
  类型: 私有

Lambda 函数地址:        0x16cfb2aaf
  区域: 0x16c7b8000 - 0x16cfb4000
  大小: 8372224 bytes
  权限: rw-
  类型: 私有

成员函数地址:           (成员函数指针，不能直接转换为 void*)

静态成员函数地址:       0x102e4db60
  区域: 0x102e4c000 - 0x102e50000
  大小: 16384 bytes
  权限: r-x
  类型: 私有

函数指针地址:           0x16cfb2aa0
函数指针指向的地址:     0x102e4d6b4
```

## 详细分析

### 1. 普通函数

**地址**: `0x102e4d6b4`
**存储位置**: 代码段（Text 段）
**权限**: `r-x`（可读可执行）

**特点**:
- 存储在代码段，地址在编译时确定
- 可以通过函数指针调用
- 在整个程序中全局可见
- 符号表中显示为全局符号（大写 T）

**示例代码**:
```cpp
void normal_function() {
    std::cout << "这是普通函数" << std::endl;
}

// 获取地址
void* addr = (void*)&normal_function;
```

### 2. 静态函数

**地址**: `0x102e4db00`
**存储位置**: 代码段（Text 段）
**权限**: `r-x`（可读可执行）

**特点**:
- 存储在代码段，与普通函数相同
- 只在当前文件内可见（文件作用域）
- 不会与其他文件的函数冲突
- 符号表中显示为局部符号（小写 t）

**示例代码**:
```cpp
static void static_function() {
    std::cout << "这是静态函数" << std::endl;
}

// 获取地址
void* addr = (void*)&static_function;
```

**与普通函数的区别**:
- **链接可见性**: 静态函数只在当前翻译单元（.cpp 文件）内可见
- **符号表**: 静态函数在符号表中是局部符号，普通函数是全局符号
- **命名冲突**: 静态函数不会与其他文件的同名函数冲突

### 3. 内联函数

**地址**: `0x102e4db30`
**存储位置**: 代码段（Text 段）
**权限**: `r-x`（可读可执行）

**特点**:
- 通常不生成独立的函数代码，而是直接嵌入调用点
- 当取函数地址时，编译器会强制生成函数体
- 优化级别（-O0 vs -O2）影响是否真正内联
- 内联只是建议，编译器可以忽略

**示例代码**:
```cpp
inline void inline_function() {
    std::cout << "这是内联函数" << std::endl;
}

// 获取地址（强制生成函数体）
void* addr = (void*)&inline_function;
```

**内联函数的行为**:
- **-O0（无优化）**: 可能不会内联，生成独立函数
- **-O2（优化）**: 可能内联，不生成独立函数
- **取地址**: 强制生成函数体，即使标记为 inline

**汇编对比**:
```bash
# -O0 编译
g++ -O0 -S test.cpp

# -O2 编译
g++ -O2 -S test.cpp

# 对比汇编代码，观察是否有 call 指令
```

### 4. Lambda 函数

**地址**: `0x16cfb2aaf`
**存储位置**: 栈
**权限**: `rw-`（可读可写）

**特点**:
- 本质上是函数对象（functor）
- 编译器生成一个唯一的类
- 地址是函数对象的地址，不是函数代码地址
- 可以捕获变量（存储在对象中）
- 存储在栈上，生命周期与作用域相同

**示例代码**:
```cpp
auto lambda = []() {
    std::cout << "这是 lambda 函数" << std::endl;
};

// 获取地址（函数对象的地址）
void* addr = (void*)&lambda;

// 调用 lambda
lambda();
```

**Lambda 的底层实现**:
```cpp
// 编译器生成的类似代码
class LambdaClass {
public:
    void operator()() const {
        std::cout << "这是 lambda 函数" << std::endl;
    }
};

LambdaClass lambda;  // lambda 对象存储在栈上
```

**带捕获的 Lambda**:
```cpp
int x = 42;
auto lambda = [x]() {
    std::cout << "捕获的值: " << x << std::endl;
};

// x 的值存储在 lambda 对象中
// sizeof(lambda) >= sizeof(int)
```

### 5. 成员函数

**地址**: 不能直接转换为 `void*`

**存储位置**: 代码段（Text 段）
**权限**: `r-x`（可读可执行）

**特点**:
- 存储在代码段
- 需要 `this` 指针作为隐式参数
- 成员函数指针不能直接转换为 `void*`
- 静态成员函数不需要 `this` 指针

**示例代码**:
```cpp
class TestClass {
public:
    void member_function() {
        std::cout << "这是成员函数" << std::endl;
    }

    static void static_member_function() {
        std::cout << "这是静态成员函数" << std::endl;
    }
};

// 成员函数指针（不能转换为 void*）
void (TestClass::*member_ptr)() = &TestClass::member_function;

// 静态成员函数指针（可以转换为 void*）
void* static_member_ptr = (void*)&TestClass::static_member_function;

// 调用成员函数
TestClass obj;
(obj.*member_ptr)();  // 需要对象实例
```

**成员函数指针的特殊性**:
- **大小**: 成员函数指针可能比普通指针大（需要存储偏移量）
- **多继承**: 可能需要调整 `this` 指针
- **虚函数**: 可能通过虚表调用

**成员函数指针的大小**:
```cpp
std::cout << "普通指针大小: " << sizeof(void*) << std::endl;
std::cout << "成员函数指针大小: " << sizeof(void (TestClass::*)()) << std::endl;
```

### 6. 静态成员函数

**地址**: `0x102e4db60`
**存储位置**: 代码段（Text 段）
**权限**: `r-x`（可读可执行）

**特点**:
- 存储在代码段
- 不需要 `this` 指针
- 可以转换为 `void*`
- 行为与普通函数相同

**示例代码**:
```cpp
class TestClass {
public:
    static void static_member_function() {
        std::cout << "这是静态成员函数" << std::endl;
    }
};

// 获取地址
void* addr = (void*)&TestClass::static_member_function;

// 调用
TestClass::static_member_function();
```

### 7. 函数指针

**地址**: `0x16cfb2aa0`（指针本身）
**指向地址**: `0x102e4d6b4`（指向的函数）
**存储位置**: 栈
**权限**: `rw-`（可读可写）

**特点**:
- 本身是存储在栈或堆上的变量
- 指向代码段中的函数地址
- 可以动态改变指向的函数
- 支持回调函数和函数表

**示例代码**:
```cpp
void normal_function() {
    std::cout << "这是普通函数" << std::endl;
}

// 函数指针
void (*func_ptr)() = normal_function;

// 获取函数指针的地址
void* ptr_addr = (void*)&func_ptr;

// 获取函数指针指向的地址
void* func_addr = (void*)func_ptr;

// 调用
func_ptr();

// 改变指向
func_ptr = another_function;
```

**函数指针的应用**:
- **回调函数**: 传递函数作为参数
- **函数表**: 实现多态
- **状态机**: 根据状态调用不同函数
- **插件系统**: 动态加载函数

## 内存布局总结

### 代码段（Text 段）
- **内容**: 函数代码
- **权限**: `r-x`（可读可执行）
- **函数类型**:
  - 普通函数
  - 静态函数
  - 内联函数（取地址时）
  - 成员函数
  - 静态成员函数

### 栈
- **内容**: 局部变量、函数对象、函数指针
- **权限**: `rw-`（可读可写）
- **函数类型**:
  - Lambda 函数对象
  - 函数指针变量

### 堆
- **内容**: 动态分配的对象
- **权限**: `rw-`（可读可写）
- **函数类型**:
  - 动态分配的 Lambda 对象
  - 动态分配的函数指针

## nm 命令分析

### 查看函数符号
```bash
nm -C 01_stack_vs_heap | grep "function"
```

### 预期输出
```
0000000100001bb0 T normal_function()
0000000100001bb0 t static_function()
0000000100001bb0 T inline_function()
0000000100001bb0 T TestClass::static_member_function()
```

### 符号类型说明
- **T**: 全局函数符号
- **t**: 局部函数符号（静态函数）
- **U**: 未定义符号（外部依赖）

## 实用技巧

### 1. 检查函数是否真正内联
```bash
# -O0 编译
g++ -O0 -S test.cpp -o test_O0.s

# -O2 编译
g++ -O2 -S test.cpp -o test_O2.s

# 对比汇编代码
diff test_O0.s test_O2.s
```

### 2. 查看 Lambda 的底层实现
```bash
# 使用 clang 查看生成的 AST
clang -Xclang -ast-dump test.cpp

# 查看编译器生成的类名
nm -C a.out | grep lambda
```

### 3. 分析成员函数指针
```cpp
#include <iostream>

class Base {
public:
    virtual void func() {}
};

class Derived : public Base {
public:
    void func() override {}
};

int main() {
    std::cout << "成员函数指针大小: " << sizeof(void (Base::*)()) << std::endl;
    std::cout << "普通指针大小: " << sizeof(void*) << std::endl;
    return 0;
}
```

## 常见问题

### Q1: 为什么成员函数指针不能转换为 void*?
**A**: 成员函数指针可能包含额外的信息（如 this 指针调整、虚表偏移等），不能简单地转换为普通指针。

### Q2: Lambda 函数的地址为什么在栈上?
**A**: Lambda 本质上是函数对象，存储在栈上。其调用运算符的代码在代码段。

### Q3: 内联函数取地址后还会内联吗?
**A**: 取地址会强制生成函数体，但其他调用点可能仍然内联。

### Q4: 静态函数和普通函数在性能上有区别吗?
**A**: 没有性能区别，区别只在链接时的可见性。

### Q5: 如何判断函数是否真正内联?
**A**: 查看汇编代码，如果没有 call 指令，则说明内联成功。

## 总结

不同类型函数的地址和存储位置：

| 函数类型 | 存储位置 | 权限 | 可转换为 void* |
|---------|---------|------|---------------|
| 普通函数 | 代码段 | r-x | ✓ |
| 静态函数 | 代码段 | r-x | ✓ |
| 内联函数 | 代码段 | r-x | ✓ |
| Lambda 函数对象 | 栈 | rw- | ✓ |
| 成员函数 | 代码段 | r-x | ✗ |
| 静态成员函数 | 代码段 | r-x | ✓ |
| 函数指针 | 栈/堆 | rw- | ✓ |

理解这些概念有助于深入理解 C++ 的底层实现和优化机制。