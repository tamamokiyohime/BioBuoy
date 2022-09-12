//void measure_CO2() {
//	for (int retry = 0; retry < 3; retry++) {
//		valCO2 = requestCO2();
//		if (valCO2 > -3000) break;
//		if (DEBUG || debugMode) {
//			Serial.print("K30 CO2 err:");
//			Serial.println(valCO2);
//		}
//		delay(50);
//	}
//	
//	if (valCO2 <= -3000 ) {
//		valCO2 = -9999;
//		Serial.println("K30 Read CO2 Error");
//	}
//}

void measure_CO2() {
	byte requestchar[] = { 0x22, 0x00, 0x08};
	String s = "";
	for (int retry = 0; retry < 3; retry++)
	{
		s = sendata(co2Addr, requestchar);
		if (s[0] != '*') break;
	}
	if (s[0] == '*') {
		Serial.println("*K30 Read CO2 Error");
		valCO2 = -9999;
	}
	else {
		valCO2 = StringtoInt(s);
	}
}

void getID()
{
	byte requestchar[] = { 0x24, 0x00, 0x28};
	String s = "";
	for (int retry = 0; retry < 3; retry++)
	{
		s = sendata(co2Addr, requestchar);
		if (s[0] != '*') break;
	}
	if (s[0] == '*') {
		Serial.println("*K30 Read ID Error");
		K30Serial = "--------";
	}
	else {
		K30Serial = s;
	}
}

//String requestID() {
//	//String serial = "000000000";
//	byte requestchar[] = { 0x24, 0x00, 0x28, 0x4C };
//
//	Wire.beginTransmission(co2Addr);
//	Wire.write(requestchar, 4);
//	if (Wire.endTransmission() != 0) return "*transFail";
//	delay(20);
//	Wire.requestFrom(co2Addr, 6);
//
//	byte i = 0;
//	byte buffer[6] = { 0, 0, 0, 0, 0, 0 };
//	unsigned long starttime = millis();
//	while (Wire.available())
//	{
//		if (i >= 6) return "*ovfFail";
//		buffer[i] = Wire.read();
//		i++;
//	}
//	if (millis() - starttime > 120) return "*ttlFail";
//
//	byte sum = 0; //Checksum Byte
//	sum = buffer[0] + buffer[1] + buffer[2] + buffer[3] + buffer[4]; //Byte addition utilizes overflow
//	if (sum != buffer[5]) return "*chksumFail";
//	else
//	{
//		char buf[9];
//		sprintf(buf, "%02X%02X%02X%02X", buffer[1], buffer[2], buffer[3], buffer[4]);
//		return String(buf);
//	}
//}


String sendata(byte addr, byte data[]) {
	byte length = data[0] & 0x0F;
	length += 2;
	byte crc = 0x00;

	Wire.beginTransmission(addr);
	for (int i = 0; i <= sizeof(data) / sizeof(byte); i++) {
		Wire.write(data[i]);
		crc = crc + data[i];
	}
	crc &= 0xFF;
	Wire.write(crc);

	if (Wire.endTransmission() != 0) return "*trans";

	delay(20);
	Wire.requestFrom(addr, length);

	byte* _return = 0;
	_return = (byte*)malloc(length * sizeof(byte));

	unsigned long starttime = millis();
	int returncount = 0;
	if (DEBUG || debugMode) Serial.print("$ I2C: ");
	while (Wire.available())
	{
		byte in = Wire.read();
		if (returncount >= length) {
			free(_return);
			return "*ovf";
		}
		else {
			_return[returncount] = in;
		}
		returncount++;
		if (DEBUG || debugMode) {
			char sz[6];
			sprintf(sz, "0x%02X\t", in);
			Serial.print(sz);
		}
	}
	if (DEBUG || debugMode) Serial.println();
	if (millis() - starttime > 120) {
		free(_return);
		return "*ttl";
	}
	crc = 0;
	for (int i = 0; i < length - 1; i++)	crc += _return[i];
	if (crc != _return[length - 1]) {
		free(_return);
		return "*crc";
	}
	String s = "";
	char _temp[3];
	for (int i = 1; i < length - 1; i++) {
		sprintf(_temp, "%02X", (byte)_return[i]);
		s += String(_temp);
	}
	free(_return);
	return s;
}

unsigned int StringtoUInt(String s) {
	unsigned int value = 0x0000;
	byte _temp = 0;
	for (int i = 0; i < s.length(); i++) {
		//judge is 0-9 or A-F
		_temp = s[i] - (s[i] < 58 ? 48 : 55);
		//or operate with 0b1111 to ensure only 1 bit
		value |= _temp & 0xF;
		//shift 1 bit
		if (i < s.length()-1)	value = value << 4;
	}
	return value;
}

int StringtoInt(String s) {
	int value = 0x0000;
	byte _temp = 0;
	for (int i = 0; i < s.length(); i++) {
		//judge is 0-9 or A-F
		_temp = s[i] - (s[i] < 58 ? 48 : 55);
		//& operate with 0b1111 to ensure only 1 bit
		value |= _temp & 0xF;
		//shift 1 bit
		if (i < s.length() - 1)	value = value << 4;
	}
	return value;
}
