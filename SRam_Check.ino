#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
	char top;
#ifdef __arm__
	return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
	return &top - __brkval;
#else  // __arm__
	return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

void checkSRam() {
	int sRam = freeMemory();
	if (sRam < 3000) {
		lcd.setCursor(0, 1);
		lcd.print("SRam-> ");
		lcd.print(String(sRam));
		RamER.resume();
	}
	SerialDebug("SRam=", String(sRam));
}

void SRamRestart() {
	int sRam = freeMemory();
	if(sRam < 2500)	resetFunc();
}