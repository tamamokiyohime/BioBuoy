void init_System() {
	Watchdog.disable();
	RamER.on();
	battery.on();
	SDIO.on();
	ThermoER.on();
	PGER.on();
	//RTCER.on();
	if (LogIn("2567", "---System   Reset---")) {
		SensorFactoryReset();
		tone(buzzerpin, 20, 50);
		lcd.setCursor(0, 3);
		lcd.print("Finish...Reboot");
		Serial.println("Initialize Complete!");
		delay(3000);
		resetFunc();
	}
	else {
		Watchdog.reset();
		Watchdog.enable();
		RamER.off();
		battery.off();
		SDIO.off();
		ThermoER.off();
		PGER.off();
		//RTCER.off();
	}
	
}

void init_DeviceID() {
	Watchdog.disable();
	if (!LogIn("1245", "---ID  Maintaince---")) return;
	String cmd = "";
	lcd.setCursor(0, 2);
	lcd.print("New ID:");

	lcd.setCursor(0, 2);
	if (!GetInputData(&cmd, 30000, true)) {
		lcd.print("Time Out .......Exit");
		delay(1000);
		lcd.clear();
		return;
	}

	UpdateEEP(IDAddress, cmd.toInt());
	UpdateEEPParameter();
	getFilename(filename);
	File_check_exists();
	//Serial.print("New Device ID is:");
	//Serial.println(cmd.toInt());
	//EEPROM.write(IDAddress, cmd.toInt());
	//Serial.println("ID Reset Complete!");
	lcd.setCursor(0, 3);
	tone(buzzerpin, 20, 50);
	lcd.print("Finish...Reboot");
	delay(5000);
	resetFunc();
}

bool LogIn(String pinCode, String title) {
	lcd.clear();
	String cmd = "";
	lcd.setCursor(0, 0);
	lcd.print(title);
	lcd.setCursor(0, 1);
	lcd.print("Enter:[" + pinCode + "]-> apply");
	lcd.setCursor(0, 2);
	lcd.print("PIN:");
	
	if (!GetInputData(&cmd, 30000, true)) {
		lcd.setCursor(0, 2);
		lcd.print("Time Out .......Exit");
		delay(1000);
		lcd.clear();
		return false;
	}

	lcd.setCursor(0, 2);
	if (pinCode.equals(cmd)) {
		lcd.print("  PinCode Accepted  ");
		return true;
	}
	else {
		lcd.print("PIN Reject .....Exit");
		delay(1000);
		lcd.clear();
		return false;
	}
}

bool GetInputData(String* cmd, long expair, bool print) {
	long entertime = millis();
	while (true) {
		if (millis() - entertime <= expair)
		{
			if (GetPadData(cmd, print))	return true;
		}
		else	return false;
	}
}

bool GetPadData(String* s, bool beep)
{
	customKeypad.tick();
	bool finish = false;
	while (customKeypad.available())
	{
		
		keypadEvent e = customKeypad.read();
		char inputdata = (char)e.bit.KEY;
		if (e.bit.EVENT == KEY_JUST_PRESSED)
		{
			pad();
			if (inputdata == '#')
			{
				finish = true;
			}
			else {
				lcd.print(beep ? inputdata : '*');
				*s += inputdata;
			}
		}
	}
	delay(50);
	return finish;
}

void SensorFactoryReset() {
	UpdateEEP(IDAddress, 99);
	UpdateEEP(k_cal_addr, 0.0f);
	UpdateEEP(interval_addr, 10000l);
	UpdateEEP(power_addr, 0);
	UpdateEEP(pumpPower_addr, 0);
	UpdateEEP(voltage_min_addr, 6.2f);
	UpdateEEP(voltage_max_addr, 8.4f);
	UpdateEEP(voltage_slope_addr, 1.0f);
	UpdateEEP(pCO2_lowP_addr, 0.8f);
	UpdateEEP(pCO2_highP_addr, 1.2f);
	UpdateEEP(pCO2_PSampleT_addr, 3);
	UpdateEEP(pCO2_PRecoverMode_addr, false);
	UpdateEEP(ec_print_addr, 0);
	//RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
	Atlas_EC.print("\r");
	Atlas_pH.print("\r");
	delay(100);
	Atlas_EC.print("*OK,1\r");
	Atlas_pH.print("*OK,1\r");
	delay(50);
	Atlas_EC.print("L,0\r");
	Atlas_pH.print("L,0\r");
	delay(50);
	Atlas_EC.print("C,0\r");
	Atlas_pH.print("C,0\r");
	delay(50);
	Atlas_EC.print("Cal,clear\r");
	Atlas_pH.print("Cal,clear\r");
	delay(50);
	Atlas_EC.print("O,EC,1\r");
	delay(50);
	Atlas_EC.print("O,TDS,1\r");
	delay(50);
	Atlas_EC.print("O,S,1\r");
	delay(50);
	Atlas_EC.print("O,SG,1\r");
	delay(50);
}

bool CheckPIN(String pinCode, String input) {
	return input.equals(pinCode);
}
