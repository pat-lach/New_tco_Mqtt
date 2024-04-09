
#ifdef ARDUINO
#include <Arduino.h>
#endif

#ifndef ARDUINO
static bool looping = true;
#endif

#include "IOManager.h"
#include "WifiManager.h"
#include "mqttManager.h"

void setup() {
	Serial.begin(115200);
	Serial.println("Starting");
	WifiManager::get().setup();
	MqttManager::get().setup();
	IOManager::get().setup();
}

void loop() {
	WifiManager::get().loop();
	MqttManager::get().loop();
	IOManager::get().loop();
}

#ifndef ARDUINO
int main() {
	setup();
	while (looping)
		loop();
	return 0;
}
#endif
