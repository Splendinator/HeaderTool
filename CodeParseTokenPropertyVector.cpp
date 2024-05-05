#include "CodeParseTokenPropertyVector.h"

#include "CodeParseTokenClass.h"
#include "CodeParseTokenStruct.h"
#include "HeaderTool.h"
#include "HeaderToolUtils.h"

#include "DomLog/DomLog.h"

#include "DomWindow/DomWindow.h"

void CodeParseTokenPropertyVector::WriteToFile(std::ofstream& outputFile)
{
	outputFile << "vector " << propertyName << " " << dataType << std::endl;
}

std::string CodeParseTokenPropertyVector::GenerateSetPropertyCode(const std::string& lValueString, const std::string& propertyTypeCode, HeaderTool& headerTool) const
{
	std::string returnVal = 
		"{\n"
		"\t\tEditorTypePropertyVector* pVectorProperty = static_cast<EditorTypePropertyVector*>(" + propertyTypeCode + ");\n"
		"\t\tfor (std::unique_ptr<EditorTypePropertyBase>& instancedProperty : pVectorProperty->instancedProperties)\n"
		"\t\t{\n";
	
	// How we cast the GetValue() return changes based off the data type
	switch(GetDataTypeClassification(headerTool))
	{
	case EDataTypeClassification::Float:
	{
		returnVal +=
			"\t\t\t" + lValueString + ".push_back(static_cast<EditorTypePropertyFloat*>(instancedProperty.get())->GetValue());\n";
		break;
	}
	case EDataTypeClassification::Int:
	{
		returnVal +=
			"\t\t\t" + lValueString + ".push_back(static_cast<EditorTypePropertyInt*>(instancedProperty.get())->GetValue());\n";
		break;
	}
	case EDataTypeClassification::Bool:
	{
		returnVal +=
			"\t\t\t" + lValueString + ".push_back(static_cast<EditorTypePropertyBool*>(instancedProperty.get())->GetValue());\n";
		break;
	}
	case EDataTypeClassification::String:
	{
		returnVal +=
			"\t\t\t" + lValueString + ".push_back(static_cast<EditorTypePropertyString*>(instancedProperty.get())->GetValue());\n";
		break;
	}
	case EDataTypeClassification::Class:
	{
		returnVal +=
			"\t\t\t" + lValueString + ".push_back(static_cast<" + dataType + "*>(static_cast<EditorTypePropertyClass*>(instancedProperty.get())->GetValue()));\n";
		break;
	}
	default:
	{
		DOMLOG_ERROR("Unsupported type");
		break;
	}
	}
	
	returnVal +=
		"\t\t}\n"
		"\t}\n";

	return returnVal;
}

CodeParseTokenPropertyVector::EDataTypeClassification CodeParseTokenPropertyVector::GetDataTypeClassification(HeaderTool& headerTool) const
{
	for (std::string& floatType : HeaderToolUtils::floatTypes)
	{
		if (floatType == dataType)
		{
			return EDataTypeClassification::Float;
		}
	}

	for (std::string& intType : HeaderToolUtils::intTypes)
	{
		if (intType == dataType)
		{
			return EDataTypeClassification::Int;
		}
	}
	
	for (std::string& boolType : HeaderToolUtils::boolTypes)
	{
		if (boolType == dataType)
		{
			return EDataTypeClassification::Bool;
		}
	}

	for (std::string& stringType : HeaderToolUtils::stringTypes)
	{
		if (stringType == dataType)
		{
			return EDataTypeClassification::String;
		}
	}
	
	for (CodeParseTokenBase* pToken : headerTool.GetAllTokens())
	{
		if (pToken->GetSatisfiedDependency() == dataType)
		{
			if (dynamic_cast<CodeParseTokenClass*>(pToken))
			{
				return EDataTypeClassification::Class;
			}
			if (dynamic_cast<CodeParseTokenStruct*>(pToken))
			{
				return EDataTypeClassification::Struct;
			}
		}
	}

	DOMLOG_ERROR("Can't find the type", dataType)
	return EDataTypeClassification::INVALID;
}
