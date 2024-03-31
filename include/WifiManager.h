//
// Created by damien on 08/10/23.
//

#pragma once

#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <PubSubClient.h>
#include <WiFiUdp.h>

/**
 * @brief Class to manage the network.
 */
class WifiManager {
public:
	/**
     * @brief Constructor
     */
	WifiManager();
	/**
     * @brief Initialization function.
     */
	void setup();
	/**
     * @brief Frame function;
     */
	void loop();

private:
	/// UDP connection.
	WiFiUDP ntpUDP;
	/// Network time.
	NTPClient timeClient;
	/**
	 * @brief check and eventually reconnect the wifi.
	 */
	void checkWifiConnexion();
};
