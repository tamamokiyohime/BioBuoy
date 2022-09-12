/*
void measurement_pH() {

	//flush_pH_serial();
	String str = "";
	str.reserve(20);

	//Atlas_pH.print("R\r");
	Serial.print("pHstr:");
	bool complete = false;
	for (unsigned long _time = millis(); millis() - _time < 500;) {
		if (Atlas_pH.available()) 
		{
			char inchar = (char)Atlas_pH.read();
			str += inchar;
			Serial.print(inchar);
			if (inchar == '\r')
			{
				Serial.println();
				//is the first charater is number?
				if (isDigit(str[0])) {
					
					complete = true;
					break;
				}
				else {
					str = "";
				}
				
			}
		}
	}
	if (complete)	pHdecode(str);
	else Serial.println("pH not decode");
}
*/
void getpHdata() {
	String str = "";
	str.reserve(10);
	flush_pH_serial();

	Atlas_pH.print('\r');
	str = waitUntilpHReturn(150);
	SerialDebug("wakepH:", str);

	String s = "T,";	s += K_temperature;		s += "\r";
	Atlas_pH.print(s);
	str = waitUntilpHReturn(100);
	SerialDebug("sendT2pH:", str);

	Atlas_pH.print("R\r");
	str = waitUntilpHReturn(800);
	pHdecode(str);
	SerialDebug("readpH:", str);

	Atlas_pH.print("Sleep\r");
	SerialDebug("sleeppH", "");
}


void pHdecode(String data) {
	SerialDebug("pHraw:", data);
	
	if (!isDigit(data[0]))	return;
	pH = data.toFloat();
}

void flush_pH_serial() {
  while (Atlas_pH.available() > 0)
  {
    Atlas_pH.read();
    //delay(1);
  }
}

/*
void requestAtlas() {
	Atlas_EC.print("R\r");
	Atlas_pH.print("R\r");
}
void wakeAtlas() {
	Atlas_EC.print("\r");
	Atlas_pH.print("\r");
}

void sleepAtlas() {
	Atlas_EC.print("Sleep\r");
	Atlas_pH.print("Sleep\r");
}
*/

void pHCalibration(String type, float target) {
	String str = "";
	str.reserve(10);
	flush_pH_serial();
	update_K_temp();

	Atlas_pH.print('\r');
	str = waitUntilpHReturn(150);
	if (DEBUG || debugMode) {
		Serial.print("wake pH:");
		Serial.println(str);
	}

	String s = "T,";	s += K_temperature;		s += "\r";
	Atlas_pH.print(s);
	str = waitUntilpHReturn(150);
	if (DEBUG || debugMode) {
		Serial.print("sendT2pH");
		Serial.println(str);
	}

	String cmd = "";
	cmd = "Cal," + type + "," + String(target, 3) + "\r";
	Atlas_pH.print(cmd);
	str = waitUntilpHReturn(1000);
	if (DEBUG || debugMode) {
		Serial.print("calpH");
		Serial.println(str);
	}

	Serial.println(cmd);
	lcd.setCursor(8, 3);
	lcd.print("-" + type);
	for (int i = type.length(); i < 5; i++)	lcd.print(" ");

	Atlas_pH.print("Sleep\r");
}

String waitUntilpHReturn(unsigned long ttl) {
	String s = "";
	s.reserve(20);
	for (unsigned long time = millis(); millis() - time < ttl;) {
		if (Atlas_pH.available()) {
			char inchar = (char)Atlas_pH.read();
			if (inchar == '\r') return s;
			else				s += inchar;
		}
	}
	return "ttl";
}