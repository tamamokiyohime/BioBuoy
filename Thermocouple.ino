void update_K_temp() {
	float temp = 0.0f;
	//noFault = true;
	int retry = 0;
	while (retry < 3) {
		temp = max.readThermocoupleTemperature();
		fault = max.readFault();
		SerialDebug("Fault:0x", String(fault, HEX));

		if (fault) {
			retry++;
			delay(10);
		}
		else
		{
			K_temperature = temp + k_cal;
			break;
		} 
	}
	if (fault) Serial.println("*Thermo Error");
	lcd.createChar(5, fault ? k_fail_icon : k_icon);
	fault ? ThermoER.resume() : ThermoER.pause();
}


void send_temperature() {
	String s = "";
	s += "T,";
	s += K_temperature;
	s += "\r";
	Atlas_EC.print(s);
	Atlas_pH.print(s);
 }


void calibrateThermo(float target) {
  K_temperature = max.readThermocoupleTemperature();
  k_cal = target - K_temperature;
  UpdateEEP(k_cal_addr, k_cal);
  lcd.setCursor(15, 3);
  if (target == 25.00)	lcd.print("-S25.");
  else lcd.print("-User");
  lcd.setCursor(0, 1);
  lcd.print("K Cal to ");
  lcd_qFprint(k_cal, 2, 9, 1);
  k_cal_btnEvt = true;
}

bool MAXisFault() {
	return digitalRead(MAX31856FT) == HIGH ? true : false;
}

bool MAXisReady() {
	return digitalRead(MAX31856DR) == HIGH ? true : false;
}

void ThermoError() {
	if (fault & MAX31856_FAULT_CJRANGE) Serial.println("Cold Junction Range Fault");
	if (fault & MAX31856_FAULT_TCRANGE) Serial.println("Thermocouple Range Fault");
	if (fault & MAX31856_FAULT_CJHIGH)  Serial.println("Cold Junction High Fault");
	if (fault & MAX31856_FAULT_CJLOW)   Serial.println("Cold Junction Low Fault");
	if (fault & MAX31856_FAULT_TCHIGH)  Serial.println("Thermocouple High Fault");
	if (fault & MAX31856_FAULT_TCLOW)   Serial.println("Thermocouple Low Fault");
	if (fault & MAX31856_FAULT_OVUV)    Serial.println("Over/Under Voltage Fault");
	if (fault & MAX31856_FAULT_OPEN)    Serial.println("Thermocouple Open Fault");
}
