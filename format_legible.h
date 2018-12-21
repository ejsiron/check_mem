#pragma once

#include <iomanip>
#include <locale>
#include <sstream>
#include <string>
#include "map_positivepowerprefix.h"

namespace format_legible
{
	constexpr int KiloBreak { 1024 };

	// returns numbers greater than KiloBreak as human-readable numbers with commonly accepted prefixes
	// ex: 1073741824 returns "1 GB"
	// - SourceNumber: numeric raw value
	// - Precision: maximum number of decimal places in output string. default: 1
	template <typename T>
	std::string FriendlyFormatBytes(T SourceNumber, std::streamsize Precision = 1)
	{
		std::stringstream NumberStream;
		NumberStream << std::fixed;
		auto KiloBreaks = 0;
		if (SourceNumber <= KiloBreak)
		{
			NumberStream << SourceNumber;
		}
		else
		{
			NumberStream.precision(Precision);
			auto TranslatedNumber = SourceNumber / 1.0;
			while (TranslatedNumber > KiloBreak || TranslatedNumber < -KiloBreak)
			{
				++KiloBreaks;
				TranslatedNumber /= KiloBreak;
			}
			NumberStream.imbue(std::locale(""));
			std::use_facet<std::num_put<char>>(NumberStream.getloc()).put(NumberStream, NumberStream, '0', TranslatedNumber);
		}

		const char prefix{ map_positivepowerprefix::GetPrefixByKiloPosition(KiloBreaks) };
		NumberStream << std::setw(prefix == ' ' ? 1 : 2);
		NumberStream << prefix;
		NumberStream << 'B';

		return NumberStream.str();
	}

	// returns any number as a percentage in string format
	// prevents breaking into scientific notation
	// - Percentage: raw number
	// - Precision: maximum number of decimal places in output string. default: 1
	template <typename T>
	std::string FriendlyFormatPercentage(T Percentage, std::streamsize Precision = 1)
	{
		std::stringstream NumberStream;
		NumberStream << std::fixed;
		NumberStream.precision(Precision);
		NumberStream << Percentage << '%';
		return NumberStream.str();
	}

	// returns a string representing Value's percent of Total
	// ex: Value 23, Total 100 returns "23%"
	// - Value: value to calculate as percentage against Total
	// - Total: total number that Value is a percentage of
	// - Precision: maximum number of decimal places in output string. default: 1
	template <typename T, typename U>
	std::string FriendlyFormatCalculatedPercentage(T Value, U Total, std::streamsize Precision = 1)
	{
		if (Total)
		{
			return FriendlyFormatPercentage(1.0 * Value / Total * 100, Precision);
		}
		else
		{
			return std::string("nan%");
		}
	}
}