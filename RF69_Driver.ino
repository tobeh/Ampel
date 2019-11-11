#ifndef spi
#include <SPI.h>
#endif

// Pins

// Pull low to completely cut power to the radio
#define PinRegulatorEnable 5

// Pull LOW to turn on the radio
#define PinReset 9


#define PinCS 10

// RF69 SPI Registers

/*
 FIFO
*/
#define RegFIFO 0x00

/*
	7) SequencerOff (Datasheet 4.2)
	6) ListenOn
	5) ListenAbort
	4-2) Mode
		000 -> Sleep
		001 -> STDBY
		010 -> Frequ. Synth. Mode
		011 -> Transmitter Mode
		100 -> Receiver Mode
*/
#define RegOpMode			0x01

/*
	6-5) DataMode
		00 -> Packet Mode
		10 -> Continous with bit synchronizer
		11 -> Continous without ...
*/
#define RedDataModul 0x02

#define RegNodeAdrs			0x39
#define RegBroadcastAdrs	0x3A
#define RegAutoModes		0x3B

/*
	
*/
#define RegFifoThresh		0x3C

/*
	3) TempMeasStart (set to 1 if you want to trigger a measurement)
	2) TempMeasRunning (1 while meas. is running)
*/
#define RegTemp1			0x4E

/*
	Measured Temperature (1°C per LSB)
*/
#define RegTemp2			0x4F

void init(){
	initBoard();
	initSpi();
}

void initBoard(){
	pinMode(PinRegulatorEnable, OUTPUT);
	pinMode(PinReset, OUTPUT);
	
	digitalWrite(PinRegulatorEnable, HIGH);
	digitalWrite(PinReset, LOW);
}

void initSpi(){
	SPI.begin();
	SPI.beginTransaction(SPISettings(9600, MSBFIRST, SPI_MODE0));
	pinMode(PinCS, OUTPUT);
	delay(50);
}

byte readTemp(){
	writeRegister(RegTemp1, (1 << 3));
	delay(1);
	byte temp = readRegister(RegTemp2);
	return temp;
}

void writeRegister(byte reg, byte val){
	digitalWrite(PinCS, LOW);
	delay(50);
	byte addrByte = (1 << 7) | reg;
	SPI.transfer(addrByte);
	SPI.transfer(val);
	digitalWrite(PinCS, HIGH);
	return;
}

byte readRegister(byte reg){
	digitalWrite(PinCS, LOW);
	delay(50);
	SPI.transfer(reg);
	delay(1);
	byte answer = SPI.transfer(0);
	digitalWrite(PinCS, HIGH);
	return answer;
}

void setNodeAddress(byte addr){
	writeRegister(RegNodeAdrs, addr);
}

byte getNodeAddress(){
	return readRegister(RegNodeAdrs);
}

void setBroadcastAddress(byte addr){
	writeRegister(RegBroadcastAdrs, addr);
}

byte getBroadcastAddress(){
	return readRegister(RegBroadcastAdrs);
}

void sendData(byte* data, byte len){
	digitalWrite(PinCS, LOW);
	SPI.transfer(RegFIFO | (1 << 7));
	for (int i = 0; i<len; i++)
		SPI.transfer(data[i]);
	digitalWrite(PinCS, HIGH);
	writeRegister(RegOpMode, (011) << 2);
}