void tsys01_get() {
	if (isnan(temperature)) m_tsys01.begin();
	connected = m_tsys01.is_connected();
	float tsys01_temp = 0.00;

	if (connected) {
		for (int i = 0; i < sample_times; i++) {
			m_tsys01.read_temperature(&temperature);
			tsys01_temp += temperature;
		}
		temperature = tsys01_temp / sample_times;
	}
	else {
		temperature = NAN;
		Serial.println("*TSYS01 Read Error");
		//waring();
	}
}


