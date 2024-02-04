#pragma once

#include "CodeParseTokenBase.h"

/** CodeParseTokenDelimiter
 *
 * Class used for delimiter tokens that output an empty line
 */
class CodeParseTokenDelimiter : public CodeParseTokenBase
{
public:
    CodeParseTokenDelimiter() : CodeParseTokenBase() {}

    //~ Begin CodeParseTokenBase Interface
    virtual void WriteToFile(std::ofstream& outputFile) override;
    //~ End CodeParseTokenBase Interface
};
