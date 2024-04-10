/**
 * @file Turnout.h
 * @author Silmaen
 * @date 09/04/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include <IOManager.h>
#include <cstdint>
#include <string>

/**
 * @brief Class Turnout.
 */
class Turnout final {
public:
	enum struct Type {
		Ytype,
		/*
		TurnRight,
		TurnLeft,
		*/
		TriState,
	};
	enum struct State {
		Left,
		Direct,
		Right
	};
	Turnout() = default;
	/**
	 * @brief Default Constructor.
	 */
	explicit Turnout(const Type &iType, uint8_t iCmdPin1, uint8_t iRdPin1, uint8_t iCmdPin2 = NOVALUE, uint8_t iRdPin2 = NOVALUE);
	/**
	 * @brief Copy Constructor.
	 */
	Turnout(const Turnout &) = default;
	/**
	 * @brief Move Constructor.
	 */
	Turnout(Turnout &&) = default;
	/**
	 * @brief Copy assignation.
	 */
	Turnout &operator=(const Turnout &) = default;
	/**
	 * @brief Move assignation.
	 */
	Turnout &operator=(Turnout &&) = default;

	/**
	 * @brief Destructor.
	 */
	~Turnout();

	/**
	 * @brief Type getter.
	 * @return The type.
	 */
	[[nodiscard]] const Type &getType() const { return m_type; }

	/**
	 * @brief Type getter.
	 * @return The type.
	 */
	[[nodiscard]] const State &getState() const { return m_state; }

	/**
	 * @brief Define the new turnout state.
	 * @param iState The new state
	 */
	void setState(const State &iState);

	/**
	 * @brief Cycle through the possible states.
	 */
	void cycleState();

	/**
	 * @brief General update of the current state.
	 */
	void loop();

	/**
	 * @brief Handle receive message.
	 * @param iMsg The message to treat.
	 */
	void receiveMessage(const std::string &iMsg);

	/**
	 * @brief Check if the turnout is currently moving.
	 * @return True if the turnout is currently moving.
	 */
	[[nodiscard]] bool isMoving() const { return m_moving; }

private:
	void readState();
	/// Type of turnout
	Type m_type{Type::Ytype};
	/// State of the turnout
	State m_state{State::Left};
	/// is the turnout in movement.
	bool m_moving = false;
	/// First Command Pin.
	uint8_t m_cmdPin1 = NOVALUE;
	/// Second Command Pin (on for tristate)
	uint8_t m_cmdPin2 = NOVALUE;
	/// First Reading Pin.
	uint8_t m_rdPin1 = NOVALUE;
	/// Second Reading Pin (on for tristate)
	uint8_t m_rdPin2 = NOVALUE;
};
