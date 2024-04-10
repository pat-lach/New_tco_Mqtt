
#ifdef ARDUINO
#include <Arduino.h>
#endif

#ifndef ARDUINO
static bool looping = true;
#endif

#include "DevicesManager.h"
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

	// Get order from network
	auto &wmgr = WifiManager::get();
	auto &mqmgr = MqttManager::get();
	wmgr.loop();
	if (wmgr.getStatus() != WifiManager::Status::Connected)
		mqmgr.loop();

	// manage turnout
	for (auto &[id, turn]: DevicesManager::get().m_turnout) {
		turn.loop();
	}

	// manager remaining IO...
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
