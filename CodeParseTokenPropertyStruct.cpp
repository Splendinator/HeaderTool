#include "CodeParseTokenPropertyStruct.h"

#include "WritePropertyToFileUtils.h"

void CodeParseTokenPropertyStruct::WriteToFile(std::ofstream& outputFile)
{
	PropertyFileUtils::WriteStructToFile(outputFile, propertyName, structName);
}

std::string CodeParseTokenPropertyStruct::GenerateSetPropertyCode(const std::string& lValueString, const std::string& propertyTypeCode, HeaderTool& headerTool) const
{
	// #TODO: This could use InitFromPropertiesSubset() to set the struct up inline without new/delete
	return
	"{\n\t\t" + 
	structName + "* temp = static_cast<" + structName + "*>(static_cast<EditorTypePropertyStruct*>(" + propertyTypeCode + ")->GetValue());\n\t\t"
	+ lValueString + " = *temp;\n\t\t"
	"delete temp;\n\t"
	"}\n"; 
}
