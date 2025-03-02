#pragma once

#include "CodeParseTokenFactoryBase.h"

class CodeParseTokenFactoryClassMetadataFlagEditInlineNew : public CodeParseTokenFactoryBase
{
protected:
    //~ Begin CodeParseTokenFactoryBase Interface
    bool IsKeyword(const std::string& keyword) override;
    CodeParseTokenBase* CreateToken(const std::string& keyword, std::stringstream& stream, CodeParseTokenFactoryBase* pOuterScopedFactory) override;
    //~ End CodeParseTokenFactoryBase Interface
};
