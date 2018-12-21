#include <algorithm>
#include <locale>
#include <map>
#include "map_positivepowerprefix.h"

using std::map;
using std::pair;

namespace map_positivepowerprefix
{
	// 3 decimal places == 1 kilo place
	constexpr int KiloPlaces{ 3 };

	// map number of kilo breaks to commonly-accepted single character prefixes (kilo, mega, giga, etc.)
	static const map<const size_t, const char> PrefixMap{
		{0, ' '},
		{1, 'K'},
		{2, 'M'},
		{3, 'G'},
		{4, 'T'},
		{5, 'P'},
		{6, 'E'},
		{7, 'Z'},
		{8, 'Y'}
	};

	const int GetKiloPositionByPrefix(const char Prefix) noexcept
	{
		std::locale CurrentLocale{ std::locale("") };
		auto FoundMatch = std::find_if(
			PrefixMap.cbegin(),
			PrefixMap.cend(),
			[&Prefix, &CurrentLocale](const pair<const int, const char>& PrefixMapItem) { return PrefixMapItem.second == std::toupper(Prefix, CurrentLocale); });
		return FoundMatch == PrefixMap.end() ? -1 : static_cast<const int>(FoundMatch->first);
	}

	const int GetDecimalPlacesByPrefix(const char Prefix) noexcept
	{
		auto KiloPosition = GetKiloPositionByPrefix(Prefix);
		return KiloPosition == -1 ? -1 : KiloPosition * KiloPlaces;
	}

	const char GetPrefixByKiloPosition(const int KiloPosition) noexcept
	{
		try
		{
			return PrefixMap.at(static_cast<size_t>(std::abs(KiloPosition)));
		}
		catch (...)
		{
			return '?';
		}
	}

	const char GetPrefixByDecimalPlaces(const int DecimalPlaces) noexcept
	{
		return GetPrefixByKiloPosition(DecimalPlaces / KiloPlaces);
	}
}