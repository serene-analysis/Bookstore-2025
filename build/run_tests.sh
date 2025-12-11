#!/bin/bash

# ========== 配置参数（根据你的目录结构调整） ==========
TEST_DIR="../TestPoints/1075"  # 测试点目录
EXE="./bookstore"       # 可执行文件路径
TEMP_OUT="./temp_test.out"    # 临时输出文件（运行后自动删除）
START_CASE=1                  # 起始测试点编号
END_CASE=214                  # 结束测试点编号（固定到214）


# ========== 前置检查 ==========
# 检查可执行文件是否存在
if [ ! -f "$EXE" ]; then
    echo "错误：可执行文件 $EXE 不存在！请先编译。"
    exit 1
fi

# 检查测试点目录是否存在
if [ ! -d "$TEST_DIR" ]; then
    echo "错误：测试点目录 $TEST_DIR 不存在！"
    exit 1
fi


# ========== 初始化统计 ==========
total=0
passed=0
missing=0  # 缺失文件的测试点数量

# ========== 从1到214遍历所有测试点 ==========
for case_num in $(seq $START_CASE $END_CASE); do
    # 拼接输入/输出文件路径
    in_file="$TEST_DIR/$case_num.in"
    out_file="$TEST_DIR/$case_num.out"
    total=$((total + 1))

    # 检查输入文件是否存在
    if [ ! -f "$in_file" ]; then
        echo "测试点 $case_num：缺少输入文件 $in_file → 跳过 ❓"
        missing=$((missing + 1))
        continue
    fi

    # 检查预期输出文件是否存在
    if [ ! -f "$out_file" ]; then
        echo "测试点 $case_num：缺少预期输出文件 $out_file → 跳过 ❓"
        missing=$((missing + 1))
        continue
    fi

    echo "=== 运行测试点 $case_num ==="

    # ========== 运行程序，重定向输入/输出 ==========
    ./run.sh
    "$EXE" < "$in_file" > "$TEMP_OUT"

    # ========== 对比输出与预期结果 ==========
    if diff -q "$TEMP_OUT" "$out_file" > /dev/null; then
        echo "测试点 $case_num：通过 ✅"
        passed=$((passed + 1))
    else
        echo "测试点 $case_num：不通过 ❌"
        echo "差异详情（实际输出 vs 预期输出）："
        diff "$TEMP_OUT" "$out_file"
    fi
    echo ""  # 空行分隔测试点
done


# ========== 测试总结 ==========
echo "=== 测试完成 ==="
echo "总测试点范围：$START_CASE ~ $END_CASE"
echo "有效测试点（文件齐全）：$((total - missing))"
echo "缺失文件的测试点：$missing"
echo "通过：$passed | 未通过：$((total - missing - passed))"


# ========== 清理临时文件 ==========
rm -f "$TEMP_OUT"