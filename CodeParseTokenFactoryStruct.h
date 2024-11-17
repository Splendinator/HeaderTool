﻿#pragma once

#include "CodeParseTokenFactoryUDT.h"

/** CodeParseTokenFactoryStruct
*
* This struct is used to create CodeParseTokenStruct objects
*/
struct CodeParseTokenFactoryStruct : public CodeParseTokenFactoryUDT
{
public:
	
	//~ Begin CodeParseTokenFactoryBase Interface
	bool IsKeyword(const std::string& keyword) override;
	//~ End CodeParseTokenFactoryBase Interface

	//~ Begin CodeParseTokenFactoryUDT Interface
	virtual CodeParseTokenBase* CreateUDTToken(const std::string& udtName, const std::vector<std::string>& baseUdts) override;
	//~ End CodeParseTokenFactoryUDT Interface

	
	
};
