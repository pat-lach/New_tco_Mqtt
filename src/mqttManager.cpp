// Created by damien on 08/10/23.
// modified by patrick with assistance of damien 11/10/2023
// modified by patrick MQTT via Rasberrypi p, 14/02/2024

#include "mqttManager.h"

//static const String mqttUser = "papa";
//static const String mqttPassword = "papa";
static const String mqttTopicIn = "Aig/Cde";     // si Click sur Aig, demande de changement position  et Topic Pub " Aig/cde" mess un nombre
//static const String mqtt_server = "pat-lach-pil";  // name dockermqtt, http://192.168.1.32:1883  image eclipse-mosquitto:2.0.18
static const IPAddress mqtt_server = {192, 168, 1, 32};//adress  ou est le brocker modif 14/02 adress Raspi
constexpr uint16_t mqtt_server_port = 1883;

static IOManager *s_ioManager = nullptr;

/**
 * @brief Function called everytime a message is received from broker.
 * @param topic The topic received
 * @param payload The message received.
 * @param length The length of the message.
 */
static void callback(char *topic, byte *payload, unsigned int length) {
	String Topic(topic);
	String Payload;
	String topic_sub;
	String Payload_sub;

	Payload.reserve(length);
	for (unsigned int i = 0; i < length; ++i) {
		Payload.concat((char) payload[i]);
	}
	Serial.print("  Received message: ");
	Serial.print(Topic);
	Serial.print(" // '");
	Serial.print(Payload);
	Serial.println("'");
	if (Topic == "Aig/Cde")  {     //if ((Topic == "Aig/Cde") || (Topic == "TopicESP/bp1")) {		
		int id = Payload.toInt();
		Serial.print("  int id ");
		Serial.println(id);		
			const byte CdeBobine[] = {11,12,21,22,31,32,33,34,41,42,51,52,61,62,63,64};
			for (int i=0; i<16;i++)
			{ if (CdeBobine[i] == id) 	{ 
				id = i;
				Serial.print("  new id for Output ");
				Serial.println(id);	
				}
			}
		if ( id >= 0 && id < 16){
		//if (Payload.equalsIgnoreCase("on")) 
		
		if (s_ioManager) {
			s_ioManager->setLEDState(id, true, topic_sub, Payload_sub);
		}
		 } else if (Payload.equalsIgnoreCase("off")) {
		 	if (s_ioManager) {
		 		s_ioManager->setLEDState(id, false, topic_sub, Payload_sub);
				
		 	}
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
	if (!mqttClient.connected()) {
		connect();
	}
	if (!mqttClient.loop()) {
	}
}

void MqttManager::connect() {

	while (!mqttClient.connected()) {
		Serial.print("Attempting MQTT connection...");
		String mqttClientId = "";
		if (mqttClient.connect(mqttClientId.c_str())) {
			Serial.println("connected");
			mqttClient.subscribe(mqttTopicIn.c_str());
			Serial.print("subscribed to ");
			Serial.println(mqttTopicIn);
			senMessage("TopicESP/Welcome", "hello");
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

void MqttManager::senMessage(String topic, String Payload) {
	Serial.print("  Sending Message: ");
	Serial.print(topic);
	Serial.print(" // ");
	Serial.println(Payload);
	mqttClient.publish(topic.c_str(), Payload.c_str());
}

void MqttManager::attachIOManager(IOManager *mngr) {
	ioManager = mngr;
	s_ioManager = mngr;
}
