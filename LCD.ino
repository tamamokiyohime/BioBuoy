void lcd_qSprint(String s, int column, int row) {
	lcd.setCursor(column, row);
	lcd.print(s);
}

void lcd_qFprint(float k, float num, int column, int row) {
	lcd.setCursor(column, row);
	isnan(k) ? lcd.print("-----") : lcd.print(k, num);
}


void LCD_update_main() {

///Row 0
		update_time_info();
		lcd.setCursor(18, 0);
		if (VoltageInPercent() >= 100) lcd.print("Fu");
		else lcd.print(inttostring(VoltageInPercent(), 2));
		//if (!RTC_alive) {			lcd.write(3);		}
		//lcd.print(gps_valid ? "G" : "-");
///Row 1
		LCDUpdateRow1();
		lcd.setCursor(17, 1);
		lcd.write(7);
		lcd.print(inttostring(humidity, 2));
///Row 2
		lcd.setCursor(0, 2);
		lcd.write(2);
		lcd.print(inttostring(valCO2, 5));

		lcd.setCursor(7, 2);
		lcd.write(6);
		//lcd_qFprint(pressure_in_atm, 3, 8, 2);
		lcd.print(floattostring(pressure_in_atm, 1, 3));

		lcd.setCursor(14, 2);
		lcd.write(4);
		lcd.print(floattostring(temperature, 2, 2));
		//lcd_qFprint(temperature, 2, 15, 2);
///ROW 3
		lcd.setCursor(6, 3);
		lcd.print(" ");
		lcd.setCursor(0, 3);
		//judge is EC or EP
		//lcd.createChar(3, (bool)ec_print ? EP_Stand : EC_Stand);
		lcd.write(3);
		if ((bool)ec_print) 
		{
			if (SAL < 10.0f )	lcd.print(floattostring(SAL, 1, 3));
			else				lcd.print(floattostring(SAL, 2, 2));
		}
		else
		{
			if		(EC < 100.0f)	lcd.print(floattostring(EC, 2, 2));
			else if (EC < 1000.0f)	lcd.print(floattostring(EC, 3, 1));
			else if	(EC < 99999.0f)	lcd.print(longtostring((long)EC, 5));
			else {
				lcd.print(inttostring(EC / 100, 4));
				lcd.print("h");
			}
				//lcd.print(longtostring(EC, 5));
		}

		lcd.setCursor(7, 3);
		lcd.write(1);
		lcd.print(floattostring(pH, 1, 3));

		lcd.setCursor(14, 3);
		lcd.write(5);
		lcd.print(floattostring(K_temperature, 2, 2));
}

void LCDUpdateRow1() {
	if (!resetRow1.isCounterCompleted())
	{
		RemoveBTNmessage();
		resetRow1.stop();
	}
	lcd.setCursor(0, 1);
	for (int i = 0; i < 10; i++)	lcd.print(keypaddata[i] == '\0' ? " " : String(keypaddata[i]));
	//lcd.print(String(keypaddata));
}

void RemoveBTNmessage() {
	lcd.setCursor(0, 1);
	for (int i = 0; i < 17; i++)lcd.print(" ");
	//row1cleared = true;
}

void LCDSerialPrint(String s) {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(s);
	Serial.println(s);
	delay(50);
}
