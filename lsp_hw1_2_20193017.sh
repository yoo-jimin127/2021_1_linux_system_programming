#!/bin/bash
# 2020년 12월 31일은 목요일이다. 2021년의 하루를 월과 일을 나타내는 두 개의 정수로 입력받은 후, 해당 일이 무슨 요일인지 출력하시오.

declare -i days
days=(31 28 31 30 31 30 31 31 30 31 30 31)

div=7

echo -n "월과 일을 입력하세요 :"
read num1 num2

let month=$num1 #월을 month변수에 저장
let day=$num2 #일을 day변수에 저장

for ((i=1;i<month;i++))
	do
		let year_days=$year_days+${days[i]}
done

year_days=$((year_days+day))

let week_day=$((year_days%div))

case $week_day in
	"0") echo Thursday;;
	"1") echo Friday;;
	"2") echo Saturday;;
	"3") echo Sunday;;
	"4") echo Monday;;
	"5") echo Tuesday;;
	"6") echo Wednesday;;
	*) echo wrong_data;;
esac
