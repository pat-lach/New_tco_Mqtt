/**
 * @file Definitions.h
 * @author Silmaen
 * @date 10/04/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

enum struct MessageType {
	Command,
	State,
	Debug,
	Emergency
};

enum struct DeviceType {
	General,
	Aiguillage,
	PositionSensor,
	TraficLight
};
