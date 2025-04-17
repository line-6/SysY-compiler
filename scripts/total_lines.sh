#!/bin/bash

# 统计代码行数

# 获取目录路径
DIR="/root/compiler/sysy_compiler/src"

# 初始化行数计数器
TOTAL_LINES=0

# 使用循环直接处理文件，避免管道导致的子 Shell 问题
while IFS= read -r -d '' file; do
  LINES=$(wc -l < "$file")
  TOTAL_LINES=$((TOTAL_LINES + LINES))
done < <(find "$DIR" -type f \( -name "*.h" -o -name "*.cpp" -o -name "*.l" -o -name "*.y" \) -print0)

# 输出总行数
echo "Total lines of .h /.cpp /.l(Flex) /.y(Bison) files: $TOTAL_LINES"