// eeprom_anything.h.h

#ifndef eeprom_anything_h
#define eeprom_anything_h

#include <Arduino.h>
#include <EEPROM.h>

// �Q�γo���C++�ҪO�AŪ�g��ո��
// �Ѽ�address�OEEPROM��}�A�Ѽ�data�O�QŪ�g����Ƶ��c
template <class T> int EEPROM_writeAnything(int address, const T &data);
template <class T> int EEPROM_readAnything(int address, T &data);

// �禡�ҪO���w�q
// �ϥ�EEPROM�{���w�g�J�@�Ӥ@�Ӫ�byte
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

