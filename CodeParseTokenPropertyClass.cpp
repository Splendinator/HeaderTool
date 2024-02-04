#include "CodeParseTokenPropertyClass.h"

#include "WritePropertyToFileUtils.h"

void CodeParseTokenPropertyClass::WriteToFile(std::ofstream& outputFile)
{
	PropertyFileUtils::WriteClassToFile(outputFile, propertyName, className);
}
