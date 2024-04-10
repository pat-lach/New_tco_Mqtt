/**
 * @file Turnout.cpp
 * @author Silmaen
 * @date 09/04/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Turnout.h"

#include <DevicesManager.h>
#include <IOManager.h>
#include <mqttManager.h>


Turnout::Turnout(const Type &iType,
				 const uint8_t iCmdPin1, const uint8_t iRdPin1,
				 const uint8_t iCmdPin2, const uint8_t iRdPin2) : m_type{iType},
																  m_cmdPin1{iCmdPin1},
																  m_cmdPin2{iCmdPin2},
																  m_rdPin1{iRdPin1},
																  m_rdPin2{iRdPin2} {}

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
	const auto &iomgr = IOManager::get();
	switch (m_type) {
		case Type::Ytype:
			if (iomgr.readPin(m_rdPin1) == 0)
				m_state = State::Left;
			else
				m_state = State::Right;
			break;
		case Type::TriState:
			const std::pair pins = {iomgr.readPin(m_rdPin1), iomgr.readPin(m_rdPin2)};
			if (pins.first == 1 && pins.second == 1)
				m_state = State::Right;
			else if (pins.first == 0 && pins.second == 0)
				m_state = State::Left;
			else if (pins.first == 0 && pins.second == 1)// to be adapted
				m_state = State::Direct;
			break;
	}
}

void Turnout::loop() {
	// todo
	// lecture de l'entrée (appel à IOManager)
	//auto& iomng = IOManager::get();
	//auto st = iomng.readInputState(m_id);

	// si lecture == consigne
	// est-ce qu'une commande est en attente? oui -> termine la commande ===> message MQTT
	// si lecture != consigne
	// est-ce qu'une commande est en attente? non -> lance la commande
}

void Turnout::receiveMessage(const std::string &msg) {
	auto &tmgr = DevicesManager::get();
	auto &mqmgr = MqttManager::get();
	if (msg == "change") {
		if (!tmgr.isReady()) {
			mqmgr.sendMessage("EMERGENCY", "chuipaprès");
		} else {
			mqmgr.sendMessage("COUCOU", "je bosse dessus!");
		}
	}
}
