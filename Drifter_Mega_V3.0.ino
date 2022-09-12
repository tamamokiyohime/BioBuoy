/*Change Log
   190615		 Change pH SD logging digi from 2 to 3
   190724		 Add Serial Command Decode Function (RTC Adjust Function, Interval Adjust Function)
   190810		 Trying to fix EC data and GPS
				 Replace seeed_BME280.h to Costom Adafruit_BME280.h
				 Add function let System can still operate when Air Moudle isn't plugin
				 Serial Command add Thermocouple Calibration function
   190818		 Fix Conductivetiy Value Problem
				 Change SD_DATA and SERIAL_PRINT method
   200128 V2.4.1 Add LCD's LED Auto off function
				 Add LCD's LED Auto off serial command "P,_"
				 Fix the problem which date data is 0 when first logging after boot up
				 Change Serial printing and SD logging method
   200202 V3.0.0 Change buttoms to keypad
   200206 V3.0.0 Adjust GPS Data Format And gps_number to gps_valid
   200308 V3.0.1 Add Atlas sleep mode
   200311 V3.0.2 Prevent wrong pH and Conductivity reading from Thermocouple reading error
   200317 V3.1.0
   200408 V3.1.1 fix calibration problem cause by atlas sleep mode
   210419 V3.2.0 Add K30 Serial data and thermocouple flag to file data
   210714 V4.0.0 修正Atlas透過按鍵校正時沒有正確校正問題
				 新增電池電壓監測
				 記錄檔中新增GPS時間與電池電壓
				 移除LCD面板SD卡、RTC與GPS運作狀態指示
				 新增LED狀態顯示燈
				 EEPROM增加電池電壓上下限、電池電壓量測校正係數
   210720 V4.0.1 修改熱電偶錯誤紀錄資料及錯誤偵測方法
   210726 V4.1.0 增加透過GPS調整時間功能(C3)
   210731 V4.1.1 嘗試修正I2C造成CH340記憶體下降問題
   210801 V4.1.1 修改熱電偶錯誤時LCD顯示方式
   220701 V4.1.2 新增pCO2壓力保護模式
   220712 V4.1.3 修改millis()與interval比較法成絕對值，避免millis逸位造成不做動
   220712 V4.1.4 修改pin6,7,8頻率為62kHz
   220819 V4.2.0 增加SensorFactoryReset()中EC顯示方式預設值寫入
*/

#include <TinyGPS++.h>
#include <BlinkControl.h>
#include "TimedAction.h"
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "RTClib.h"
#include <tsys01.h>
#include <Adafruit_MAX31856.h>
#include <Button.h>
#include <EEPROM.h>
#include <Adafruit_SleepyDog.h>
#include <Adafruit_Keypad.h>
#include <Countimer.h>

#define DEBUG false
bool debugMode = false;


#define ROWS 4 // rows
#define COLS 4 // columns
//define the symbols on the buttons of the keypads
const char keys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'.', '0', '#', '<'}
};
byte rowPins[ROWS] = { 43, 41, 39, 37}; //connect to the row pinouts of the keypad
byte colPins[COLS] = { 42, 40, 38, 36}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Adafruit_Keypad customKeypad = Adafruit_Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
char keypaddata[10] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
int data_index = 0;
bool data_available = false;
//bool row1cleared = true;

Countimer resetRow1;

#define SERIAL_BAUD 115200          	//Baud Rate
#define LCD_I2C_ADDRESS 0x27       	//LCD I2C Address
#define initial_data 0
#define GPS_SERIAL 9600
#define ESP_BAUD 115200
#define Drifter_Ver "V4.2.0"


/*========================Things You Should Change================================*/
/**/unsigned long     interval = 10000;           	//Default Interval					/**/
/**/#define CO2_cali_coeff 1          	//CO2 Calibration Coefficient			/**/
/**/#define valMultiplier 1				//CO2 Coffiect							/**/
/**/#define sample_times 3.000			//Sample Times For Average				/**/
/*================================================================================*/

#define update_System_temp false
#define initial false
#define device_ID_temp 1
#define k_cal_temp  0.00f

unsigned long last_time = 0;		//For Timed Action Group 1
unsigned long last_time2 = 0;		//For Timed Action Group 2
unsigned long lasttime_powersaving = 0;

//GPS DATA Parameter=================================================================================
//TinyGPSPlus gps;
TinyGPSPlus gps;
HardwareSerial &ss = Serial1;							//GPS Using UART Port1
String flat = "", flon = "";
String GPS_datetime = "";
byte  GGADisable[] = { 0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x00, 0x00, 0xFA, 0x0F};
byte  GLLDisable[] = { 0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x01, 0x00, 0xFB, 0x11};
byte  GSADisable[] = { 0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x02, 0x00, 0xFC, 0x13};
byte  GSVDisable[] = { 0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x03, 0x00, 0xFD, 0x15};
byte  VTGDisable[] = { 0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x05, 0x00, 0xFF, 0x19};
//GPS DATA Parameter END=============================================================================

//K Type Thermocouple Data Parameter=================================================================
#define MAX31856Select 53							//Max31856 CS pin
#define MAX31856FT 46
#define MAX31856DR 47
#define MAX31856PW 49
float K_temperature = 25.0f;
//bool noFault = true;
uint8_t fault;
Adafruit_MAX31856 max = Adafruit_MAX31856(MAX31856Select);// Use software SPI: CS, DI, DO, CLK
//K Type Thermocouple Data Parameter END=============================================================

//Tsys01 Data Parameter==============================================================================
static tsys01 m_tsys01;
float temperature;
boolean connected;
//Tsys01 Data Parameter END==========================================================================

//Atlas Conductivity Data Parameter==================================================================
HardwareSerial &Atlas_EC = Serial2;						//Atlas EC Using UART Port2
String  strEC = ""; 								//a string to hold the data from the Atlas Scientific product
boolean sensor_string_complete = false;					//have we received all the data from the Atlas Scientific product
//float SAL_f, GRAV_f;
//unsigned long EC_i, TDS_i;
float EC, SAL, GRAV;
unsigned int TDS;
//Atlas Conductivity Data Parameter END===============================================================

//Atlas pH Data Parameter=============================================================================
HardwareSerial &Atlas_pH = Serial3;
//String  strpH = "";							//a string to hold the data from the Atlas Scientific product
//boolean sensor_string_pH_complete = false;				//have we received all the data from the Atlas Scientific product
float pH;
//Atlas pH Data Parameter END=========================================================================

//SD Moudle Data Parameter============================================================================
#define chipSelect 4									//SD Moudle CS pin
File myFile;
char filename[] = "00000000.csv";
boolean SD_alive = true;
const int logger_size = 100;
char event_logger[logger_size];
int current_pos = 0;
//SD Moudle Data Parameter END========================================================================

//CO2 Data Parameter==================================================================================
//HardwareSerial &K_30_Serial = Serial3;					//CO2 Using UART Port3
int valCO2 = initial_data;
//const byte co2Addr = 0x7F;
#define co2Addr 0x7F
//byte  readCO2[] = { 0xFE, 0X44, 0X00, 0X08, 0X02, 0X9F, 0X25 };
//byte response[] = { 0, 0, 0, 0, 0, 0, 0 };
bool readCO2Success = true;
String K30Serial = "";
//CO2 Data Parameter END==============================================================================

//RTC Data Parameter==================================================================================

#define DS3231
//#define DS1307

#ifdef DS3231
	RTC_DS3231 RTC;
#endif // DS3231
#ifdef DS1307
	RTC_DS1307 RTC;
#endif // DS1307
int year, month, day, last_day;
int hour, minute, second;
//RTC Data Parameter END==============================================================================

//BME280 Data Parameter===============================================================================
Adafruit_BME280 bme280;
float pressure;
int humidity;
float pressure_in_atm;
boolean bme280connected;
//BME280 Data Parameter END===========================================================================

//Button Data Parameter===============================================================================
bool EC_dry_btnEvt = false;
bool EC_low_btnEvt = false;
bool EC_high_btnEvt = false;	
bool ph_4_btnEvt = false;	
bool ph_7_btnEvt = false;	
bool ph_10_btnEvt = false;	
bool k_cal_btnEvt = false;
int btn_command = 0;
//Button Data Parameter END===========================================================================

template <class T> bool UpdateEEP(int addr, const T value);
template <class T> void   ReadEEP(int addr, const T &value);

//EEPROM Data Parameter================================================================================
const int IDAddress = 0;

/*
sizeof(int) = 2
sizeof(float) = 4
sizeof(double) = 4
sizeof(bool) = 1
*/
const int k_cal_addr = sizeof(int);
const int interval_addr = 3 * sizeof(int);
const int power_addr = 5 * sizeof(int);
const int pumpPower_addr = 9 * sizeof(int);
const int voltage_min_addr = 12 * sizeof(int);
const int voltage_max_addr = 15 * sizeof(int);
const int voltage_slope_addr = 18 * sizeof(int);
const int pH_low_addr = 21 * sizeof(int);
const int pH_mid_addr = 24 * sizeof(int);
const int pH_high_addr = 27 * sizeof(int);
const int ec_low_addr = 31 * sizeof(int);
const int ec_high_addr = 34 * sizeof(int);
const int ec_print_addr = 36 * sizeof(int);
const int pCO2_lowP_addr = 40 * sizeof(int);
const int pCO2_highP_addr = 43 * sizeof(int);
const int pCO2_PSampleT_addr = 46 * sizeof(int);
const int pCO2_PRecoverMode_addr = 48 * sizeof(int);

int device_ID;
float k_cal;
float V_min;
float V_max;
int ec_print;
float pCO2_lowP, pCO2_highP;
int pCO2_PSampleT, _pCO2_PSampleT = 0;
bool pCO2_PRecoverMode = false;
//EEPROM Data Parameter END============================================================================

//LCD Data Parameter===================================================================================
LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, 20, 4);
boolean powerSaving = false;
boolean RTC_alive = true;

byte    EC_Stand[] = { 0x1F,  0x15,  0x15,  0x00,  0x1F,  0x11,  0x1B,  0x00 };
byte    ES_Stand[] = { 0x1F,  0x15,  0x15,  0x00,  0x17,  0x15,  0x1D,  0x00 };
byte    pH_Stand[] = { 0x1F,  0x05,  0x07,  0x00,  0x1F,  0x04,  0x1F,  0x00 };
byte	    xco2[] = { 0x1B,  0x04,  0x1B,  0x00,  0x1F,  0x11,  0x1B,  0x00 };
byte tsys01_icon[] = { 0x01,  0x1F,  0x01,  0x00,  0x17,  0x15,  0x1D,  0x00 };
byte      k_icon[] = { 0x1F,  0x04,  0x1B,  0x00,  0x01,  0x1F,  0x01,  0x00 };
byte k_fail_icon[] = { 0x05,  0x02,  0x05,  0x10,  0x10,  0x14,  0x18,  0x14 };
byte    ATM_icon[] = { 0x1E,  0x05,  0x1E,  0x00,  0x01,  0x1F,  0x01,  0x00 };
byte     RH_icon[] = { 0x1F,  0x05,  0x1B,  0x00,  0x1F,  0x04,  0x1F,  0x00 };
//byte    SD_Fatal[] = { 0x05,  0x02,  0x05,  0x0E,  0x10,  0x0C,  0x02,  0x1C };
//byte   RTC_Fatal[] = { 0x05,  0x02,  0x05,  0x18,  0x14,  0x18,  0x14,  0x14 };
//LCD Data Parameter END===============================================================================


//tone Data Parameter===================================================================================
#define buzzerpin 45
//Buzzer Data Parameter END===============================================================================

#define pump_pin 6
int pumpPower = 0;

//status led pin
BlinkControl battery(27);
BlinkControl SDIO(29);
BlinkControl ThermoER(31);
//BlinkControl RTCER(33);
BlinkControl PGER(33);
BlinkControl RamER(35);


//Watch Dog============================
int watchdogTimer = 10000;
//Watch Dog End========================

//ESP 8266=============================
//SoftwareSerial ESP8266(12, 13);
// ESP_times = 0;
//=====================================

//SerialEvent==========================
String strSerial = "";
bool serialComplete = false;
//=====================================

//Battery Voltage Measurment
float voltage = 0.0f;
float voltage_slope = 0.0f;
//#define voltage_in_percent (voltage - V_min) / (V_max - V_min) * 100
#define batteryV_pin A13

void(*resetFunc) (void) = 0; //declare reset function @ address 0

void setup() {
	strSerial.reserve(50);
	strEC.reserve(20);                           //set aside some bytes for receiving data from Atlas Scientific product
	//strpH.reserve(20);                        //set aside some bytes for receiving data from Atlas Scientific product
	K30Serial.reserve(10);
	flat.reserve(15);
	flon.reserve(15);
	GPS_datetime.reserve(25);

	pinMode(buzzerpin, OUTPUT);
	pinMode(MAX31856FT, INPUT);
	pinMode(MAX31856DR, INPUT);
	pinMode(MAX31856PW, OUTPUT);
	digitalWrite(MAX31856PW, HIGH);
	pinMode(pump_pin, OUTPUT);
	digitalWrite(pump_pin, LOW);
	pinMode(batteryV_pin, INPUT);

	//Set pin6 PWM to Fast PMW mode (62kHz), affect to pin 6,7,8
	TCCR4A = 0xA9;
	TCCR4B = 0x09;

	Serial.begin(SERIAL_BAUD);
	while (Serial.available() > 0) Serial.read();
	Wire.begin();
	
	
	lcd.init();
	lcd.createChar(1, pH_Stand);
	lcd.createChar(2, xco2);
	lcd.createChar(3, EC_Stand);
	lcd.createChar(4, tsys01_icon);
	lcd.createChar(5, k_icon);
	lcd.createChar(6, ATM_icon);
	lcd.createChar(7, RH_icon);
	lcd.backlight();
	lcd.clear();

	LCDSerialPrint(F("LED Test"));
	battery.begin();
	SDIO.begin(); 
	ThermoER.begin(); 
	//RTCER.begin(); 
	PGER.begin();
	RamER.begin();
	battery.on();	delay(50);	battery.off();  battery.blink3();		battery.pause();
	SDIO.on();		delay(50);	SDIO.off();		SDIO.fastBlinking();	SDIO.pause();
	ThermoER.on();	delay(50);	ThermoER.off(); ThermoER.blink3();		ThermoER.pause();
	//RTCER.on();		delay(50);	RTCER.off();	RTCER.blink3();			RTCER.pause();
	PGER.on();		delay(50);	PGER.off();		PGER.fastBlinking();	PGER.pause();
	RamER.on();		delay(50);	RamER.off();	RamER.fastBlinking();	RamER.pause();

	LCDSerialPrint(F("Sensor Activate"));
	ss.begin(GPS_SERIAL);
	max.begin();
	max.setThermocoupleType(MAX31856_TCTYPE_K);		//Set the type of Thermocouple
	max.setTempFaultThreshholds(10.0, 100.0);
	Atlas_EC.begin(9600);                	            //set baud rate for the software serial port to 9600
	Atlas_pH.begin(9600);                               //set baud rate for the software serial port to 9600
	customKeypad.begin();
	m_tsys01.begin();
	bme280.begin(0x76, &Wire);
	resetRow1.setCounter(0, 0, 3, resetRow1.COUNT_DOWN, RemoveBTNmessage);
	resetRow1.setInterval(NULL, 1000);
	delay(50);

	LCDSerialPrint(F("Disable partial GPS Data"));
	ss.write(GGADisable, 11);
	ss.write(GLLDisable, 11);
	ss.write(GSADisable, 11);
	ss.write(GSVDisable, 11);
	ss.write(VTGDisable, 11);
	delay(50);

	LCDSerialPrint(F("Get EEPROM data"));
	PrintEEPParameter();
	delay(50);

	LCDSerialPrint(F("RTC Checking"));
	RTC_check();
	delay(50);

	LCDSerialPrint(F("SD Checking"));
	SD_checking();
	delay(50);

	if (SD_alive && RTC_alive)  {		LCDSerialPrint(F("SD and RTC Pass"));		delay(50);	}
	else						{		LCDSerialPrint(F("SD or RTC error"));		delay(500);	}

	lcd.setCursor(0, 0);
	lcd.print(F("Biochemistry    Buoy"));
	lcd.setCursor(0, 1);
	lcd.print(F("Data Collecting Syst"));
	lcd.setCursor(0, 2);
	lcd.print(Drifter_Ver);
	lcd.setCursor(0, 3);
	lcd.print(F("Device ID = "));
	lcd.print(device_ID);
	delay(1000);
	LCDSerialPrint(F("Open pump"));
	pumpControl(pumpPower);
	delay(50);
	if (powerSaving) {
		Serial.println(F("Waring! Power Saving ON, LCD's LED will shutdown after booting!"));
		notice();
		LCDSerialPrint(F("POWER SAVING ON"));
		delay(500);
		LCDSerialPrint(F("LCD's LED will TURN OFF after booting"));
		delay(500);
	}
	LCDSerialPrint(F("Watchdog activate"));
	Watchdog.enable(watchdogTimer);
	delay(50);
///Initial Parameter==================================================
	//Serial.println("First time measurement");
	update_time_info();
	getFilename(filename);
	File_check_exists();
	last_day = day;
	flushAllSerial();
	doMeasure();
	last_time = millis();
	last_time2 = millis();
	Watchdog.reset();

	if (powerSaving) lcd.noBacklight();
}

void loop() {
 ///Call in Every Loop ====================================================================
	Watchdog.reset();
	gps_new_get();					//GPS Function									 [GPS_DATA]
	resetRow1.run();
	IOloop();
	SerialDecodeLoop();
///Update By Interval Set in "interval" ==================================================
	if (abs(millis() - last_time) >= interval)
	{
		last_time = millis();		//Set Time for Time Action
		last_time2 = millis();		//Set Time for Time Action
		doMeasure();
		checkSRam();
	}
///Update By Interval of 1s ==============================================================
	if (abs(millis() - last_time2) >= 1000)
	{
		last_time2 = millis();		//Set Time for Time Action
		update_time_info();			//Get New Time  								 [RTC]
	}
///LED backlight control in powersaving mode================================================
	if (powerSaving) 
	{
		unsigned long temp = abs(millis() - lasttime_powersaving);
		if (temp >= 600000)
		{
			if (temp <= 610000) {
				lcd.backlight();
			}
			else {
				lcd.noBacklight();
				lasttime_powersaving = millis();
			}
		}
	}
	SRamRestart();
}

void serialEvent() {
	while (Serial.available()) 
	{
		char inChar = (char)Serial.read();
		strSerial += inChar;
		if (inChar == '\r')
		{
			serialComplete = true;
		}
	}
}

void SerialDecodeLoop() {
	if (serialComplete) {
		SerialDecode(strSerial);
		strSerial = "";
		serialComplete = false;
	}
}

void IOloop() {
	battery.loop();
	SDIO.loop();
	ThermoER.loop();
	//RTCER.loop();
	//GPSLED.loop();
	PGER.loop();
	RamER.loop();
	customKeypad.tick();
	CheckKeypadInput();
}
