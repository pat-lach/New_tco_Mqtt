
#ifdef ARDUINO
#include <Arduino.h>
#endif

#ifndef ARDUINO
static bool looping = true;
#endif

#include "IOManager.h"
#include "TurnoutManager.h"
#include "WifiManager.h"
#include "mqttManager.h"

void setup() {
	Serial.begin(115200);
	Serial.println("Starting");
	WifiManager::get().setup();
	MqttManager::get().setup();
	IOManager::get().setup();
}

uint64_t last = 0;
void loop() {
	// Chronometre
	uint64_t now = micros64();
	const uint64_t delta = now - last;
	std::swap(last, now);

	// Get order from network
	WifiManager::get().loop();
	MqttManager::get().loop();

	// manage turnout
	for (auto &[id, turn]: TurnoutManager::get().m_turnout) {
		turn.loop(delta);
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
