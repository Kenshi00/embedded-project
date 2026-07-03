int main(void) {
	LCD_init();
	I2C_init();
	init_uart0();
	stdout = &OUTPUT;
	sei();

	uint8_t sec, min, hour, day_of_week, day, month, year;
	uint8_t address = 0x68;
	char lcd_buff[16];

	printf("System Start & RTC Read Mode\r\n");

	while (1) {
		// 1. 더미 쓰기 (Read를 위한 주소 설정)
		if (I2C_start() == 0) {
			I2C_transmit(address << 1); // 쓰기 모드
			I2C_transmit(0x00);         // 0번지 설정
			I2C_stop();
		}

		// 2. 읽기 시작
		I2C_start();
		I2C_transmit((address << 1) | 1); // 읽기 모드

		// 데이터 수신 및 에러 체크
		uint8_t temp;
		I2C_receive_ACK(&temp); sec = bcd_to_decimal(temp);
		I2C_receive_ACK(&temp); min = bcd_to_decimal(temp);
		I2C_receive_ACK(&temp); hour = bcd_to_decimal(temp);
		I2C_receive_ACK(&temp); day_of_week = bcd_to_decimal(temp);
		I2C_receive_ACK(&temp); day = bcd_to_decimal(temp);
		I2C_receive_ACK(&temp); month = bcd_to_decimal(temp);
		I2C_receive_NACK(&temp); year = bcd_to_decimal(temp);
		
		I2C_stop();

		printf("20%02d-%02d-%02d %02d:%02d:%02d\r\n", year, month, day, hour, min, sec);
		
		// LCD 출력 (이전처럼 덮어쓰기)
		LCD_goto_XY(0, 2);
		sprintf(lcd_buff, "%02d-%02d-%02d", year, month, day);
		LCD_write_string(lcd_buff);
		LCD_goto_XY(1, 2);
		sprintf(lcd_buff, "%02d:%02d:%02d", hour, min, sec);
		LCD_write_string(lcd_buff);

		_delay_ms(1000);
	}
}