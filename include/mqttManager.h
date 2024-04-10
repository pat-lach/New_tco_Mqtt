// Created by damien on 08/10/23.
//
#pragma once

#include "Definitions.h"
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <string>


/**
 * @brief Class managing the MQTT connexion and messages.
 */
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
	 * @brief Send a message to the broker, topic auto contruct.
	 * @param iMessage The message to send.
	 * @param iMType Type of message.
	 * @param iDtype Type of device.
	 * @param iId Device id.
	 */
	void send(const std::string &iMessage, const MessageType &iMType, const DeviceType &iDtype = DeviceType::General,
			  uint16_t iId = std::numeric_limits<uint16_t>::max());

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

	/**
	 * @brief Connection status.
	 */
	enum struct Status {
		NotConnected,///< Device not connected.
		Connected    ///< Device is connected.
	};

private:
	/// Client Wifi.
	WiFiClient m_wifiClient;
	/// Client MQTT.
	PubSubClient m_mqttClient;
	/// The Current connexion statuus.
	Status m_status{Status::NotConnected};
	/// Cooldown before connection retry.
	uint32_t m_coolDown = 0;
	/**
	 * @brief Constructor.
	 */
	MqttManager();
	/**
	 * @brief Connect to the broker.
	 */
	void connect();
};
