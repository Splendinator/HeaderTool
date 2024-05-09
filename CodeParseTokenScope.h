#pragma once

#include "CodeParseTokenBase.h"

/** CodeParseTokenScope
 *
 * Used to represent a scope after a class/struct/enum.
 */
class CodeParseTokenScope : public CodeParseTokenBase
{
public:
	//~ Begin CodeParseTokenBase Interface
	virtual void WriteToFile(std::ofstream& outputFile) override {};
	//~ End CodeParseTokenBase Interface
};
