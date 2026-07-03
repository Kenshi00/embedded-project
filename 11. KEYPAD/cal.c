/* cal.c */
#include "cal.h"
#include <stdio.h>

#define CALC_MAX 40

char calc_buff[CALC_MAX]; // 수식을 저장할 바구니
int calc_idx = 0;         // 바구니에 담을 위치

void calculator_process(uint8_t key)
{
	// 1. '=' 이외의 버튼 (숫자나 연산자)이 눌렸을 때
	if (key != '=')
	{
		if (calc_idx < CALC_MAX - 1)
		{
			calc_buff[calc_idx++] = key;
			calc_buff[calc_idx] = '\0';
			printf("%c", key);
		}
	}
	// 2. '=' 버튼이 눌렸을 때 (계산 시작)
	else
	{
		printf("=");
		
		int total_sum = 0;    // 최종적으로 화면에 출력될 정답
		int current_term = 0; // 중간단계 (term)
		int current_num = 0;  // 방금 읽어들인 숫자
		char current_op = '+'; // 이전 연산자 (첫 시작은 +로 가정)
		// 10 + 13 * 2 = (current_num -> current_term -> total_sum) 
		int i = 0;
		while (calc_buff[i] != '\0')
		{
			// 숫자인 경우: 자릿수 누적
			if (calc_buff[i] >= '0' && calc_buff[i] <= '9')
			{
				current_num = (current_num * 10) + (calc_buff[i] - '0');
			}
			// 연산자를 만난 경우
			else
			{
				if (current_op == '+')
				{
					total_sum += current_term;  
					current_term = current_num;  
				}
				else if (current_op == '-')
				{
					total_sum += current_term;  
					current_term = -current_num;
				}
				else if (current_op == '*')
				{
					current_term *= current_num;
				}
				else if (current_op == '/')
				{
					if (current_num != 0) current_term /= current_num;
					else printf(" [Err] ");
				}
				// 새 연산자 기억 & temp 초기화
				current_op = calc_buff[i]; 
				current_num = 0;           
			}
			i++;
		}
		
	
		if (current_op == '+')
		{
			total_sum += current_term;
			current_term = current_num;
		}
		else if (current_op == '-')
		{
			total_sum += current_term;
			current_term = -current_num;
		}
		else if (current_op == '*')
		{
			current_term *= current_num;
		}
		else if (current_op == '/')
		{
			if (current_num != 0) current_term /= current_num;
		}
		
		
		total_sum += current_term;
		
		printf("%d\n", total_sum);
		
		// 다음 계산을 위해 버퍼 초기화
		calc_idx = 0;
		calc_buff[0] = '\0';
	}
}