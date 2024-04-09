// Created by argawaen on 08/10/23.
//
#pragma once

#include <Arduino.h>

class MqttManager;
/**
 * @brief class managing the IO.
 */


class IOManager {
public:
	/**
	 * @brief Constructor.
	 */
	IOManager();
	/**
	 * @brief Initialization function.
	 */
	void setup();
	/**
	 * @brief Frame function.
	 */
	void loop();
	/**
	 * @brief Attach a messaging system
	 * @param mngr The new messaging system
	 */
	void attachMqttManager(MqttManager * mngr = nullptr);
	// void senMessage(String topic, String Payload);

	/**
	 * @brief Defines the LED State.
	 * @param on If the LED should be on.
	 */
	void setLEDState(int8_t Id, bool on, String topic_sub, String Payload_sub);
	

private:
	/// The MqttManager
	MqttManager *mqttManager = nullptr;
	/// The pin to read
	//uint8_t PinRead = D6;
	/// The LED pin
	
	
	///  The last state of the switch
	bool switchState = false;
	///  pour mise au point
	int iNb = 0;
	int iEtatMemPB = HIGH;
	uint64_t pulseStop = 0;

	int8_t timing = -1;
	struct EtatP {
		bool memoire = false;
		bool release = true;
	};
	EtatP MesEtats[16];
	String topic_sub ="Aig/Pos";
	String Payload = "0";
	int Contact[16] = { 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
	const byte PosBobine[16] = {11, 12, 21, 22, 31, 32, 33, 34, 41, 42, 51, 52, 61, 62, 63, 64};	
	String InitAigPos = "11";

};
