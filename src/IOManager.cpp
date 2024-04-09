
// Created by damien on 08/10/23.
// modified by Patrick 12/10/2023, tout ok avec buton ou message MQTT
// modified by Patrick 14/02/2024 modif wiring of MCP23017, One for Aig et ond for return position
// modified by Patrick 30/03/2024 reading state of Aig position ton confirm right way

#include "IOManager.h"
#include "mqttManager.h"
#include <Adafruit_MCP23X17.h>// add for I2C & MCP23017
#include <Wire.h>             // add for I2C GPIO 5 (D1) (SCL), GPIO 4 (D2) (SDA)

constexpr uint64_t pulseTime = 500;// 500

Adafruit_MCP23X17 mcp1;// Adafruit_MCP23X17 mcp1;
Adafruit_MCP23X17 mcp2;// Adafruit_MCP23X17 mcp2;

IOManager::IOManager() = default;

constexpr char PosBobine[] = {11, 12, 21, 22, 31, 32, 33, 34, 41, 42, 51, 52, 61, 62, 63, 64};

void IOManager::setup() {
	m_switchState = false;// all Switch off

	Wire.begin(4, 5);// SDA d2 GPIO4, SCL d1 GPIO5, esp8266...Start I2C communication

	/****************************************/
	//          I2C Scanner
	/********************************************************************/
	int nDevices = 0;
	Serial.println("Scanning...");
	for (byte address = 1; address < 127; address++) {
		// The i2c_scanner uses the return value of
		// the Write.endTransmisstion to see if
		// a device did acknowledge to the address.
		Wire.beginTransmission(address);
		if (const uint8_t error = Wire.endTransmission(); error == 0) {
			Serial.print("I2C device found at address 0x");
			if (address < 16) Serial.print("0");
			Serial.print(address, HEX);
			Serial.println("  !");
			nDevices++;
		} else if (error == 4) {
			Serial.print("Unknown error at address 0x");
			if (address < 16) Serial.print("0");
			Serial.println(address, HEX);
		}
	}
	if (nDevices == 0) Serial.println("No I2C devices found\n");
	else
		Serial.println("done\n");
	delay(200);// wait 0.2 seconds for next scan

	/********************************************************************/
	//          Init mcp2 input
	/********************************************************************/

	mcp2.begin_I2C(0x21);// Instantiate mcp2: objectaddress 1
	if (!mcp2.begin_I2C(0x21)) {
		Serial.println(" Error.mcp2 ");// if (!mcp2.begin_SPI(CS_PIN)) { --> if use a spi version
		for (;;)
			;
	}
	Serial.print(" mcp2 Ok");
	for (int i = 0; i < 16; i++) {
		mcp2.pinMode(i, INPUT_PULLUP);// init HIGH
		delay(100);
	}
	byte input2[16];
	Serial.print("  lecture position on mcp2 (setup):  ");
	for (int i = 0; i < 16; i++) {
		input2[i] = mcp2.digitalRead(i);
		if (i == 15) {
			Serial.print(input2[i]);
		} else {
			Serial.print(input2[i]);
			if (i % 2 == 1) { Serial.print(" "); }
			if (i == 3 || i == 7 || i == 11) {
				Serial.print("  ");
			}
		}
		delay(5);
	}

	/********************************************************************/
	//          Init mcp1 output
	/********************************************************************/

	mcp1.begin_I2C(0x20);// Instantiate mcp1: objectaddress 0
	if (!mcp1.begin_I2C()) {
		Serial.println(" Error.mcp1 ");// if (!mcp1.begin_SPI(CS_PIN)) { --> if use a spi version
		for (;;)
			;
	}
	Serial.print(" mcp1 Ok");
	Serial.println("  setting mcp1 output High (setup) ");
	for (int n = 0; n < 16; n++)// configure Port A: pins 0..7/ Port B: pins 8..15
	{
		mcp1.pinMode(n, OUTPUT);
		delay(50);
		mcp1.digitalWrite(n, HIGH);// all setting off
	}
	delay(100);
	Serial.println();
}

void IOManager::loop() {
	if (m_timing >= 0) {
		if ((millis() - m_pulseStop) >= pulseTime) {
			setLEDState(m_timing, false);
		}
	}
}

void IOManager::setLEDState(const int8_t iId, const bool iOn) {
	if (iOn) {
		m_timing = iId;
		m_pulseStop = millis();
		mcp1.digitalWrite(iId, LOW);// Relais on     LOW
		Serial.print("   -->  mcp1 on  for Id: ");
		Serial.println(iId);
	} else {
		m_timing = -1;
		mcp1.digitalWrite(iId, HIGH);// Relais off    HIGH
		Serial.print("   -->  mcp1 off for Id: ");
		Serial.println(iId);
		delay(5);

		byte input2[16];
		Serial.print("  lecture position on mcp2:  ");
		for (int i = 0; i < 16; i++) {
			input2[i] = mcp2.digitalRead(i);
			if (i == 15) {
				Serial.print(input2[i]);
			} else {
				Serial.print(input2[i]);
				if (i % 2 == 1) { Serial.print(" "); }
				if (i == 3 || i == 7 || i == 11) { Serial.print("  "); }
			}
			delay(5);
		}
		Serial.print("  -->  Check position Aig for id ");
		Serial.println(iId);
		std::string payload;
		payload = PosBobine[iId];
		Serial.print("Payload_sub: ");
		Serial.print(payload.c_str());

		if (input2[iId] == 1) {
			Serial.print(" topic: ");
			Serial.print(m_topic.c_str());
			Serial.print(" mess_sub: ");
			Serial.println(payload.c_str());
			MqttManager::get().senMessage(m_topic, payload);
		} else {
			Serial.print(" wrong etat Aig: ");
			Serial.println(input2[iId]);
		}
	}
}