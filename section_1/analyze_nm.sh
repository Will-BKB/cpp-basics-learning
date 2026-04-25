#!/bin/bash

# nm 命令分析脚本
# 用于分析 01_stack_vs_heap 可执行文件的符号表
# 使用方法：在 section_1 目录内执行 ./analyze_nm.sh

EXECUTABLE="./01_stack_vs_heap"

if [ ! -f "$EXECUTABLE" ]; then
    echo "错误：找不到可执行文件 $EXECUTABLE"
    echo "请先编译：g++ -g -Wall -Wextra -std=c++17 01_stack_vs_heap.cpp -o 01_stack_vs_heap"
    exit 1
fi

echo "========================================"
echo "nm 命令分析示例"
echo "========================================"
echo ""

echo "1. 查看所有符号（前 20 行）"
echo "----------------------------------------"
nm "$EXECUTABLE" | head -20
echo ""

echo "2. 查看所有符号（解码 C++ 符号名）"
echo "----------------------------------------"
nm -C "$EXECUTABLE" | head -20
echo ""

echo "3. 只查看外部符号（全局可见）"
echo "----------------------------------------"
nm -g "$EXECUTABLE"
echo ""

echo "4. 查看全局变量和静态变量"
echo "----------------------------------------"
nm -C "$EXECUTABLE" | grep -E "(global_var|static_var|const_global_var|uninitialized_global_var)"
echo ""

echo "5. 查看所有函数"
echo "----------------------------------------"
nm -C "$EXECUTABLE" | grep " T "
echo ""

echo "6. 查看未定义符号（外部依赖）"
echo "----------------------------------------"
nm "$EXECUTABLE" | grep " U "
echo ""

echo "7. 按地址排序查看符号"
echo "----------------------------------------"
nm -n "$EXECUTABLE" | head -20
echo ""

echo "8. 查看符号类型统计"
echo "----------------------------------------"
nm "$EXECUTABLE" | awk '{print $2}' | sort | uniq -c | sort -rn
echo ""

echo "9. 查看特定段的符号"
echo "----------------------------------------"
echo "Text 段（代码）："
nm -C "$EXECUTABLE" | grep " T "
echo ""
echo "Data 段（已初始化数据）："
nm -C "$EXECUTABLE" | grep " D "
echo ""
echo "BSS 段（未初始化数据）："
nm -C "$EXECUTABLE" | grep " B "
echo ""

echo "10. 对比静态变量和全局变量"
echo "----------------------------------------"
echo "全局变量（大写字母）："
nm -C "$EXECUTABLE" | grep -E " D | B " | grep -v "::"
echo ""
echo "静态变量（小写字母）："
nm -C "$EXECUTABLE" | grep -E " d | b | s "
echo ""

echo "========================================"
echo "分析完成"
echo "========================================"