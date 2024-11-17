#pragma once

#include "CodeParseTokenBase.h"

class CodeParseTokenStructMetadata : public CodeParseTokenBase
{
public:
	//~ Begin CodeParseTokenBase Interface
	void WriteToFile(std::ofstream& outputFile) override;
	ECodeParseTokenType GetCodeParseTokenType() const override {return ECodeParseTokenType::StructMetadata;}
	ECodeParseTokenPriority GetPriorityWithinBlock() const {return ECodeParseTokenPriority::Medium;}
	//~ End CodeParseTokenBase Interface
};
