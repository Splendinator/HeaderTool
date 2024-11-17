#pragma once

#include <fstream>
#include <vector>

// Type of code parse token
enum class ECodeParseTokenType
{	
	UNUSED,	// The type of most code parse tokens isn't needed.
	ClassMetadata,
	StructMetadata,
	Delimiter,
};

enum class ECodeParseTokenPriority
{	
	Low,
	Medium,
	High,
	COUNT
};

/** CodeParseTokenBase
*
* This class is used as a token for reading in code files. It represents an useful part of a code file for the purpose of gathering data to be used in the ImGuiEditor (see ImGuiEditor.h)
* A token's primary purpose is to write useful data (class name, property name+default value, etc.) to a master file.
*/
class CodeParseTokenBase
{
public:

	CodeParseTokenBase() {}
	virtual ~CodeParseTokenBase() {};
	

	// Override to write this token to a file.
	virtual void WriteToFile(std::ofstream& outputFile) = 0;

	// Optional functions that return which dependency is satisfied/required when this token is written to the file.
	// Used to ensure base classes are written before child classes etc.
	virtual std::string GetSatisfiedDependency() const { return ""; }
	virtual std::vector<std::string> GetRequiredDependencies() const { return {}; }

	// Priority within the block, this is used to re-order code parse tokens within a block (blocks are seperated by CodeParseTokenDelimiter). Higher comes first
	// e.g. "Class -> Class Metadata -> Class Properties"
	virtual ECodeParseTokenPriority GetPriorityWithinBlock() const {return ECodeParseTokenPriority::Low;}

	virtual ECodeParseTokenType GetCodeParseTokenType() const {return ECodeParseTokenType::UNUSED;} 
};
