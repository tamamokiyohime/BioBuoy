bool SerialDecode(String strIn)
{
	bool result = true;
	//Serial Command format = [command],[data]\r
	//First, check is there only contain one comma...
	int len = strIn.length() + 1;
	int comma_count = 0;
	for (int i = 0; i < len; i++)
	{
		if (strIn[i] == ',') comma_count++;
		if (comma_count > 1) return false;
	}
	if (comma_count == 0) return false;
	//bool returnvalue = true;

	//we make a char array, which size = 0
	char* str_arr = 0;
	//resize the array to fit the input serial command length
	str_arr = (char*)malloc(len * sizeof(char));
	//convert the string to a char array
	strIn.toCharArray(str_arr, len);
	//Separate the serial command to command and data by ','
	char* command;
	char* data;
	command = strtok(str_arr, ",");
	data = strtok(NULL, ",");
	//Print out the separated data for checking
	if (DEBUG || debugMode) {
		Serial.print("INPUT COMMAND = ");
		Serial.println(command);
		Serial.print("DATA = ");
		Serial.println(data);
	}
	//resetRow1.start();
	//Start to judge the command and data
	switch (command[0])
	{
		///Set battery voltage slope
		case 'C':
		{
			float value = atof(data);
			UpdateEEP(voltage_slope_addr, value);
			PrintEEPParameter();
		}
		break;
		///Set ID, format = D,[ID]\r
		case 'D':
		{
			int temp = 0;
			int IDs = atoi(data);
			UpdateEEP(IDAddress, IDs);
			PrintEEPParameter();
			getFilename(filename);
			File_check_exists();
		}
		break;
		///Set pressure guard threshold, format = G,low_value;high_value;_test_time;recover_mode
		case 'G':
		{
			setPGuardbystr(data);
		}
		break;
		///Measurement Interval command, format = I,[sec]\r
		case 'I':
		{
			long new_interval = atoi(data) * 1000;
			if (atoi(data) >= 10)
			{
				UpdateEEP(interval_addr, new_interval);
				PrintEEPParameter();
			}
			else {
				Serial.println("Wrong Interval Setting, Interval Should >= 10 sec");
				result = false;
			}
		}
		break;
		///Thermocouple calibration, format = K,[temperature]\r
		case 'K':
		{
			calibrateThermo(atof(data));
			PrintEEPParameter();
		}
		break;
		///Set pump speed, format = M,[value]\r
		case 'M':
		{
			int pwmvalue = atoi(data);
			if (pwmvalue > 100 || pwmvalue < 0) result = false;
			updatePumpValue(pwmvalue);
		}
		break;
		///Power saving mode. format = P,[on(1)/off(0)]\r
		case 'P':
		{
			if (atoi(data) == 1)
			{
				UpdateEEP(power_addr, 1);
				PrintEEPParameter();
				lasttime_powersaving = millis();
				Serial.println("Power Saving Mode ON");
				lcd.noBacklight();
			}
			else if (atoi(data) == 0)
			{
				UpdateEEP(power_addr, 0);
				PrintEEPParameter();
				Serial.println("Power Saving Mode OFF");
				lcd.backlight();
			}
			else
			{
				result = false;
			}
		}
		break;
		///Set EC printing mode
		case 'S':
		{
			int value = atoi(data) % 2;
			UpdateEEP(ec_print_addr, value);
			PrintEEPParameter();
		}
		break;
		///Time settimg command, format = T,[yyyy]-[MM]-[dd] [hh]:[mm]:[ss]\r
		case 'T'://no wrong data input protection
		{
			///we check is there 2 '-' and 2 ':'
			int hyphen = 0, colon = 0;
			for (int i = 0; i < strlen(data); i++)
			{
				if (data[i] == '-')	hyphen++;
				if (data[i] == ':')	colon++;
			}
			if (hyphen != 2 || colon != 2) result = false;
			RTC.adjust(str2datetime(data));
			Serial.println("Time Adjust Success");
		}
		break;
		///Set min and max voltage for input, formate = V,[min]-[max]\r
		case 'V':
		{
			float tempmin, tempmax;
			char *cVmin, *cVmax;
			cVmin = strtok(data, "-");
			cVmax = strtok(NULL, "-");
			tempmin = atof(cVmin);
			tempmax = atof(cVmax);
			UpdateEEP(voltage_min_addr, tempmin);
			UpdateEEP(voltage_max_addr, tempmax);
			PrintEEPParameter();
		}
		break;
		default:
			Serial.println("*failed");
			result = false;
	}
	free(str_arr);
	return result;
}

void SerialDebug(String title, String value) {
	if (DEBUG || debugMode)
	{
		Serial.print("$ ");
		Serial.print(title);
		Serial.println(value);
	}
}