#pragma once

enum CardinalPointFlags
{
	EAST = 1 << 0,
	NORTH = 1 << 1,
	SOUTH = 1 << 2,
	WEST = 1 << 3
};

// Allows for a CardinalPointFlag to hold multiple flags of the same type
// Example:
//     CardinalPointFlag northEast = CardinalPointFlag.NORTH | CardinalPointFlag.EAST;
inline CardinalPointFlags operator|(CardinalPointFlags a, CardinalPointFlags b)
{
	return static_cast<CardinalPointFlags>(static_cast<int>(a) | static_cast<int>(b));
}

// Allows for a CardinalPointFlag to hold multiple flags of the same type
// Example:
//     CardinalPointFlag northEast = CardinalPointFlag.NORTH;
//     northEast |= CardinalPointFlag.EAST;
inline CardinalPointFlags operator|=(CardinalPointFlags originalFlag, CardinalPointFlags flagToAdd)
{
	return originalFlag = originalFlag | flagToAdd;
}
