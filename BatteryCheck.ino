float GetBatteryVoltage()
{
	float value = 0.0f;
	for (int i = 0; i < 3; i++) value += (float)analogRead(batteryV_pin);
	value = value * 30.0f;
	value = value / 1023.0f;
	value /= 3.0f;
	value = value * voltage_slope;
	///3 -> sample times
	///1023 -> ADC value
	///30 -> max Voltage of voltage divider circuit
	return value;

}

void MeasureBatteryVoltage() {
	voltage = GetBatteryVoltage();
	if (VoltageInPercent() > 20)	battery.off();
	if (VoltageInPercent() <= 10)	battery.resume();
}

int VoltageInPercent() {
	int value = (voltage - V_min) / (V_max - V_min) * 100;
	if (value > 100) value = 100;
	if (value < 0)	value = 0;
	return value;
}