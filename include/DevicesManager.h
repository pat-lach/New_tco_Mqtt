/**
 * @file DevicesManager.h
 * @author Silmaen
 * @date 09/04/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "Turnout.h"
#include <map>

struct DevicesManager {
	static DevicesManager &get() {
		static DevicesManager instance;
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

	void loop() {
		for (auto &[id, turn]: m_turnout) {
			turn.loop();
		}
	}

private:
	DevicesManager() {
		m_turnout = {
				{1, Turnout{Turnout::Type::Ytype, MCP1_0, MCP2_0}},
				{2, Turnout{Turnout::Type::Ytype, MCP1_1, MCP2_1}},
				{3, Turnout{Turnout::Type::Ytype, MCP1_2, MCP2_2}},
				{4, Turnout{Turnout::Type::TriState, MCP1_3, MCP2_3, MCP1_4, MCP2_4}},
				{5, Turnout{Turnout::Type::Ytype, MCP1_5, MCP2_5}},
				{6, Turnout{Turnout::Type::TriState, MCP1_6, MCP2_6, MCP1_7, MCP2_6}}};
	}
};
