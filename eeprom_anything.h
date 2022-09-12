// eeprom_anything.h.h

#ifndef eeprom_anything_h
#define eeprom_anything_h

#include <Arduino.h>
#include <EEPROM.h>

// 利用這兩個C++模板，讀寫整組資料
// 參數address是EEPROM位址，參數data是想讀寫的資料結構
template <class T> int EEPROM_writeAnything(int address, const T &data);
template <class T> int EEPROM_readAnything(int address, T &data);

// 函式模板的定義
// 使用EEPROM程式庫寫入一個一個的byte
template <class T> int EEPROM_writeAnything(int address, const T &data)
{
	const byte *p = (const byte *)(const void *)&data;
	int i, n;
	for (i = 0, n = sizeof(data); i < n; i++)
		EEPROM.write(address++, *p++);
	return i;
}
template <class T> int EEPROM_readAnything(int address, T &data)
{
	byte *p = (byte *)(void *)&data;
	int i, n;
	for (i = 0, n = sizeof(data); i < n; i++)
		*p++ = EEPROM.read(address++);
	return i;
}


#endif

