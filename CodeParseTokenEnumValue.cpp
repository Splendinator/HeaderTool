#include "CodeParseTokenEnumValue.h"

void CodeParseTokenEnumValue::WriteToFile(std::ofstream& outputFile)
{
	outputFile << name << " " << value << std::endl;
}
