#include "CodeParseTokenPropertyInstancedAssetPtr.h"

#include "Editor/Types/Properties/WritePropertyToFileUtils.h"

void CodeParseTokenPropertyInstancedAssetPtr::WriteToFile(std::ofstream& outputFile)
{
	PropertyFileUtils::WriteInstancedAssetPtrToFile(outputFile, propertyName, className);
}

std::string CodeParseTokenPropertyInstancedAssetPtr::GenerateSetPropertyCode(const std::string& lValueString, const std::string& propertyTypeCode, HeaderTool& headerTool) const
{
	return lValueString + ".SetAsset(static_cast<EditorTypePropertyInstancedAssetPtr*>(" + propertyTypeCode + ")->GetValue());\n";	
}
