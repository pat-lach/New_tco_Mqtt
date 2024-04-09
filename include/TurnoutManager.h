/**
 * @file TurnoutManager.h
 * @author Silmaen
 * @date 09/04/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "Turnout.h"
#include <map>

struct TurnoutManager {
	static TurnoutManager &get() {
		static TurnoutManager instance;
		return instance;
	}
	std::map<uint8_t, Turnout> m_turnout;

	bool isReady() {
		for (auto &[id, item]: m_turnout) {
			if (item.isMoving())
				return false;
		}
		return true;
	}

private:
	TurnoutManager() {
		m_turnout = {
				{1, Turnout{Turnout::Type::Ytype}},
				{2, Turnout{Turnout::Type::Ytype}},
				{3, Turnout{Turnout::Type::Ytype}},
				{4, Turnout{Turnout::Type::TriState}},
				{5, Turnout{Turnout::Type::Ytype}},
				{6, Turnout{Turnout::Type::TriState}}};
	}
};
