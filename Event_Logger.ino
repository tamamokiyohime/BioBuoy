void EventLog() {
  char Eve_time[18];
  int temp_year = year % 100;
  sprintf(Eve_time, "%02d/%02d/%02d %02d:%02d:%02d", temp_year, month, day, hour, minute, second);
  myFile = SD.open("EventLog.txt", FILE_WRITE);
  if (myFile) {
    myFile.print(Eve_time);
    myFile.print(" >>> ");

    if (EC_dry_btnEvt) {
      myFile.print("EC Dry Calibration Btn Pressed\n");
    }
    if (EC_low_btnEvt) {
      myFile.print("EC Low Calibration Btn Pressed\n");
    }
    if (EC_high_btnEvt) {
      myFile.print("EC Low Calibration Btn Pressed\n");
    }
    if (ph_4_btnEvt) {
      myFile.print("pH 4 Calibration Btn Pressed\n");
    }
    if (ph_7_btnEvt) {
      myFile.print("pH 7 Calibration Btn Pressed\n");
    }
    if (ph_10_btnEvt) {
      myFile.print("pH 10 Calibration Btn Pressed\n");
    }
    if (k_cal_btnEvt) {
      myFile.print("K-Thermocouple Calibration Btn Pressed\n");
    }

    EC_dry_btnEvt = false;
    EC_low_btnEvt = false;
    EC_high_btnEvt = false;
    ph_4_btnEvt = false;
    ph_7_btnEvt = false;
    ph_10_btnEvt = false;
    k_cal_btnEvt = false;
    myFile.close();
    Serial.println("Event Logged");
  }
  else {
    Serial.println("SD Error");
  }


}


