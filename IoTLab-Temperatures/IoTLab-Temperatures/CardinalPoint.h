#pragma once

enum CardinalPointFlag
{
	EAST = 1 << 0,
	NORTH = 1 << 1,
	SOUTH = 1 << 2,
	WEST = 1 << 3,
};

// Allows for a CardinalPointFlag to hold multiple flags of the same type
// Example:
//     CardinalPointFlag northEast = CardinalPointFlag.NORTH | CardinalPointFlag.EAST;
inline CardinalPointFlag operator|(CardinalPointFlag a, CardinalPointFlag b)
{
	return static_cast<CardinalPointFlag>(static_cast<int>(a) | static_cast<int>(b));
}
