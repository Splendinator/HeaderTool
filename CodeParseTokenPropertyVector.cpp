#include "CodeParseTokenPropertyVector.h"

void CodeParseTokenPropertyVector::WriteToFile(std::ofstream& outputFile)
{
	outputFile << "vector " << propertyName << " " << dataType << std::endl;
}

std::string CodeParseTokenPropertyVector::GenerateSetPropertyCode(const std::string& lValueString, const std::string& propertyTypeCode) const
{
	return
	"{\n"
	"\t\tEditorTypePropertyVector* pVectorProperty = static_cast<EditorTypePropertyVector*>(" + propertyTypeCode + ");\n"
	"\t\tfor (std::unique_ptr<EditorTypePropertyBase>& instancedProperty : pVectorProperty->instancedProperties)\n"
	"\t\t{\n"
	"\t\t\t" + lValueString + ".push_back(static_cast<EditorTypePropertyFloat*>(instancedProperty.get())->GetValue());\n"
	"\t\t}\n"
	"\t}\n";

	// #TEMP: we need to fill in EVectorDataType in a post-parse pass and then change EditorTypePropertyFloat above appropriately. We might need to do something wild like child code tokens when it comes to structs. I'm not sure. Might just be able to use InitFromPropertiesSubset?
}
