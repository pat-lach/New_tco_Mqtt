// Created by damien on 08/10/23.
// modified by patrick with assistance of damien 11/10/2023
// modified by patrick MQTT via Rasberrypi p, 14/02/2024

#include "mqttManager.h"

#include "IOManager.h"

#include <TurnoutManager.h>

//static const std::string mqttUser = "papa";
//static const std::string mqttPassword = "papa";
static const std::string mqttTopicIn = "Aig/Cde";// si Click sur Aig, demande de changement position  et Topic Pub " Aig/cde" mess un nombre
//static const std::string mqtt_server = "pat-lach-pil";  // name dockermqtt, http://192.168.1.32:1883  image eclipse-mosquitto:2.0.18
static const IPAddress mqtt_server = {192, 168, 1, 32};//adress  ou est le brocker modif 14/02 adress Raspi
constexpr uint16_t mqtt_server_port = 1883;
constexpr uint8_t CdeBobine[] = {11, 12, 21, 22, 31, 32, 33, 34, 41, 42, 51, 52, 61, 62, 63, 64};

/**
 * @brief Function called everytime a message is received from broker.
 * @param iTopic The topic received
 * @param iPayload The message received.
 * @param iLength The length of the message.
 */
static void callback(const char *iTopic, const byte *iPayload, const uint32_t iLength) {
	const String Topic(iTopic);
	String Payload;
	String topic_sub;
	String Payload_sub;

	Payload.reserve(iLength);
	for (unsigned int i = 0; i < iLength; ++i) { Payload.concat(static_cast<char>(iPayload[i])); }
	Serial.print("  Received message: ");
	Serial.print(Topic);
	Serial.print(" // '");
	Serial.print(Payload);
	Serial.println("'");
	if (Topic == "Aig/Cmd/1") {
		TurnoutManager::get().m_turnout[1].receiveMessage(Payload.c_str());
		/// on attent un mmessage de la forme "<aig_id>;<state>;<pending>"
		/*
		 * oulà, j'ai pas bien compris ton décodage de payload je crains un bug!!
		 *
		 */
		//if ((Topic == "Aig/Cde") || (Topic == "TopicESP/bp1")) {
		int id = Payload.toInt();
		Serial.print("  int id ");
		Serial.println(id);
		const byte CdeBobine[] = {11, 12, 21, 22, 31, 32, 33, 34, 41, 42, 51, 52, 61, 62, 63, 64};
		for (int i = 0; i < 16; i++) {
			if (CdeBobine[i] == id) {
				id = i;
				Serial.print("  new id for Output ");
				Serial.println(id);
			}
		}
		if (id >= 0 && id < 16) {
			//if (Payload.equalsIgnoreCase("on"))
			IOManager::get().setLEDState(static_cast<int8_t>(id), true);
		} else if (Payload.equalsIgnoreCase("off")) {
			// pas compris... que vaut 'id' si le payload vaut 'off' ???
			IOManager::get().setLEDState(static_cast<int8_t>(id), false);
		} else {
			Serial.print(" Invalid Payload: ");
			Serial.println(Payload);
		}
	}
}

MqttManager::MqttManager() : mqttClient(wifiClient) {}

void MqttManager::setup() {
	mqttClient.setServer(mqtt_server, mqtt_server_port);
	mqttClient.setKeepAlive(5);
	mqttClient.setCallback(callback);
}

void MqttManager::loop() {
	if (!mqttClient.connected()) { connect(); }
	if (!mqttClient.loop()) {}
}

void MqttManager::connect() {
	while (!mqttClient.connected()) {
		Serial.print("Attempting MQTT connection...");
		std::string mqttClientId;
		if (mqttClient.connect(mqttClientId.c_str())) {
			Serial.println("connected");
			mqttClient.subscribe(mqttTopicIn.c_str());
			Serial.print("subscribed to ");
			Serial.println(mqttTopicIn.c_str());
			sendMessage("TopicESP/Welcome", "hello");
		} else {
			Serial.print("failed, rc=");
			Serial.print(mqttClient.state());
			Serial.println(" will try again in 5 seconds");
			Serial.print("remote ip");
			Serial.print(wifiClient.remoteIP().toString());
			Serial.print(":");
			Serial.println(wifiClient.remotePort());
			delay(1000);
		}
	}
}

void MqttManager::sendMessage(const std::string &iTopic, const std::string &iPayload) {
	Serial.print("  Sending Message: ");
	Serial.print(iTopic.c_str());
	Serial.print(" // ");
	Serial.println(iPayload.c_str());
	mqttClient.publish(iTopic.c_str(), iPayload.c_str());
}
