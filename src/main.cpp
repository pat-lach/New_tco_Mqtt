
#ifdef ARDUINO
#include <Arduino.h>
#endif

#ifndef ARDUINO
static bool looping = true;
#endif

#include "WifiManager.h"
#include "mqttManager.h"

/// Manage the network.
WifiManager netMngr;
MqttManager mqttMngr;
IOManager ioMngr;
//unsigned long Timeloop = 0;

void setup() {
	Serial.begin(115200);
	Serial.println("Starting");
	netMngr.setup();
	mqttMngr.setup();
	ioMngr.setup();
	mqttMngr.attachIOManager(&ioMngr);
	ioMngr.attachMqttManager(&mqttMngr);
}

void loop() {
	netMngr.loop();
	mqttMngr.loop();
	ioMngr.loop();
}

#ifndef ARDUINO
int main() {
	setup();
	while (looping)
		loop();
	return 0;
}
#endif
