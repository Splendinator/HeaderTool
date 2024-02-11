#include "CodeParseTokenStruct.h"

void CodeParseTokenStruct::WriteToFile(std::ofstream& outputFile)
{
	outputFile << "Struct " << structName;
		
	if (baseStruct != "") 
	{
		outputFile << " " << baseStruct;
	}

	outputFile << std::endl;
}

