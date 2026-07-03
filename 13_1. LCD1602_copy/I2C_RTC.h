#ifndef I2C_RTC_H_
#define I2C_RTC_H_

#define F_CPU 16000000UL
#include <avr/io.h>

// I2C 상태 코드
#define TWSR_START       0x08
#define TWSR_REP_START   0x10
#define TWSR_MT_SLA_ACK  0x18
#define TWSR_MT_DATA_ACK 0x28
#define TWSR_MR_SLA_ACK  0x40
#define TWSR_MR_DATA_ACK 0x50

// 함수 선언 (상태 코드를 반환하도록 변경)
void I2C_init(void);
uint8_t I2C_start(void);
void I2C_stop(void);
uint8_t I2C_transmit(uint8_t data);
uint8_t I2C_receive_ACK(uint8_t *data); // 데이터와 상태를 분리
uint8_t I2C_receive_NACK(uint8_t *data);
uint8_t bcd_to_decimal(uint8_t bcd);
uint8_t decimal_to_bcd(uint8_t decimal);

#endif