/*
void AtlasCalibration(int command) {
	wakeAtlas();
	delay(50);

    update_K_temp();
    send_temperature();
    lcd.setCursor(0, 1);
    lcd.print("                 ");
    notice();
	

    switch (command) {
      case 1:
        flush_EC_serial();
        Atlas_EC.print("Cal,dry\r");
        Serial.println("Atlas EC \"Dry\" Calibration");
        lcd.setCursor(1, 3);
        lcd.print("-Dry ");
        lcd.setCursor(0, 1);
        lcd.print("EC Dry Cal");
        EC_dry_btnEvt = true;
        break;

      case 2:
        flush_EC_serial();
        Atlas_EC.print("Cal,low,12880\r");
        Serial.println("Atlas EC \"LOW\" Calibration (12880)");
        lcd.setCursor(1, 3);
        lcd.print("-Low ");
        lcd.setCursor(0, 1);
        lcd.print("EC Low Cal 12.8k");
        EC_low_btnEvt = true;
        break;

      case 3:
        flush_EC_serial();
        Atlas_EC.print("Cal,high,80000\r");
        Serial.println("Atlas EC \"HIGH\" Calibration (80000)");
        lcd.setCursor(1, 3);
        lcd.print("-High");
        lcd.setCursor(0, 1);
        lcd.print("EC High Cal 150k");
        EC_high_btnEvt = true;
        break;

      case 4:
        flush_pH_serial();
        Atlas_pH.print("Cal,mid,7.0\r");
        Serial.println("Atlas pH \"MID\" Calibration (7.0)");
        lcd.setCursor(8, 3);
        lcd.print("-Mid ");
        lcd.setCursor(0, 1);
        lcd.print("pH7.0 Mid Cal");
        ph_7_btnEvt = true;
        break;

      case 5:
        flush_pH_serial();
        Atlas_pH.print("Cal,low,4.0\r");
        Serial.println("Atlas pH \"LOW\" Calibration (4.0)");
        lcd.setCursor(8, 3);
        lcd.print("-Low ");
        lcd.setCursor(0, 1);
        lcd.print("pH4.0 Low Cal");
        ph_4_btnEvt = true;
        break;

      case 6:
        flush_pH_serial();
        Atlas_pH.print("Cal,high,10.0\r");
        Serial.println("Atlas pH \"HIGH\" Calibration (10.0)");
        lcd.setCursor(8, 3);
        lcd.print("-High");
        lcd.setCursor(0, 1);
        lcd.print("pH10.0 High Cal");
        ph_10_btnEvt = true;
        break;

      case 7:
        calibrateThermo(25.00);
        break;
    }
    delay(50);
	sleepAtlas();
  
}

*/


