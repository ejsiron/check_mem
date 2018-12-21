#pragma once

namespace map_positivepowerprefix
{
	// returns number of kilo breaks from a commonly-accepted prefix character (ex: k, m, g, t)
	// - case-insensitive
	// - returns -1 from invalid input
	const int GetKiloPositionByPrefix(const char Prefix) noexcept;

	// returns number of decimal places represented by a commonly-accepted prefix character (ex: k=3, m=6, g=9)
	// - case-insensitive
	// - returns -1 from invalid input
	const int GetDecimalPlacesByPrefix(const char Prefix) noexcept;

	// returns commonly-accepted prefix by number of kilo breaks (ex: 1=k, 2=m, 3=g)
	// - 0 returns a space (' ')
	// - negative numbers auto-converted to positive
	// - values above 8 (yotta) return '?'
	const char GetPrefixByKiloPosition(const int KiloPosition) noexcept;

	// returns decimal places matching a commonly-accepted prefix (ex: 3=k, m=6, g=9)
	// - returns a space (' ')
	// - negative numbers auto-converted to positive
	// - values above 24 (yotta) return '?'
	const char GetPrefixByDecimalPlaces(const int DecimalPlaces) noexcept;
}