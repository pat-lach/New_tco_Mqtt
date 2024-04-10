// Created by argawaen on 08/10/23.
//
#pragma once

#include <Arduino.h>
#include <string>

static constexpr uint8_t MCP1_0 = 0x00;
static constexpr uint8_t MCP1_1 = 0x01;
static constexpr uint8_t MCP1_2 = 0x02;
static constexpr uint8_t MCP1_3 = 0x03;
static constexpr uint8_t MCP1_4 = 0x04;
static constexpr uint8_t MCP1_5 = 0x05;
static constexpr uint8_t MCP1_6 = 0x06;
static constexpr uint8_t MCP1_7 = 0x07;
static constexpr uint8_t MCP1_8 = 0x08;
static constexpr uint8_t MCP1_9 = 0x09;
static constexpr uint8_t MCP1_10 = 0x0A;
static constexpr uint8_t MCP1_11 = 0x0B;
static constexpr uint8_t MCP1_12 = 0x0C;
static constexpr uint8_t MCP1_13 = 0x0D;
static constexpr uint8_t MCP1_14 = 0x0E;
static constexpr uint8_t MCP1_15 = 0x0F;

static constexpr uint8_t MCP2_0 = 0x10;
static constexpr uint8_t MCP2_1 = 0x11;
static constexpr uint8_t MCP2_2 = 0x12;
static constexpr uint8_t MCP2_3 = 0x13;
static constexpr uint8_t MCP2_4 = 0x14;
static constexpr uint8_t MCP2_5 = 0x15;
static constexpr uint8_t MCP2_6 = 0x16;
static constexpr uint8_t MCP2_7 = 0x17;
static constexpr uint8_t MCP2_8 = 0x18;
static constexpr uint8_t MCP2_9 = 0x19;
static constexpr uint8_t MCP2_10 = 0x1A;
static constexpr uint8_t MCP2_11 = 0x1B;
static constexpr uint8_t MCP2_12 = 0x1C;
static constexpr uint8_t MCP2_13 = 0x1D;
static constexpr uint8_t MCP2_14 = 0x1E;
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
	 * @brief Read a pin in the MCPs
	 * @param[in] iPin The mcp pin to read.
	 * @return The state of the MCP pin.
	 */
	[[nodiscard]] uint8_t readPin(uint8_t iPin) const;
	/**
	 * @brief Write value on the mcp
	 * @param iPin The mcp pin to write.
	 * @param iValue The value to write.
	 */
	void writePin(uint8_t iPin, uint8_t iValue) const;

	/**
	 * @brief Singleton access.
	 * @return Reference to the instance.
	 */
	static IOManager &get() {
		static IOManager instance;
		return instance;
	}

	enum struct State {
		NotInitiated,
		Initiated
	};

	const State &getState() const { return m_status; }

private:
	/**
	 * @brief Constructor.
	 */
	IOManager();

	State m_status = State::NotInitiated;

	uint64_t m_pulseStop = 0;
	int8_t m_timing = -1;
	std::string m_topic = "Aig/Pos";
	std::string m_payload = "0";
	/**
	 * @brief Defines the LED State.
	 * @param iId If the id of the LED.
	 * @param iOn If the LED should be on.
	 */
	void setLEDState(int8_t iId, bool iOn);
};
