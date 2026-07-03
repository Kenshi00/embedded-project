#include "I2C_RTC.h"

void I2C_init(void) {
	DDRD |= (1 << 0) | (1 << 1); // SCL, SDA 출력 설정
	TWSR = 0x00;
	TWBR = ((F_CPU / 100000UL) - 16) / 2; // 100kHz
}

uint8_t I2C_start(void) {
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	uint8_t st = TWSR & 0xF8;
	return (st == TWSR_START || st == TWSR_REP_START) ? 0 : st;
}

void I2C_stop(void) {
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
	_delay_us(10);
}

uint8_t I2C_transmit(uint8_t data) {
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	return (TWSR & 0xF8);
}

// 수신 함수: 성공 시 0 반환, 데이터는 포인터로 전달
uint8_t I2C_receive_ACK(uint8_t *data) {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	while (!(TWCR & (1 << TWINT)));
	*data = TWDR;
	return (TWSR & 0xF8);
}

uint8_t I2C_receive_NACK(uint8_t *data) {
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	*data = TWDR;
	return (TWSR & 0xF8);
}

uint8_t bcd_to_decimal(uint8_t bcd) { return (bcd >> 4) * 10 + (bcd & 0x0F); }
uint8_t decimal_to_bcd(uint8_t decimal) { return ((decimal / 10) << 4) | (decimal % 10); }