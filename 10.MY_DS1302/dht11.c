/*
 * dht11.c
 *
 * Created: 2026-06-26 오전 9:28:02
 *  Author: kccistc
 */ 

#include "dht11.h"

#define DHT11_DDR DDRG
#define DHT11_PORT PORTG // 출력할때 비교
#define DHT11_PIN PING // 입력 받을때 비교
#define DHT11_INPUT_PIN 0
void dht11_main(void);
enum t_state {OK, STARTUP_TIMEOUT, DATA_TIMEOUT, CHECKSUM_ERROR};
	void dht11_main(void)
	{
		uint8_t bytes[5]; // DHT11 데이터는 5바이트(40비트)면 충분합니다.
		uint8_t state = 0;
		int     us_counter = 0;
		
		// 1. 변수 초기화
		state = OK;
		memset(bytes, 0, sizeof(bytes));
		
		// --------------- start up signal scenario ---------------------
		// 1. start up signal 전송
		// --- reset DHT11
		DHT11_DDR |= 1 << DHT11_INPUT_PIN; // output mode
		DHT11_PORT |= 1 << DHT11_INPUT_PIN; // high
		_delay_ms(100);
		
		// --- low 최소 18ms
		DHT11_PORT &= ~(1 << DHT11_INPUT_PIN); // low
		_delay_ms(20); // spec상으로 최소 18ms 유지(redundancy 고려)
		
		DHT11_PORT |= 1 << DHT11_INPUT_PIN; // pull up
		_delay_us(30);
		
		// 2. --- start signal 응답 check
		DHT11_DDR &= ~(1 << DHT11_INPUT_PIN); // input mode 전환
		
		// DHT11이 LOW로 응답을 시작할 때까지 대기 (MCU가 Pull-up한 20~40us 통과)
		us_counter = 0;
		while((DHT11_PIN & (1 << DHT11_INPUT_PIN)))
		{
			_delay_us(1);
			if(++us_counter > 100) {state = STARTUP_TIMEOUT; break;}
		}
		
		// DHT11이 응답으로 보낸 LOW 구간(80us)이 끝날 때까지 대기
		if(state == OK)
		{
			us_counter = 0;
			// 수정된 부분: !(조건) 형태로 올바르게 LOW 상태 체크
			while(!(DHT11_PIN & (1 << DHT11_INPUT_PIN)))
			{
				_delay_us(1);
				if(++us_counter > 100) {state = STARTUP_TIMEOUT; break;}
			}
		}
		
		// DHT11이 응답으로 보낸 HIGH 구간(80us)이 끝날 때까지 대기 (데이터 전송 직전)
		if(state == OK)
		{
			us_counter = 0;
			while((DHT11_PIN & (1 << DHT11_INPUT_PIN)))
			{
				_delay_us(1);
				if(++us_counter > 100) {state = STARTUP_TIMEOUT; break;}
			}
		}
		
		// --------------- DATA read Part ---------------------
		// 40비트(5바이트)의 온습도 데이터를 읽어옵니다.
		if(state == OK)
		{
			for(int i = 0; i < 5; i++)
			{
				for(int j = 0; j < 8; j++)
				{
					// 1단계: 각 비트가 시작되기 전 LOW 구간(약 50us) 대기
					us_counter = 0;
					while(!(DHT11_PIN & (1 << DHT11_INPUT_PIN)))
					{
						_delay_us(1);
						if(++us_counter > 100) { state = DATA_TIMEOUT; break; }
					}
					if(state != OK) break; // 타임아웃 발생 시 루프 즉시 탈출

					// 2단계: 데이터 비트(0 또는 1) 판별을 위한 HIGH 구간 시간 측정
					us_counter = 0;
					while((DHT11_PIN & (1 << DHT11_INPUT_PIN)))
					{
						_delay_us(1);
						if(++us_counter > 100) { state = DATA_TIMEOUT; break; }
					}
					if(state != OK) break;

					// 3단계: 측정된 시간에 따라 0 또는 1을 바이트 배열에 저장
					bytes[i] <<= 1; // 비트를 왼쪽으로 한 칸 밀어줌 (기본값 0 채워짐)
					if(us_counter > 40) // HIGH 유지 시간이 40us를 넘어가면 '1'로 판정
					{
						bytes[i] |= 1; // 마지막 비트를 1로 켬
					}
				}
				if(state != OK) break;
			}
		}
		
		uint8_t checksum;
		// --------------- Checksum 검증 ---------------------
		// 수신된 데이터에 오류가 없는지 마지막으로 확인합니다.
		if(state == OK)
		{
			checksum = bytes[0] + bytes[1] + bytes[2] + bytes[3];
			// 5번째 바이트(체크섬) == 앞의 4바이트 합의 하위 8비트
			if(bytes[4] == ((bytes[0] + bytes[1] + bytes[2] + bytes[3]) & 0xFF))
			{
				// 통신 완전 성공!
				// bytes[0]: 습도 정수부, bytes[1]: 습도 소수부
				// bytes[2]: 온도 정수부, bytes[3]: 온도 소수부
			}
			else
			{
				state = CHECKSUM_ERROR; // 데이터가 깨져서 들어왔음
			}
		}
		
		switch(state)
		{
			case OK:
				printf("humi: %d.%d\n", bytes[0], bytes[1]);
				printf("temp: %d.%d\n", bytes[2], bytes[3]);
				break;
			case STARTUP_TIMEOUT:
				printf("STARTUP_TIMEOUT\n");
				break;
			case DATA_TIMEOUT:
				printf("DATA_TIMEOUT\n");
				break;
			case CHECKSUM_ERROR:
				printf("CHECKSUM_ERROR\n");
				break;
		}
	}