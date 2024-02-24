#!/bin/bash


BIN="../../IFJ19Compiler"
should_pass_path="../test_source_codes/basic_parser_tests/should_pass/"

should_pass=( `ls $should_pass_path | grep .ifj19` )
#echo ${should_pass[*]}

should_fail_path="../test_source_codes/basic_parser_tests/should_fail/"

should_fail=( `ls $should_fail_path | grep .ifj19` )
#echo ${should_fail[*]}

# Přelož projekt
cd ../.. && make >/dev/null 2>&1 && cd - >/dev/null|| (echo -e "\e[31mCompilation failed" && exit 1)

echo ""
echo "Syntactically correct tests"
echo ""
for file in ${should_pass[*]}; do
	$BIN -i "$should_pass_path$file" && echo -e "\e[32mTest:\e[0m $file \e[32msucceeded\e[0m" || echo -e "\e[31mTest:\e[0m $file\e[31m failed :(\e[0m"
	echo ""
done

#odbarvení
echo -e "\e[0m"

echo "Syntactically wrong tests"
echo ""
for file in ${should_fail[*]}; do
        $BIN -i "$should_fail_path$file" 2>/dev/null && echo -e "\e[31mTest:\e[0m $file\e[31m failed :(\e[0m" || echo -e "\e[32mTest:\e[0m $file\e[32m succeeded\e[0m"
	echo ""
done

echo -e "\e[0m"
