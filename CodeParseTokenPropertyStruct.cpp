#include "CodeParseTokenPropertyStruct.h"

#include "WritePropertyToFileUtils.h"

void CodeParseTokenPropertyStruct::WriteToFile(std::ofstream& outputFile)
{
	PropertyFileUtils::WriteStructToFile(outputFile, propertyName, structName);
}

std::string CodeParseTokenPropertyStruct::GenerateSetPropertyCode(const std::string& lValueString, const std::string& propertyTypeCode) const
{
	// #TODO: This could use InitFromPropertiesSubset() to set the struct up inline without new/delete
	return
	"{\n" + 
	structName + "* temp = static_cast<" + structName + "*>(static_cast<EditorTypePropertyStruct*>(" + propertyTypeCode + ")->GetValue());\n"
	+ lValueString + " = *temp;\n"
	"delete temp;\n"
	"}\n"; 
}
