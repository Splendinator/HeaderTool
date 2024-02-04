#pragma once

#include "CodeParseTokenBase.h"

/** CodeParseTokenScope
*
* This class is used for scope. It doesn't write to a file and is just used to add to the scope stack.
*/
class CodeParseTokenScope : public CodeParseTokenBase
{
public:
	//~ Begin CodeParseTokenBase Interface
	virtual void WriteToFile(std::ofstream& outputFile) override {};
	//~ End CodeParseTokenBase Interface
};
