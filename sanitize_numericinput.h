#pragma once

#include <string>
#include <utility>

namespace sanitize_numericinput
{
	// extracts a usable number from an input string, marking it if it contains a % sign
	// - long double in pair contains the sanitized number
	// - bool in the pair indicates the presence of a % sign
	// non-numeric and non-% characters are discarded except the first decimal separator (locale-aware)
	// decimal characters after the first are discarded
	std::pair<long double, bool> SanitizeNumericInput(const std::string& Input);
}