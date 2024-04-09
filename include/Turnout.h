/**
 * @file Turnout.h
 * @author Silmaen
 * @date 09/04/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include <cstdint>
#include <string>

/**
 * @brief Class Turnout.
 */
class Turnout {
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
	explicit Turnout(const Type &iType);
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
	virtual ~Turnout();

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

	void setState(const State &iState);
	void cycleState();

	void loop(uint64_t delta);

	void receiveMessage(const std::string &msg);

	[[nodiscard]] bool isMoving() const { return m_moving; }

private:
	void readState();
	/// Type of turnout
	Type m_type{Type::Ytype};
	/// State of the turnout
	State m_state{State::Left};
	bool m_moving = false;
};
