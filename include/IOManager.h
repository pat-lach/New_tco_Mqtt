// Created by argawaen on 08/10/23.
//
#pragma once

#include <Arduino.h>
#include <string>

static constexpr uint8_t MCP1_0 = 0x00;
// ...
static constexpr uint8_t MCP1_15 = 0x0F;

static constexpr uint8_t MCP2_0 = 0x10;
// ...
static constexpr uint8_t MCP2_15 = 0x1F;

static constexpr uint8_t NOVALUE = 0xFF;
/**
 * @brief class managing the IO.
 */
class IOManager {
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
	 * @brief Defines the LED State.
	 * @param iId If the id of the LED.
	 * @param iOn If the LED should be on.
	 */
	void setLEDState(int8_t iId, bool iOn);

	void cycleTurnout(int8_t iId);

	[[nodiscard]] uint8_t readPin() const;
	void writePin(uint8_t) const;

	/**
	 * @brief Singleton access.
	 * @return Reference to the instance.
	 */
	static IOManager &get() {
		static IOManager instance;
		return instance;
	}

private:
	/**
	 * @brief Constructor.
	 */
	IOManager();
	///  The last state of the switch
	bool m_switchState = false;
	///  pour mise au point
	int m_Nb = 0;
	int m_EtatMemPB = HIGH;
	uint64_t m_pulseStop = 0;

	int8_t m_timing = -1;

	struct EtatP {
		bool memoire = false;
		bool release = true;
	};

	EtatP m_MesEtats[16];
	std::string m_topic = "Aig/Pos";
	std::string m_payload = "0";
};
