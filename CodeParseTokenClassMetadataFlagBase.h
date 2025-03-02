#pragma once

#include "CodeParseTokenBase.h"

// Metadata e.g. EDITORCLASS(Abstract, Instanced)

class CodeParseTokenClassMetadataFlagBase : public CodeParseTokenBase
{
public:
	//~ Begin CodeParseTokenBase Interface
	void WriteToFile(std::ofstream& outputFile) override;
	ECodeParseTokenType GetCodeParseTokenType() const override {return ECodeParseTokenType::ClassMetadata;}
	//~ End CodeParseTokenBase Interface
	
protected:

	virtual std::string GetKeyword() = 0;
};

class CodeParseTokenClassMetadataFlagAbstract : public CodeParseTokenClassMetadataFlagBase
{
protected:
    //~ Begin CodeParseTokenClassMetadataFlagBase Interface
    std::string GetKeyword() override { return "Abstract"; }
    //~ End CodeParseTokenClassMetadataFlagBase Interface
};

class CodeParseTokenClassMetadataFlagInstanced : public CodeParseTokenClassMetadataFlagBase
{
protected:
    //~ Begin CodeParseTokenClassMetadataFlagBase Interface
    std::string GetKeyword() override { return "Instanced"; }
    //~ End CodeParseTokenClassMetadataFlagBase Interface
};

class CodeParseTokenClassMetadataFlagSingleton : public CodeParseTokenClassMetadataFlagBase
{
protected:
    //~ Begin CodeParseTokenClassMetadataFlagBase Interface
    std::string GetKeyword() override { return "Singleton"; }
    //~ End CodeParseTokenClassMetadataFlagBase Interface
};

class CodeParseTokenClassMetadataFlagEditInlineNew : public CodeParseTokenClassMetadataFlagBase
{
protected:
    //~ Begin CodeParseTokenClassMetadataFlagBase Interface
    std::string GetKeyword() override { return "EditInlineNew"; }
    //~ End CodeParseTokenClassMetadataFlagBase Interface
};