// Created by damien on 08/10/23.
// modified by patrick with assistance of damien 11/10/2023
// modified by patrick MQTT via Rasberrypi p, 14/02/2024

#include "mqttManager.h"

#include "IOManager.h"

#include <DevicesManager.h>
#include <WifiManager.h>
#include <sstream>

//static const std::string mqttUser = "papa";
//static const std::string mqttPassword = "papa";
static const std::string s_mqttEmergTopicIn = "train/emerge";
static const std::string s_mqttTopicIn = "train/cmd";// si Click sur Aig, demande de changement position  et Topic Pub " Aig/cde" mess un nombre
//static const std::string mqtt_server = "pat-lach-pil";  // name dockermqtt, http://192.168.1.32:1883  image eclipse-mosquitto:2.0.18
static const IPAddress s_mqttServer = {192, 168, 1, 32};//adress  ou est le brocker modif 14/02 adress Raspi
constexpr uint16_t s_mqttServerPort = 1883;
constexpr uint8_t s_cmdBobine[] = {11, 12, 21, 22, 31, 32, 33, 34, 41, 42, 51, 52, 61, 62, 63, 64};

static std::string buildTopic(const MessageType &iMType, const DeviceType &iDType, const uint16_t iDeviceId) {
	std::string result = "train/";
	switch (iMType) {
		case MessageType::Debug:
			result += "debug";
			return result;
		case MessageType::Emergency:
			result += "emerg";
			return result;
		case MessageType::Command:
			result += "cmd";
			break;
		case MessageType::State:
			result += "state";
			break;
	}
	switch (iDType) {
		case DeviceType::General:
			break;
		case DeviceType::Aiguillage:
			result += "/aig";
			break;
		case DeviceType::PositionSensor:
			result += "/pos";
			break;
		case DeviceType::TraficLight:
			result += "/feu";
			break;
	}
	if (iDType != DeviceType::General)
		result += "/" + std::to_string(iDeviceId);
	return result;
}

static std::tuple<MessageType, DeviceType, uint16_t> decompTopic(const std::string &iTopic) {
	auto mType = MessageType::Emergency;
	auto dType = DeviceType::General;
	uint16_t id = std::numeric_limits<uint16_t>::max();
	std::vector<std::string> tokens;
	{
		std::stringstream ss(iTopic);
		std::string token;
		while (std::getline(ss, token, '/')) {
			tokens.push_back(token);
		}
	}
	if (tokens.size() < 2)
		return {mType, dType, id};
	if (tokens[0] != "train")
		return {mType, dType, id};
	if (tokens[1] == "debug")
		mType = MessageType::Debug;
	if (tokens[1] == "emerg")
		mType = MessageType::Emergency;
	if (tokens[1] == "cmd")
		mType = MessageType::Command;
	if (tokens[1] == "state")
		mType = MessageType::State;
	if (mType != MessageType::State && mType != MessageType::Command)
		return {mType, dType, id};
	if (tokens.size() < 4) {
		mType = MessageType::Emergency;
		return {mType, dType, id};
	}
	if (tokens[2] == "aig")
		dType = DeviceType::Aiguillage;
	if (tokens[2] == "pos")
		dType = DeviceType::PositionSensor;
	if (tokens[2] == "feu")
		dType = DeviceType::TraficLight;
	id = std::stoi(tokens[3]);
	return {mType, dType, id};
}

/**
 * @brief Function called everytime a message is received from broker.
 * @param iTopic The topic received
 * @param iPayload The message received.
 * @param iLength The length of the message.
 */
static void onMessageReceived(const char *iTopic, const uint8_t *iPayload, const uint32_t iLength) {
	const std::string topic(iTopic);
	const std::string payload(iPayload, iPayload + iLength);
	auto [mType, dType, id] = decompTopic(topic);
	if (mType == MessageType::Emergency) {
		// todo: treat emergency!!!
	}
	if (mType == MessageType::Command) {
		// treat command !!
		auto &devices = DevicesManager::get();
		if (dType == DeviceType::Aiguillage) {
			auto &turnout = devices.m_turnout[id];// todo: safe access to turnout by id...
			turnout.receiveMessage(payload);
		}
		// todo: treat other devices
	}
}


MqttManager::MqttManager() : m_mqttClient(m_wifiClient) {}

void MqttManager::setup() {
	m_mqttClient.setServer(s_mqttServer, s_mqttServerPort);
	m_mqttClient.setKeepAlive(5);
	m_mqttClient.setCallback(onMessageReceived);
}

void MqttManager::loop() {
	if (WifiManager::get().getStatus() != WifiManager::Status::Connected)
		return;// nothing to do if no wifi.
	if (!m_mqttClient.connected()) {
		connect();
	}
	if (m_status == Status::Connected) {
		if (!m_mqttClient.loop()) {
			m_status = Status::NotConnected;
		}
	}
}

void MqttManager::connect() {
	if (!m_mqttClient.connected()) {
		if (m_coolDown > millis())
			return;
		Serial.print("Attempting MQTT connection...");
		if (m_mqttClient.connect("")) {
			Serial.println("connected");
			m_mqttClient.subscribe(s_mqttTopicIn.c_str());
			m_mqttClient.subscribe(s_mqttEmergTopicIn.c_str());
			m_status = Status::Connected;
		} else {
			Serial.print("failed, rc=");
			Serial.print(m_mqttClient.state());
			Serial.println(" will try again in 5 seconds");
			Serial.print("remote ip");
			Serial.print(m_wifiClient.remoteIP().toString());
			Serial.print(":");
			Serial.println(m_wifiClient.remotePort());
			m_status = Status::NotConnected;
			m_coolDown = millis() + 2000u;
		}
	}
}

void MqttManager::sendMessage(const std::string &iTopic, const std::string &iPayload) {
	m_mqttClient.publish(iTopic.c_str(), iPayload.c_str());
}
void MqttManager::send(const std::string &iMessage, const MessageType &iMType, const DeviceType &iDtype, const uint16_t iId) {
	sendMessage(buildTopic(iMType, iDtype, iId), iMessage);
}
