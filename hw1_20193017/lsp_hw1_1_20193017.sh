#!/bin/bash
# 두 개의 정수 a, b를 입력받은 후, a와 b 사이에 속하는 모든 소수의 합을 구하시오.

echo -n "두 개의 정수 a, b를 입력하세요 : "
read num1 num2

let a=$num1 #num1의 값을 a에 저장
let b=$num2 #num2의 값을 b에 저장

for ((i=$a;i<=$b;i++))
do
	for ((j=1;j<=i;j++))
	do
		if((i%j==0))
		then
			let cnt=$cnt+1 #a와 b 사이의 소수 확인 변수
		fi
	done

	if (($cnt==2))
	then
		let res+=i #소수 합 저장 변수
	fi

	cnt=0
done

echo "a, b 사이에 속한 모든 소수의 합 : $res"
