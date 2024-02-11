#include "CodeParseTokenPropertyFloat.h"

#include "WritePropertyToFileUtils.h"

void CodeParseTokenPropertyFloat::WriteToFile(std::ofstream& outputFile)
{
	PropertyFileUtils::WriteFloatToFile(outputFile, propertyName, defaultValue);
}

std::string CodeParseTokenPropertyFloat::GenerateSetPropertyCode(const std::string& lValueString, const std::string& propertyTypeCode) const
{
	return lValueString + " = static_cast<EditorTypePropertyFloat*>(" + propertyTypeCode + ")->GetValue();\n";
}
