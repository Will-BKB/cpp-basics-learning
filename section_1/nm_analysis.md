# nm 命令分析报告

## 编译命令
```bash
g++ -g -Wall -Wextra -std=c++17 section_1/01_stack_vs_heap.cpp -o 01_stack_vs_heap
```

## nm 命令基础

nm 命令用于显示目标文件中的符号表信息。基本格式：
```
地址 类型 符号名
```

## 符号类型说明

### 大写字母（外部符号）
- **T**：Text 段，代码段中的函数（全局可见）
- **D**：Data 段，已初始化的全局变量（全局可见）
- **B**：BSS 段，未初始化的全局变量（全局可见）
- **S**：其他段中的全局符号

### 小写字母（局部符号）
- **t**：Text 段，代码段中的函数（文件作用域）
- **d**：Data 段，已初始化的静态变量（文件作用域）
- **b**：BSS 段，未初始化的静态变量（文件作用域）
- **s**：其他段中的静态符号

### 其他符号类型
- **U**：Undefined，未定义的符号（需要从其他文件链接）
- **u**：唯一的全局符号
- **w**：Weak symbol，弱符号
- **C**：Common symbol，未初始化的公共变量
- **?**：未知符号类型

## 分析结果

### 1. 全局变量（外部可见）
```
0000000100008000 D _global_var
000000010000800c S _uninitialized_global_var
```

- `_global_var`：已初始化的全局变量，存储在 Data 段
- `_uninitialized_global_var`：未初始化的全局变量，存储在 BSS 段
- 前缀 `_` 是 macOS 的命名约定

### 2. 静态变量（文件作用域）
```
0000000100008004 d static_var
0000000100003e64 s const_global_var
```

- `static_var`：文件作用域的静态变量，存储在 Data 段
- `const_global_var`：常量全局变量，存储在只读段
- 小写字母表示这些符号只在当前文件内可见

### 3. 函数内静态变量
```
0000000100008010 b test_memory_layout()::static_var
0000000100008008 d test_global_static_memory()::function_static_var
```

- `test_memory_layout()::static_var`：函数内的未初始化静态变量
- `test_global_static_memory()::function_static_var`：函数内的已初始化静态变量
- 符号名包含函数名，表示它们的作用域

### 4. 全局函数
```
0000000100001bb0 T test_heap_memory()
0000000100002348 T test_memory_leak()
0000000100001998 T test_stack_memory()
00000001000020a8 T test_memory_layout()
000000010000228c T test_stack_overflow()
0000000100001e70 T test_global_static_memory()
0000000100002404 T _main
```

- 所有测试函数都是全局可见的
- `main` 函数有 `_` 前缀（macOS 命名约定）
- 大写 `T` 表示这些函数可以被其他文件调用

### 5. 未定义符号（外部依赖）
```
                 U __Unwind_Resume
```

- `__Unwind_Resume`：未定义符号，需要从系统库链接
- 这是异常处理相关的函数

### 6. 编译器生成的符号
```
0000000100003528 s GCC_except_table15
0000000100003550 s GCC_except_table17
```

- `GCC_except_table`：异常处理表
- 这些是编译器自动生成的符号

## 符号命名规则

### C++ Name Mangling
C++ 函数名会被编译器"修饰"以支持重载：

原始函数名：
```cpp
void test_stack_memory()
```

修饰后的符号名：
```
__Z17test_stack_memoryv
```

解码规则：
- `__Z`：C++ 符号前缀
- `17`：函数名长度
- `test_stack_memory`：函数名
- `v`：void 返回类型

### 使用 c++filt 解码
```bash
nm 01_stack_vs_heap | c++filt
```

这会将修饰后的符号名还原为可读的 C++ 名称。

## 实用 nm 命令示例

### 1. 查看所有符号
```bash
nm 01_stack_vs_heap
```

### 2. 只查看外部符号
```bash
nm -g 01_stack_vs_heap
```

### 3. 解码 C++ 符号名
```bash
nm -C 01_stack_vs_heap
# 或
nm 01_stack_vs_heap | c++filt
```

### 4. 只查看已定义的符号
```bash
nm --defined-only 01_stack_vs_heap
```

### 5. 按地址排序
```bash
nm -n 01_stack_vs_heap
```

### 6. 查看特定符号
```bash
nm 01_stack_vs_heap | grep global_var
```

### 7. 查看符号大小（需要使用 size 命令）
```bash
size 01_stack_vs_heap
```

## 内存布局分析

从 nm 输出可以看出内存布局：

```
代码段（Text）：
- 0x100001304 - 0x100002348：各种测试函数

数据段（Data）：
- 0x100008000：_global_var
- 0x100008004：static_var
- 0x100008008：function_static_var
- 0x10000800c：_uninitialized_global_var

只读段（RODATA）：
- 0x100003e64：const_global_var
```

## 静态变量的作用域验证

从 nm 输出可以验证：

1. **第21行的 `static_var`**：
   ```
   0000000100008004 d static_var
   ```
   - 文件作用域的静态变量
   - 小写 `d` 表示局部可见

2. **第208行的 `static_var`**：
   ```
   0000000100008010 b test_memory_layout()::static_var
   ```
   - 函数作用域的静态变量
   - 符号名包含函数名
   - 小写 `b` 表示局部可见且未初始化

这两个是完全不同的符号，不会冲突！

## 总结

nm 命令是分析目标文件和可执行文件的强大工具：

1. **符号类型**：大写字母表示全局符号，小写字母表示局部符号
2. **存储段**：T（代码）、D（数据）、B（未初始化数据）、S（其他）
3. **作用域**：通过符号名和类型可以判断变量的作用域
4. **C++ 支持**：使用 c++filt 可以解码修饰后的符号名

对于理解 C++ 的底层实现，nm 命令是必不可少的工具。