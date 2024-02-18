#pragma once

#include "CodeParseTokenBase.h"
#include <string>
#include <fstream>

/** CodeParseTokenUDT
 *
 * Base class for user-defined types (class or struct)
 */
class CodeParseTokenUDT : public CodeParseTokenBase
{
public:
	CodeParseTokenUDT(std::string inName, std::vector<std::string> baseNames = {}) : udtName(inName), baseUdtNames(baseNames) {}
	
	//~ Begin CodeParseTokenBase Interface
	virtual void WriteToFile(std::ofstream& outputFile) override;
	virtual std::string GetSatisfiedDependency() const { return udtName; }
	virtual std::vector<std::string> GetRequiredDependencies() const { return baseUdtNames; }
	//~ End CodeParseTokenBase Interface

	virtual std::string GetUDTTypeName() = 0; // Type of UDT (Class/Struct)
	
	std::string udtName; // Name of UDT ("MyClass"/"MyStruct")
	std::vector<std::string> baseUdtNames; // Base class(es) of the UDT
};
