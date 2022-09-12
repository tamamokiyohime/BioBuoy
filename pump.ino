void pumpControl(int freq) {
	int pwm = 0;
	if (freq != 0) {
		pwm = map(freq,1,100,70,255);
	}
	if (freq == 0) pwm = 0;
	analogWrite(pump_pin, pwm);
}

void updatePumpValue(int value) {
	UpdateEEP(pumpPower_addr, value);
	pumpPower = value;
	PrintEEPParameter();
	pumpControl(value);
}