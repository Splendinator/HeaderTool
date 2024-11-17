#pragma once

#include "CodeParseTokenBase.h"

class CodeParseTokenClassMetadata : public CodeParseTokenBase
{
public:
	//~ Begin CodeParseTokenBase Interface
	void WriteToFile(std::ofstream& outputFile) override;
	ECodeParseTokenType GetCodeParseTokenType() const override {return ECodeParseTokenType::ClassMetadata;}
	ECodeParseTokenPriority GetPriorityWithinBlock() const override {return ECodeParseTokenPriority::Medium;}
	//~ End CodeParseTokenBase Interface
};
