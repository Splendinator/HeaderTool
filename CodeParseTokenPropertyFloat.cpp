#include "CodeParseTokenPropertyFloat.h"

#include "WritePropertyToFileUtils.h"

void CodeParseTokenPropertyFloat::WriteToFile(std::ofstream& outputFile)
{
	PropertyFileUtils::WriteFloatToFile(outputFile, propertyName, defaultValue);
}
