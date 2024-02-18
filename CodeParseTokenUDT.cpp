#include "CodeParseTokenUDT.h"

void CodeParseTokenUDT::WriteToFile(std::ofstream& outputFile)
{
	outputFile << GetUDTTypeName() << " " << udtName << std::endl;

	for (const std::string& baseUdt : baseUdtNames)
	{
		outputFile << "Child " << baseUdt << std::endl;
	}
}
