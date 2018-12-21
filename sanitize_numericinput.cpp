#include <cmath>
#include <locale>
#include <sstream>
#include "map_positivepowerprefix.h"
#include "sanitize_numericinput.h"

namespace sanitize_numericinput
{
	static const char GetDecimalPoint() noexcept
	{
		return std::use_facet<std::numpunct<char>>(std::locale("")).decimal_point();
	}

	std::pair<long double, bool> SanitizeNumericInput(const std::string& Input)
	{
		std::stringstream OutputStream{};
		bool PercentFlag{ false };
		char UserDecimalPoint = GetDecimalPoint();
		int Exponent{ -1 };
		bool DecimalFound{ false };

		for (auto const& inchar : Input)
		{
			if (std::isdigit(inchar)) { OutputStream << inchar; }
			else if (std::isalpha(inchar) && Exponent == -1)
			{
				int PotentialExponent(map_positivepowerprefix::GetKiloPositionByPrefix(inchar));
				if (PotentialExponent != -1) { Exponent = PotentialExponent; }
			}
			else if (inchar == '%') { PercentFlag = true; }
			else if (inchar == UserDecimalPoint)
			{
				if (!DecimalFound)
				{
					OutputStream << inchar;
					DecimalFound = true;
				}
			}
		}
		long double ParsedNumber{ std::stold(OutputStream.str()) };
		if (Exponent > 0)
		{
			double Multiplier{ std::pow(1024, Exponent) };
			ParsedNumber *= Multiplier;
		}
		return std::make_pair(ParsedNumber, PercentFlag);
	}
}
