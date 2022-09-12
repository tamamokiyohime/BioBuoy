void getECdata() {
	String str = "";
	str.reserve(10);
	flush_EC_serial();

	Atlas_EC.print('\r');
	str = waitUntilECReturn(150);
	SerialDebug("wakeEC:", str);

	String s = "T,";	s += K_temperature;		s += "\r";
	Atlas_EC.print(s);
	str = waitUntilECReturn(100);
	SerialDebug("sendT2EC:", str);

	Atlas_EC.print("R\r");
	str = waitUntilECReturn(800);
	ECdecode(str);
	SerialDebug("readEC:", str);

	Atlas_EC.print("Sleep\r");
	SerialDebug("sleepEC", "");
}

void ECdecode(String data) {
	
	SerialDebug("ECraw:", data);
	if (!isDigit(data[0])) return;

	int len = data.length() + 1;
	char* str_arr = 0;
	str_arr = (char*)malloc(len * sizeof(char));
	data.toCharArray(str_arr, len);
	//Separate the serial command to command and data by ','
	char *_ec, *_tds, *_sal, *_grav;                        //char pointer used in string parsing

	  _ec = strtok(str_arr, ",");							//let's pars the array at each comma
	 _tds = strtok(NULL, ",");                              //let's pars the array at each comma
	 _sal = strtok(NULL, ",");                              //let's pars the array at each comma
	_grav = strtok(NULL, ",");                              //let's pars the array at each comma

	//  EC_i = atol(_ec);                                     //uncomment this line to convert the char to a float
	// TDS_i = atol(_tds);
	// SAL_f = atof(_sal);
	//GRAV_f = atof(_grav);
	EC = atof(_ec);
	TDS = atoi(_tds);
	SAL = atof(_sal);
	GRAV = atof(_grav);
	free(str_arr);
}



void flush_EC_serial() {
  while (Atlas_EC.available() > 0) {
    Atlas_EC.read();
  }
}


void ECCalibration(String type, long target) {
	String str = "";
	str.reserve(10);
	flush_EC_serial();
	update_K_temp();

	Atlas_EC.print("\r");
	str = waitUntilECReturn(150);

	SerialDebug("wakeEC :", str);

	String s = "T,";	s += K_temperature;		s += "\r";
	Atlas_EC.print(s);
	str = waitUntilECReturn(100);

	SerialDebug("sendT2EC:", str);

	String cmd = "";
	cmd = "Cal," + type;
	if (target != NULL) cmd += "," + String(target);
	cmd += "\r";
	Atlas_EC.print(cmd);
	str = waitUntilECReturn(1000);

	SerialDebug("calEC:", str);

	Serial.println(cmd);
	lcd.setCursor(1, 3);
	lcd.print("-" + type);
	for (int i = type.length(); i < 5; i++)	lcd.print(" ");
	Atlas_EC.print("Sleep\r");
}
String waitUntilECReturn(unsigned long ttl) {
	String s = "";
	s.reserve(20);
	for (unsigned long time = millis(); millis() - time < ttl;) {
		if (Atlas_EC.available()) {
			char inchar = (char)Atlas_EC.read();
			if (inchar == '\r') return s;
			else				s += inchar;
		}
	}
	return "ttl";
}