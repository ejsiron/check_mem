#include <cctype>
#include <getopt.h>
#include <iostream>
#include <locale>
#include <string>
#include <sys/sysinfo.h>
#include "format_legible.h"
#include "sanitize_numericinput.h"

using namespace format_legible;
using namespace sanitize_numericinput;
using namespace std;

constexpr int NAGSTAT_OK{0};
constexpr int NAGSTAT_WARN{1};
constexpr int NAGSTAT_CRIT{2};
constexpr int NAGSTAT_UNKNOWN{3};

constexpr char HelpText[]{R"ht(** check_mem Module for Nagios-compatible systems **
Outputs current free memory in relation to total memory.
Formatted for human-readability and performance data gathering.
Optionally includes warning and critical state.

-w, --warning=VALUE: fixed number or percent representing low-point warning level
-c, --critical=VALUE: fixed number of percent representing low-point critical level
-h, --help: show this help; overrides all other settings

Numeric values can be entered as regular numbers, percentages, or with commonly-accepted prefixes.
Prefixed numbers work on a basis of 1 kilobyte = 1024 bytes

Examples:
check_mem -w 20% -c 1048576
- Outputs free memory, warning below 20% and critical below 1 megabyte;
  either field accepts a percentage or a fixed number
check_mem -w 20% -c 1MB
- Same as previous, check_mem understands common prefixes from (k)ilo to (y)otta
check_mem -w 20% -c 1mb
- Same as previous; -w and -c are case-insensitive.
)ht"};

// converts percentage measures to concrete unsigned long; converts calculated floating-point levels to unsigned long
const unsigned long GetLevel(const unsigned long Value, const unsigned long Maximum, const pair<long double, bool> &Level) noexcept
{
	if (Level.second) // this is a percentage
	{
		return static_cast<unsigned long>(Maximum * (Level.first / 100));
	}
	else
	{
		return static_cast<unsigned long>(Level.first);
	}
}

int main(int argc, char **argv)
{
	setlocale(LC_ALL, "");
	int RetVal{NAGSTAT_OK};

	errno = 0;
	struct sysinfo SystemInformation;
	if (sysinfo(&SystemInformation))
	{
		std::cout << "sysinfo returned error code" << errno;
		return NAGSTAT_UNKNOWN;
	}

	static struct option long_options[] = {
		 {"warning", required_argument, 0, 'w'},
		 {"critical", required_argument, 0, 'c'},
		 {"help", no_argument, 0, 'h'},
		 {0, 0, 0, 0}};

	int EnteredOptionCharacter{0};
	int EnteredOptionIndex{0};
	bool ShowHelp{false};
	string Warning{};
	unsigned long WarningLevel{0};
	string Critical{};
	unsigned long CriticalLevel{0};
	pair<unsigned long, bool> ProcessedNumeric;
	while (EnteredOptionCharacter != -1)
	{
		EnteredOptionCharacter = getopt_long(argc, argv, "w:c:h", long_options, &EnteredOptionIndex);
		switch (EnteredOptionCharacter)
		{
		case 'w':
			Warning.assign(optarg);
			break;
		case 'c':
			Critical.assign(optarg);
			break;
		default:
			if (EnteredOptionCharacter > -1)
			{
				ShowHelp = true;
			}
			break;
		}
	}

	if (ShowHelp)
	{
		cout << HelpText;
		return RetVal;
	}

	if (Critical.size())
	{
		CriticalLevel = GetLevel(SystemInformation.freeram, SystemInformation.totalram, SanitizeNumericInput(Critical));
		if (SystemInformation.freeram < CriticalLevel)
		{
			RetVal = NAGSTAT_CRIT;
			std::cout << "CRITICAL";
		}
	}
	if (Warning.size())
	{
		WarningLevel = GetLevel(SystemInformation.freeram, SystemInformation.totalram, SanitizeNumericInput(Warning));
		if (!RetVal && SystemInformation.freeram < WarningLevel)
		{
			RetVal = NAGSTAT_WARN;
			std::cout << "WARNING";
		}
	}
	if (RetVal == NAGSTAT_OK)
	{
		std::cout << "OK";
	}

	// output friendly first
	auto FriendlyTotalMemory = FriendlyFormatBytes(SystemInformation.totalram);
	auto FriendlyFreeMemory = FriendlyFormatBytes(SystemInformation.freeram);

	std::cout << "- " << std::fixed << FriendlyFreeMemory << " of " << FriendlyTotalMemory << " free (" << FriendlyFormatCalculatedPercentage(SystemInformation.freeram, SystemInformation.totalram) << ")";

	// output performance data; most values inverted
	auto UsedRam = SystemInformation.totalram - SystemInformation.freeram;
	WarningLevel = WarningLevel ? (SystemInformation.totalram - WarningLevel) : WarningLevel;
	CriticalLevel = CriticalLevel ? (SystemInformation.totalram - CriticalLevel) : CriticalLevel;
	std::cout << "| Memory=" << UsedRam << "B;" << std::setprecision(0) << WarningLevel << ';' << CriticalLevel << ';' << "0;" << SystemInformation.totalram << '\n';
	return RetVal;
}
