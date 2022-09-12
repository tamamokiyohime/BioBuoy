void flushAllSerial() {
  flush_EC_serial();
  flush_pH_serial();
}

String floattostring(float value, int integer, int decimal) {
	//we make a char array, which size = 0
	char* str = 0;
	//resize the array to fit the input serial command length
	int len = integer + decimal + 2;
	str = (char*)malloc(len * sizeof(char));
	//convert the string to a char array
	if (isnan(value)) for (int i = 0; i < len - 1; i++) str[i] = '-';
	else dtostrf(value, len - 1, decimal, str);
	String s = String(str);
	free(str);
	return s;
}

String inttostring(int value, int integer) {
	char* str = 0;
	char* format = "%0d";
	format[1] = (char)(integer + '0');
	int len = integer + 1;
	str = (char*)malloc(len * sizeof(char));
	if (value == -9999) for (int i = 0; i < len - 1; i++) str[i] = '-';
	else sprintf(str, format, value);
	String s = String(str);
	free(str);
	return s;
}

String longtostring(long value, int integer) {
	char* str = 0;
	char* format = "%0ld";
	format[1] = (char)(integer + '0');
	int len = integer + 1;
	str = (char*)malloc(len * sizeof(char));
	if (value == -9999) for (int i = 0; i < len - 1; i++) str[i] = '-';
	else sprintf(str, format, value);
	String s = String(str);
	free(str);
	return s;
}