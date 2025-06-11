#include <BlobMax72XX.h>
#include <SPI.h>

#define OP_NOOP         0
#define OP_DIGIT0       1
#define OP_DIGIT1       2
#define OP_DIGIT2       3
#define OP_DIGIT3       4
#define OP_DIGIT4       5
#define OP_DIGIT5       6
#define OP_DIGIT6       7
#define OP_DIGIT7       8
#define OP_DECODEMODE   9
#define OP_INTENSITY   10
#define OP_SCANLIMIT   11
#define OP_SHUTDOWN    12
#define OP_DISPLAYTEST 15

BlobMax72XX::BlobMax72XX(uint8_t _csPin, uint8_t _numDisplays) : Adafruit_GFX(8, _numDisplays * 8) {
	csPin = _csPin;

	numDisplays = _numDisplays;
}

void BlobMax72XX::init() {
	SPI.begin();
	pinMode(csPin, OUTPUT);

	spiTransfer(OP_SHUTDOWN, 0); // Disable powersaving
	spiTransfer(OP_DISPLAYTEST, 0); // Normal operation mode
	spiTransfer(OP_SCANLIMIT, 7); // Show all rows
}

void BlobMax72XX::spiTransfer(uint8_t opcode, uint8_t data) {
	digitalWrite(csPin, LOW);

	for(uint8_t i = 0; i < numDisplays; i++) {
		Serial.printf("0x%x 0x%x\n", opcode, data);
		SPI.transfer(opcode);
		SPI.transfer(data);
	}

	digitalWrite(csPin, HIGH);
}

void BlobMax72XX::drawPixel(int16_t x, int16_t y, uint16_t color) {
	Serial.println("Woof?");
}