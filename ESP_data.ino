//void upload_ESP() {
//  //  digitalWrite(esp_RST_pin, HIGH);
//  //  delay(1);
//  //  digitalWrite(esp_RST_pin, LOW);
//  //  delay(300);
//  char INSERT_DATA[] = "%02d/%02d/%02d,%02d:%02d:%02d,%s,%s,%s,%d,%d,%s,%s,%s,%s,%s,%d\r";
//  char query[256];
//  char temp1[10], temp2[10], temp3[10], temp4[10], temp5[10], temp6[10], temp7[10], temp8[10], temp9[10], temp10[10], temp11[10];
//
//  dtostrf(temperature, 2, 2, temp1);
//  dtostrf(K_temperature, 2, 2, temp2);
//  dtostrf(pressure, 2, 2, temp3);
//  //  dtostrf(flat,1,2,temp4);
//  //  dtostrf(flon,1,2,temp5);
//  //  dtostrf(alti,1,2,temp6);
//  dtostrf(EC_f, 1, 2, temp7);
//  dtostrf(TDS_f, 1, 2, temp8);
//  dtostrf(SAL_f, 1, 2, temp9);
//  dtostrf(GRAV_f, 1, 2, temp10);
//  dtostrf(pH, 1, 3, temp11);
//
//
//  sprintf(query, INSERT_DATA, year, month, day, hour, minute, second, temp1, temp2, temp3, humidity, valCO2, temp7, temp8, temp9, temp10, temp11, device_ID);
//  Serial.print("query = ");
//  Serial.println(query);
//  ESP8266.print(query);
//}
//
//void PushData() {
//  sendCommandToWifi("AT+RST");
//  String DATA = "GET /test.php?temp=" + String(temperature) + "&ktemp=" + String(K_temperature) + "&press=" + String(pressure) + "&humi=" + humidity + "&co=" + valCO2 + "&ec=" + String(EC_f) + "&tds=" + String(TDS_f) + "&sal=" + String(SAL_f) + "&grav=" + String(GRAV_f) + "&ph=" + String(pH,3) + "\r\n";
//  delay(3000);
//  Watchdog.reset();
//  delay(4000);
//  sendCommandToWifi("AT+CIPSTART=\"TCP\",\"140.117.59.26\",80");
//  //  if (ESP8266.find("Error"))
//  //  {
//  //    Serial.println("CIPSTART error");
//  //    return;
//  //  }
//  delay(500);
//  sendCommandToWifi("AT+CIPSEND=" + String(DATA.length() + 1));
//  delay(500);
//  //  if (ESP8266.find(">"))
//  //  {
//  sendCommandToWifi(DATA);
//  //delay(500);
//  //Serial.println(query);
//  //  }
//  //  else
//  //  {
//  //    sendCommandToWifi("AT+CIPCLOSE");
//  //  }
//  //sendCommandToWifi("AT+CWQAP");
//}
//
//void sendCommandToWifi(String s) {
//  ESP8266.println(s);  //向ESP8266傳送字串s
//  Serial.println(s);  //顯示字串s到監控視窗
//}
//
