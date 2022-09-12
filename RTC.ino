void RTC_check() {
	//lcd_qSprint("RTC....", 0, 0);
	RTC_alive = true;
	if (!RTC.begin())
	{
		Serial.println(F("RTC Not Found"));
		RTC_alive = false;
		waring();
	}
	bool RTCrunning = false;

	#ifdef DS3231
	RTCrunning = RTC.lostPower();
	#endif // DS3231
	#ifdef DS1307
	RTCrunning = RTC.isrunning();
	#endif // DS1307

	if (!RTCrunning) 
		{
		Serial.println(F("!!!RTC TIME ERROW!!!.... Connect to PC to Reset"));
		//RTC_alive = false;
		waring();
		//RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));         //<<<<Uncomment to Set Time Via PC
	}
	//!RTC_alive ? RTCER.resume() : RTCER.pause();
}


void update_time_info() {
  DateTime now = RTC.now();
  year = now.year();  month = now.month();   day = now.day();
  hour = now.hour();  minute = now.minute(); second = now.second();
  char time_format[18];
  //int temp_year = year % 100;
  sprintf(time_format, "%02d/%02d/%02d %02d:%02d:%02d", year % 100, month, day, hour, minute, second);
  //sprintf(time_format, "%02d/%02d/%02d %02d:%02d:%02d", now.year()%100, now.month(), now.day(), now.hour(), now.minute(), now.second());
  lcd_qSprint(time_format, 0, 0);
}

DateTime str2datetime(const char* data) {
	int _Year, _Month, _Day, _Hour, _Minute, _Second;
	sscanf(data, "%d-%d-%d %d:%d:%d", &_Year, &_Month, &_Day, &_Hour, &_Minute, &_Second);
	DateTime value = DateTime(_Year, _Month, _Day, _Hour, _Minute, _Second);
	return value;
}

String SyncRTCbyGPS(bool applysync) {
	if (gps.date.isValid() && gps.time.isValid()) 
	{
		//Check GPS datetime data is not 2000/00/00 00:00:00
		if (gps.date.year() == 2000 || gps.date.month()&gps.date.day()&gps.time.hour()&gps.time.minute()&gps.time.second() == 0) {
			SerialDebug("GPS Sync fail:", "check valid");
			return "*Sync[f:2]";
		}
		
		DateTime GPS = DateTime(gps.date.year(), gps.date.month(), gps.date.day(), gps.time.hour(), gps.time.minute(), gps.time.second());
		GPS = GPS + TimeSpan(0, 8, 0, 0);
		//gps.date.year(), gps.date.month(), gps.date.day(), gps.time.hour(), gps.time.minute(), gps.time.second()
		if (!applysync) 
		{
			char sz[18];
			sprintf(sz, "%02d/%02d/%02d %02d:%02d:%02d", GPS.year()%100, GPS.month(), GPS.day(), GPS.hour(), GPS.minute(), GPS.second());
			return String(sz);
		}
		else 
		{
			RTC.adjust(GPS);
			SerialDebug("RTC Adjusted", "");
		}
	}
	else 
	{
		SerialDebug("GPS Sync fail:", "GPS valid check");
		return "*Sync[f:3]";
	}
	return "*OK";
}