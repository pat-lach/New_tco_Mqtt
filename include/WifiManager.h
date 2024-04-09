//
// Created by damien on 08/10/23.
//

#pragma once

#include <NTPClient.h>
#include <WiFiUdp.h>

/**
 * @brief Class to manage the network.
 */
class WifiManager {
public:
	/**
	 * @brief Initialization function.
	 */
	void setup();
	/**
	 * @brief Frame function;
	 */
	void loop();

	/**
	 * @brief Singleton access.
	 * @return Reference to the instance.
	 */
	static WifiManager &get() {
		static WifiManager instance;
		return instance;
	}

private:
	/**
	 * @brief Constructor
	 */
	WifiManager();
	/// UDP connection.
	WiFiUDP ntpUDP;
	/// Network time.
	NTPClient timeClient;
	/**
	 * @brief check and eventually reconnect the wifi.
	 */
	static void checkWifiConnexion();
};
