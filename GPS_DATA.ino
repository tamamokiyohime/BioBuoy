void gps_new_get() {
/// For one second we parse GPS data and report some key values
	for (unsigned long start = millis(); millis() - start < 100;)	if(ss.available() > 0)		gps.encode(ss.read());
}

void updateGPSdata() {
///update location data
	if (gps.location.isValid())
	{
		//GPSLED.resume();
		flat = floattostring(gps.location.lat(), 2, 6);
		flon = floattostring(gps.location.lng(), 3, 6);
	}
	else 
	{
		//GPSLED.pause();
		flat = "-";
		flon = "-";
	}
///update datetime data
	if (gps.date.isValid() && gps.time.isValid())
	{
		char sz[22];
		sprintf(sz, "%04d/%02d/%02d %02d:%02d:%02d", gps.date.year(), gps.date.month(), gps.date.day(), gps.time.hour(), gps.time.minute(), gps.time.second());
		GPS_datetime = String(sz);
	}
	else 
	{
		GPS_datetime = "-";
	}
	
}
