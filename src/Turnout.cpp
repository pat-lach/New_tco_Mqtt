/**
 * @file Turnout.cpp
 * @author Silmaen
 * @date 09/04/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Turnout.h"

#include <IOManager.h>
#include <TurnoutManager.h>
#include <mqttManager.h>


Turnout::Turnout(const Type &iType) : m_type{iType} {}

Turnout::~Turnout() = default;

void Turnout::setState(const State &iState) {
	if (m_type == Type::Ytype && iState == State::Direct) {
		// Print error message!
		return;
	}
	m_state = iState;
}

void Turnout::cycleState() {
	switch (m_type) {
		case Type::Ytype:
			if (m_state == State::Right)
				m_state = State::Left;
			else
				m_state = State::Right;
			break;
		case Type::TriState:
			switch (m_state) {
				case State::Left:
					m_state = State::Direct;
					break;
				case State::Direct:
					m_state = State::Right;
					break;
				case State::Right:
					m_state = State::Left;
					break;
			}
			break;
	}
}

void Turnout::readState() {
	switch (m_type) {
		case Type::Ytype:
			// read pin (call IOManager)
			m_state = State::Direct;
			break;
		case Type::TriState:
			// read pin1
			// read pin2
			m_state = State::Left;
			break;
	}
}

void Turnout::loop(uint64_t delta) {

	/// lecture de l'entrée (appel à IOManager)
	//auto& iomng = IOManager::get();
	//auto st = iomng.readInputState(m_id);

	/// si lecture == consigne
	/// est-ce qu'une commande est en attente? oui -> termine la commande ===> message MQTT
	/// si lecture != consigne
	/// est-ce qu'une commande est en attente? non -> lance la commande
}

void Turnout::receiveMessage(const std::string &msg) {
	auto &tmgr = TurnoutManager::get();
	auto &mqmgr = MqttManager::get();
	if (msg == "change") {
		if (!tmgr.isReady()) {
			mqmgr.sendMessage("EMERGENCY", "chuipaprès");
		} else {
			mqmgr.sendMessage("COUCOU", "je bosse dessus!");
		}
	}
}
