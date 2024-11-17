#include "CodeParseTokenClassMetadataFlagBase.h"

void CodeParseTokenClassMetadataFlagBase::WriteToFile(std::ofstream& outputFile)
{
	outputFile << "meta " << GetKeyword() << "\n";
}
