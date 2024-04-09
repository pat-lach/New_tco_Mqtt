// Created by damien on 08/10/23.
//
#pragma once
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <string>

class MqttManager {
public:
	/**
	 * @brief Initialization function.
	 */
	void setup();
	/**
	 * @brief Frame function.
	 */
	void loop();

	/**
	 * @brief Send a message to the broker.
	 * @param iTopic Topic of the message.
	 * @param iPayload Content of the message.
	 */
	void sendMessage(const std::string &iTopic, const std::string &iPayload);

	/**
	 * @brief Singleton access.
	 * @return Reference to the instance.
	 */
	static MqttManager &get() {
		static MqttManager instance;
		return instance;
	}

private:
	/// Client Wifi.
	WiFiClient wifiClient;
	/// Client MQTT.
	PubSubClient mqttClient;
	/**
	 * @brief Constructor.
	 */
	MqttManager();
	/**
	 * @brief Connect to the broker.
	 */
	void connect();
};
