#include "CodeParseTokenPropertyVector.h"

#include "CodeParseTokenClass.h"
#include "CodeParseTokenEnum.h"
#include "CodeParseTokenStruct.h"
#include "HeaderTool.h"
#include "HeaderToolUtils.h"

#include "DomLog/DomLog.h"

#include "DomWindow/DomWindow.h"

void CodeParseTokenPropertyVector::WriteToFile(std::ofstream& outputFile)
{
	std::string basicType = dataType;
	
#define MAP_BASIC_TYPE(Array, String) \
	for (std::string& floatType : Array)\
	{\
		if (floatType == dataType)\
		{\
			basicType = String;\
		}\
	};

	// We don't need to go as granular as "unsigned int 64". The vector just needs to know the general data type "int".
	// This is because when we come to generating the code, the same code will work for all int types (thanks to implicit casting)
	MAP_BASIC_TYPE(HeaderToolUtils::floatTypes, "float");
	MAP_BASIC_TYPE(HeaderToolUtils::boolTypes, "bool");
	MAP_BASIC_TYPE(HeaderToolUtils::intTypes, "int");
	MAP_BASIC_TYPE(HeaderToolUtils::stringTypes, "string");
	
	outputFile << "vector " << propertyName << " " << basicType << std::endl;
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
	case EDataTypeClassification::Struct:
	{
		returnVal +=
			"\t\t\t" + lValueString + ".push_back(*static_cast<" + dataType + "*>(static_cast<EditorTypePropertyStruct*>(instancedProperty.get())->GetValue()));\n";
		break;
	}
	case EDataTypeClassification::Enum:
	{
		returnVal +=
			"\t\t\t" + lValueString + ".push_back(static_cast<" + dataType + ">(static_cast<EditorTypePropertyEnum*>(instancedProperty.get())->GetValue()));\n";
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
			if (dynamic_cast<CodeParseTokenEnum*>(pToken))
			{
				return EDataTypeClassification::Enum;
			}
		}
	}

	DOMLOG_ERROR("Can't find the type", dataType)
	return EDataTypeClassification::INVALID;
}
