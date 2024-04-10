// Created by damien on 08/10/23.
// modified by patrick 11/10/2023
#include "WifiManager.h"
#include <ESP8266WiFi.h>
#include <string>

// TODO: store credential in ESP storage.
static const std::string s_ssid = "Bbox-1988C86B";// connection box patrick
static const std::string s_password = "la-maison-Anadele-69&Bella";

WifiManager::WifiManager() : m_timeClient(m_ntpUdp) {}

void WifiManager::setup() {
	checkWifiConnexion();
	m_timeClient.begin();
}

void WifiManager::loop() {
	if (m_status == Status::NotConnected && m_countdown > millis())
		return;
	checkWifiConnexion();
	m_timeClient.update();
}

void WifiManager::checkWifiConnexion() {
	if (WiFi.status() != WL_CONNECTED) {
		if (m_status == Status::Connecting) {
			if (m_countdown < millis()) {
				Serial.println("Wifi Connexion Timed out after 1.5s! Retry in 5s...");
				m_status = Status::NotConnected;
				m_countdown = millis() + 5000u;
			}
		} else {
			m_status = Status::Connecting;
			Serial.println("Wifi reconnecting .");
			m_countdown = millis() + 15000u;
			WiFi.mode(WIFI_STA);
			WiFi.begin(s_ssid.c_str(), s_password.c_str());
		}
	} else {
		if (m_status != Status::Connected) {
			m_status = Status::Connected;
			Serial.println("Wifi connected.");
			Serial.print("Address: ");
			Serial.println(WiFi.localIP().toString());
			Serial.print("Strength: ");
			Serial.println(WiFi.RSSI());
		}
	}
}
