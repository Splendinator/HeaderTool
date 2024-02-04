#include "CodeParseTokenClass.h"

void CodeParseTokenClass::WriteToFile(std::ofstream& outputFile)
{
	outputFile << "Class " << className;
		
	if (baseClass != "")
	{
		outputFile << " " << baseClass;
	}

	outputFile << std::endl;
}

