void measure_BME280() {
	float press_temp = 0.0f;
	float humid_temp = 0.0f;
	//if (pressure == NULL) bme280.begin();
	//if (bme280connected) bme280.begin();
	//bme280connected = bme280.is_connected();
	
	if (bme280.begin(0x76, &Wire))
	{
		delay(2);
		pressure = bme280.readPressure();
		humidity = bme280.readHumidity();
		pressure_in_atm = pressure / 101325.0f;
		check_pCO2_Pressure(pressure_in_atm);
	}
	else 
	{
		pressure = NAN;
		humidity = -9999;
		pressure_in_atm = NAN;
		Serial.println("*BME280 Read Error");
		//waring();
	}

}

void check_pCO2_Pressure(float value) {
	if (value > pCO2_highP || value < pCO2_lowP)	_pCO2_PSampleT++;
	else
	{
		_pCO2_PSampleT = 0;
		if (pCO2_PRecoverMode) 
		{
			pumpControl(pumpPower);
			PGER.pause();
		}
	}
	if (_pCO2_PSampleT >= abs(pCO2_PSampleT)) 
	{
		waring();
		PGER.resume();
		pumpControl(0);
		Serial.println("*Pump stop");
	}
}

void setPGuardbystr(char* strIn) {
	//Separate the serial command to command and data by ';'
	char* chr_high;
	char* chr_low;
	char* chr_times;
	char* chr_mode;
	chr_low = strtok(strIn, ";");
	chr_high = strtok(NULL, ";");
	chr_times = strtok(NULL, ";");
	chr_mode = strtok(NULL, ";");
	pCO2_highP = atof(chr_high);
	pCO2_lowP = atof(chr_low);
	pCO2_PSampleT = atoi(chr_times);
	pCO2_PRecoverMode = (bool)atoi(chr_mode);
	UpdateEEP(pCO2_lowP_addr, pCO2_lowP);
	UpdateEEP(pCO2_highP_addr, pCO2_highP);
	UpdateEEP(pCO2_PSampleT_addr, pCO2_PSampleT);
	UpdateEEP(pCO2_PRecoverMode_addr, pCO2_PRecoverMode);
	PrintEEPParameter();
}