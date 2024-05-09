#include "CodeParseTokenEnum.h"

void CodeParseTokenEnum::WriteToFile(std::ofstream& outputFile)
{
	outputFile << "Enum " << name << std::endl;
}
