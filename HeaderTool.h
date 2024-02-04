#pragma once

#include <string>
#include <vector>

class CodeParseTokenBase;

// Class to parse headers in the game and extract metadata about the code.
// This header tool is run *before* compiling the game.
class HeaderTool
{
public:
	void Run();

private:
	
	// Parse all code files in the input directory and collate their types into a master file
	void CollateTypesToFile(const std::string& codeDirectory, const std::string& typesFile);

	// Return all file paths of code files in a given directory
	std::vector<std::string> GatherCodeFilesInDirectory(const std::string& directory);

	// Parse a code file and get all the tokens
	void Parse(std::string codeFile);

	// Break down a parse string into smaller substrings that are usefor for more finely parsing the string if the code has a lot of things without spaces in.
	// i.e "GetSomething(){return;};" might return "GetSomething", "(", ")", "{", "return", ";", "}"
	std::vector<std::string> BreakDownParseString(const std::string& parseString) const;

	// Writes the generated code files. Generates a way for the game to create instanced classed from their data assets and string names.
	void WriteGeneratedCodeFiles(const std::string& directory);

	// Write the global __Generated file. (Contains things like class names to create functions)
	void WriteGlobalHeaderFile(std::ofstream& file);
	void WriteGlobalCppFile(std::ofstream& file);

	// Vector of all tokens, populated by reading the code files.
	std::vector<CodeParseTokenBase*> allTokens;

	// All code files that contain at least one piece of editor data. Used to #include them all in the generated cpp
	std::vector<std::string> allRelevantCodeFiles;
};

