
void UpdateEEPParameter() {
	//Get Device ID from EEPROM
	ReadEEP(IDAddress, &device_ID);
	//Get Thermocouple Calibration Data from EEPROM
	ReadEEP(k_cal_addr, &k_cal);
	//Get Interval Setting Data from EEPROM
	ReadEEP(interval_addr, &interval);
	//Get Power Saving Status from EEPROM
	ReadEEP(power_addr, &powerSaving);
	//Get pump PWM value from EEPROM
	ReadEEP(pumpPower_addr, &pumpPower);
	//Get pump PWM value from EEPROM
	ReadEEP(voltage_min_addr, &V_min);
	//Get pump PWM value from EEPROM
	ReadEEP(voltage_max_addr, &V_max);
	//Get pump PWM value from EEPROM
	ReadEEP(ec_print_addr, &ec_print);
	//Get voltage calibration slope from EEPROM
	ReadEEP(voltage_slope_addr, &voltage_slope);
	//Get pCO2 pressure guard low threshold
	ReadEEP(pCO2_lowP_addr, &pCO2_lowP);
	//Get pCO2 pressure guard high threshold
	ReadEEP(pCO2_highP_addr, &pCO2_highP);
	//Get pCO2 pressure guard sample time
	ReadEEP(pCO2_PSampleT_addr, &pCO2_PSampleT);
	//Get pCO2 pressure guard recover mode
	ReadEEP(pCO2_PRecoverMode_addr, &pCO2_PRecoverMode);
	lcd.createChar(3, (bool)ec_print ? ES_Stand : EC_Stand);
}

void PrintEEPParameter() {
	UpdateEEPParameter();
	if (DEBUG || debugMode) 
	{
		Serial.print("ID\tk_cal\tinter\tpower\tPMW\tVmin\tVmax\tVSlope\tECprint\tPGLow\tPGHigh\tPGTime\tPGMode\n");
		Serial.print(device_ID);
		Serial.print('\t');
		Serial.print(k_cal);
		Serial.print('\t');
		Serial.print(interval);
		Serial.print('\t');
		Serial.print(powerSaving);
		Serial.print('\t');
		Serial.print(pumpPower);
		Serial.print('\t');
		Serial.print(V_min);
		Serial.print('\t');
		Serial.print(V_max);
		Serial.print('\t');
		Serial.print(voltage_slope);
		Serial.print('\t');
		Serial.print(ec_print);
		Serial.print('\t');
		Serial.print(pCO2_lowP);
		Serial.print('\t');
		Serial.print(pCO2_highP);
		Serial.print('\t');
		Serial.print(pCO2_PSampleT);
		Serial.print('\t');
		Serial.print(pCO2_PRecoverMode);
		Serial.print("\n");
	}
}

template <class T> bool UpdateEEP(int addr, const T value)
{
	int temp = 0;
	EEPROM.put(addr, value);
	EEPROM.get(addr, temp);
	return temp == value;
}

template <class T> void ReadEEP(int addr, const T &value)
{
	EEPROM.get(addr, *value);
}
