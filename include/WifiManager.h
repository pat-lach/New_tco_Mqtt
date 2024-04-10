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

	/**
	 * @brief Connection status
	 */
	enum struct Status {
		NotConnected,///< Device not connected.
		Connecting,  ///< Device is pending connexion.
		Connected    ///< Device is connected.
	};

	/**
	 * @brief Getter on the status.
	 * @return The current status.
	 */
	[[nodiscard]] const Status &getStatus() const { return m_status; }

private:
	/**
	 * @brief Constructor
	 */
	WifiManager();
	/// UDP connection.
	WiFiUDP m_ntpUdp;
	/// Network time.
	NTPClient m_timeClient;
	/// Wifi device status.
	Status m_status{Status::NotConnected};
	/// Expected time to connect.
	uint32_t m_countdown = 0;
	/**
	 * @brief check and eventually reconnect the wifi.
	 */
	void checkWifiConnexion();
};
