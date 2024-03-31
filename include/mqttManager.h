// Created by damien on 08/10/23.
//
#pragma once

#include "IOManager.h"
#include <PubSubClient.h>
#include <WiFiClient.h>

class MqttManager {
public:
	/**
     * @brief Constructor.
     */
	MqttManager();
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
     * @param topic Topic of the message.
     * @param Payload Content of the message.
     */
	void senMessage(String topic, String Payload);

	/**
     * @brief Attach a new IO Manager.
     * @param mngr The new IO Manager.
     */
	void attachIOManager(IOManager *mngr = nullptr);

private:
	/// Client Wifi.
	WiFiClient wifiClient;
	/// Client MQTT.
	PubSubClient mqttClient;
	/// IO Manager.
	IOManager *ioManager = nullptr;
	/**
     * @brief Connect to the broker.
     */
	void connect();
};
