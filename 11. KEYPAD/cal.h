/*
 * cal.h
 *
 * Created: 2026-06-29 오후 4:22:26
 *  Author: kccistc
 */ 

/* cal.h */

#include <stdint.h>

// 큐에서 꺼낸 키패드 문자를 계산기로 전달하는 함수
extern void calculator_process(uint8_t key);
