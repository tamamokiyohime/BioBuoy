void CheckKeypadInput() {
	while (customKeypad.available())
	{
		keypadEvent e = customKeypad.read();
		char inputdata = (char)e.bit.KEY;
		if (e.bit.EVENT == KEY_JUST_PRESSED)
		{
			pad();
			switch (inputdata)
			{
				case '#':
				{
					data_available = true;
				}break;
				case '<':
				{
					if (data_index > 0)		data_index--;
					keypaddata[data_index] = '\0';
				}break;
				default:
				{
					if (inputdata == '.' && data_index == 2)	inputdata = '*';
					keypaddata[data_index] = inputdata;
					data_index++;
				}
				break;
			}
			if (data_index >= 10)	data_available = true;

			LCDUpdateRow1();
		}
	}
	if (data_available)
	{
		String resault;
		//JudgeKeypadEvent();
		lcd.setCursor(0, 1);
		lcd.print("                ");
		resault = KeypadStringDecode();
		Serial.println(resault);
		lcd.setCursor(0, 1);
		lcd.print(resault);
		//row1cleared = false;
		resetRow1.start();
		ResetKeypadData();
	}
	delay(10);
}

void ResetKeypadData() {
	data_available = false;
	data_index = 0;
	//Serial.println(String(keypaddata));
	for (int i = 0; i < 10; i++) {
		keypaddata[i] = '\0';
	}
}

String KeypadStringDecode() {
	//Take first letter as type
	//First, we convert data form char array to string and remove '\0'
	String str = String(keypaddata);
	char latter = keypaddata[0];
	int number = keypaddata[1] - '0';
	char star = keypaddata[2];
	String data;

	//First, we check the first data is latter A to C
	if (latter < 65 || latter > 67)	return "*fail [?__]";
	//Second, we check the second data is number
	if (number < 0 || number > 9)	return "*fail [_?_]";
	//Third, we check the third data is * or \0
	if (star == '*')	data = str.substring(3);
	else if (star == '\0') data = "nan";
	else return "*fail [__?]";

	//Switch the program to excute by latter
	switch (latter)
	{
		case 'A': //Calibration parameter
		{
			switch (number)
			{
		///Conductivity calibration action---
			case 1:
				ECCalibration("dry", NULL);
				break;
			case 2:
				star == '\0' ? ECCalibration( "low", 12880) : ECCalibration( "low", data.toInt());
				break;
			case 3:
				star == '\0' ? ECCalibration("high", 80000) : ECCalibration("high", data.toInt());
				break;
		///pH calibration action---
			case 4:
				star == '\0' ? pHCalibration( "mid",  7.0f) : pHCalibration( "mid", data.toFloat());
				break;
			case 5:
				star == '\0' ? pHCalibration( "low",  4.0f) : pHCalibration( "low", data.toFloat());
				break;
			case 6:
				star == '\0' ? pHCalibration("high", 10.0f) : pHCalibration("high", data.toFloat());
				break;
		///thermocouple calibration action---
			case 7:
				star == '\0' ? calibrateThermo(25.0f) : calibrateThermo(data.toFloat());
				break;
			default:
				return "*fail [Acmd]";
				break;
			}
		}
		break;
		case 'B':
		{
			switch (number)
			{
		///powersaving mode action---
				case 1:
				{
					String cmd = "P," + String(data.toInt()) + "\r";
					if(!SerialDecode(cmd)) return "*fail [B1]";
				}
					break;
		///Interval setting action---
				case 2:
				{
					String cmd = "I," + String(data.toInt()) + "\r";
					if (!SerialDecode(cmd)) return "*fail [B2]";
				}
					break;
		///Pump speed setting action---
				case 3:
				{
					updatePumpValue(data.toInt());
				}
					break;
				default:
					return "*fail [Bcmd]";
					break;
			}
		}
		break;
		case 'C':
		{
			switch (number)
			{
		///ID setting action---
			case 1:
			{
				String cmd = "D," + String(data.toInt()) + "\r";
				if (!SerialDecode(cmd)) return "*fail [C1]";
			}
			break;
		///EC print way 0:uS 1:salinity
			case 2:
			{
				String cmd = "S," + String(data.toInt()) + "\r";
				if (!SerialDecode(cmd)) return "*fail [C2]";
			}
			break;
			case 3: 
			{
				if (star == '\0') {
					return SyncRTCbyGPS(false);
				}
				else {
					if(data.equals("1234"))		return SyncRTCbyGPS(true);
					else return "*fail [C3]";
				}
				
			}
			break;
		///Restart
			case 7:
				delay(100);
				resetFunc();
				break;
		///Enable debug output---
			case 8:
			{
				debugMode = true;
				PrintEEPParameter();
			}
		///Reset all system action---
			case 9:
			{
				if (data.equals("ABC123")) init_System();
			}break;
			default:
				return "*fail [Ccmd]";
				break;
			}
		}
		break;
		default:
			return "*fail";
		break;
	}
	return "*OK";
}