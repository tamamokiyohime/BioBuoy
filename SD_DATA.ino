void SD_checking() {
	//lcd_qSprint("SD.....", 0, 1);
	if (!SD.begin(chipSelect)) {
		Serial.println("SD Devive error");
		SD_alive = false;
		SDIO.resume();
		waring();
	}
	else SD_alive = true;
	//lcd_qSprint(SD_alive ? "OK >w<" : "**FAIL**", 7, 1);
}

void doMeasure() {
  SDIO.on();
  updateGPSdata();
  getID();
  measure_CO2();				//Get CO2 Data									 [CO2_READ]				(valCO2)
  tsys01_get();					//Get TSYS01 Data								 [TSYS01]				(temperature)
  measure_BME280();				//Get BME280 Data								 [BME280]				(pressure,humidity)
  update_K_temp();				//Get Thermacouple Temperature  				 [Thermocouple]			(K_temperature)
  getpHdata();
  getECdata();
  MeasureBatteryVoltage();
  do_SD();						//Write Data into SD Card						 [SD_DATA]
  LCD_update_main();			//Update LCD Data								 [LCD]
  //SDIO.off();
}


void do_SD() {
	if (!SD_alive)	SD_checking();
	if (!SD_alive)	return;

	if (last_day != day) 
	{
		last_day = day;
		getFilename(filename);
		File_check_exists();
	}

	String temp = String("");
	temp.reserve(170);
	temp += String("") + device_ID + "," + String(year, DEC) + "/" + String(month, DEC) + "/" + String(day, DEC) + "," + String(hour, DEC) + ":" + String(minute, DEC) + ":" + String(second, DEC) + ",";
	temp += String("") + String(temperature, 4) + "," + String(K_temperature, 4) + "," + String(pressure, 2) + "," + String(humidity) + "," + valCO2 + ",";
	temp += String("") + flat + "," + flon + "," + GPS_datetime + ",";
	//temp += String("") + EC_i + "," + TDS_i + "," + String(SAL_f, 2) + "," + String(GRAV_f, 3) + "," + String(pH, 3) + ",";
	temp += String("") + String(EC, 2) + "," + TDS + "," + String(SAL, 2) + "," + String(GRAV, 3) + "," + String(pH, 3) + ",";
	temp += K30Serial;
	temp += "," + String(fault, HEX) + ",";
	//temp += noFault ? ",T," : ",F,";
	temp += String(voltage);
	temp += ",";
	temp += freeMemory();

	myFile = SD.open(filename, FILE_WRITE);
	if (myFile) 
	{
		myFile.println(temp);
	}
	else 
	{
		Serial.println(F("Unable To Access SD Card"));
		waring();
		SD_alive = false;
	}
	myFile.close();
	//if (EC_dry_btnEvt || EC_high_btnEvt || EC_low_btnEvt || ph_4_btnEvt || ph_7_btnEvt || ph_10_btnEvt || k_cal_btnEvt)		EventLog();
	Serial.println(temp);
	if (SD_alive) {
		SDIO.off();
	}
	else {
		SDIO.clearBlink();
		SDIO.fastBlinking();
	}
}
void getFilename(char *filename) {      //File Name Set As : IDYYMMDD.csv !!Warning!! Filename should alway be "8digits".csv
	sprintf(filename, "%02X%02d%02d%02d.csv", device_ID, year % 100, month, day);
	return;
}
void File_check_exists() {              //Check wether there is already the same filename's file exsit. If not, creat new one.=======
  if (!SD.exists(filename)) {
    myFile = SD.open(filename, FILE_WRITE);
    myFile.println("Device ID,Date,Time,Temperature,K_Temp,Pressure,Humidity,x_CO2,flat,flon,GPS Datetime,Conductivity,TDS,SAL,GRAV,pH,CO2Serial,Thermo Flag,Battery Voltage,Ram");
    myFile.println("ID,YY/MM/DD,HH/MM/SS,Celsius,Celsius,Pa,%,ppm,degree,degree,GMT(0),uS/cm,ppm,PSU,-,-,-,HEX,V,-");
    myFile.close();
  }
}
void data_write_in(float datalog, int num) {    //Write in float type data into SD Card=======================================
	if		(datalog == 8888)	myFile.print("Not_Connecting");
	else if (datalog == 9999)	myFile.print("-9999");
	else myFile.print(datalog, num);
	myFile.print(",");
}

