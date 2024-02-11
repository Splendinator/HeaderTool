#include "HeaderTool.h"

#include "CodeParseTokenBase.h"
#include "CodeParseTokenClass.h"
#include "CodeParseTokenDelimiter.h"
#include "CodeParseTokenFactoryClass.h"
#include "CodeParseTokenFactoryProperty.h"
#include "CodeParseTokenFactoryScope.h"
#include "CodeParseTokenScope.h"
#include "DomImport/DomImport.h"
#include "ImGuiEditorGlobals.h"
#include "ImGuiEditorMacros.h"

#include <filesystem>
#include <fstream>
#include <set>
#include <sstream>

#include "CodeParseTokenFactoryStruct.h"
#include "CodeParseTokenPropertyBase.h"
#include "CodeParseTokenStruct.h"

#include "../../../../../VulkanSDK/1.2.198.1/Include/vulkan/vulkan_core.h"

#include "DomWindow/DomWindow.h"

// #TEMP: Optimisation
#pragma optimize("", off)

void HeaderTool::Run()
{
	CollateTypesToFile(ImGuiEditorGlobals::codeFilesBaseDirectory, ImGuiEditorGlobals::editorTypesOutputFile);

	WriteGeneratedCodeFiles(ImGuiEditorGlobals::generatedCodeDirectory);
}


void HeaderTool::CollateTypesToFile(const std::string& codeDirectory, const std::string& typesFile)
{
	std::vector<std::string> codeFiles = GatherCodeFilesInDirectory(codeDirectory);

	// Gather all tokens
	for (const std::string& codeFile : codeFiles)
	{
		std::filesystem::path path(codeFile);
		std::cout << path.filename() << std::endl;
		Parse(codeFile);
	}

	SortCodeParseTokens();

	// Write to file
	std::ofstream outputFile(codeDirectory + "\\" + typesFile);
	for (CodeParseTokenBase* token : allTokens)
	{
		token->WriteToFile(outputFile);
	}
}

std::vector<std::string> HeaderTool::GatherCodeFilesInDirectory(const std::string& directory)
{
	std::vector<std::string> codeFiles;

	for (const auto& entry : std::filesystem::directory_iterator(directory)) 
	{
		if (entry.is_regular_file())
		{
			const std::string extension = entry.path().extension().string();
			if (extension == ".h")
			{
				codeFiles.push_back(entry.path().string());
			}
		}
		else if (entry.is_directory())
		{
			std::vector<std::string> subDirectoryCodeFiles = GatherCodeFilesInDirectory(entry.path().string());
			codeFiles.insert(codeFiles.end(), subDirectoryCodeFiles.begin(), subDirectoryCodeFiles.end());
		}
	}

	return codeFiles;
}

void HeaderTool::Parse(std::string codeFile)
{
	dmut::HeapAllocSize<char> code = dmim::importText(codeFile.c_str());
	std::stringstream stream(code.RawPtr());

	bool bCodeFileContainsEditorData = false; // Whether there's at least 1 editor relevant thing in this file. (used to know whether to #include the headers)
	
	// Set up token factories
	CodeParseTokenFactoryClass codeParseTokenFactoryClass;
	CodeParseTokenFactoryStruct codeParseTokenFactoryStruct;
	CodeParseTokenFactoryProperty codeParseTokenFactoryProperty;
	CodeParseTokenFactoryScope codeParseTokenFactoryScope;

	// Preceeding tokens (Only allow properties inside classes etc.)
	codeParseTokenFactoryScope.AddRequiredPrecedingToken(&codeParseTokenFactoryClass);
	codeParseTokenFactoryScope.AddRequiredPrecedingToken(&codeParseTokenFactoryStruct);
	codeParseTokenFactoryScope.AddRequiredPrecedingToken(&codeParseTokenFactoryScope);
	codeParseTokenFactoryProperty.AddRequiredPrecedingToken(&codeParseTokenFactoryClass);
	codeParseTokenFactoryProperty.AddRequiredPrecedingToken(&codeParseTokenFactoryStruct);

	CodeParseTokenFactoryBase* pFactories[] =
	{
		&codeParseTokenFactoryClass,
		&codeParseTokenFactoryStruct,
		&codeParseTokenFactoryProperty,
		&codeParseTokenFactoryScope
	};

	// Parse code
	std::stack<CodeParseTokenFactoryBase*> activeScopedFactories; // Token factories that are currently active (i.e. we are inside a class, struct, etc)
	
	while (!stream.eof())
	{
		// #TODO: We need to not parse string literals or comments or whatever
		std::string nextString;
		stream >> nextString;
		if (nextString == "")
		{
			continue;
		}

		std::vector<std::string> subStrings = BreakDownParseString(nextString);
		for (std::string substring : subStrings)
		{
			// Check for file requesting to be ignored
			if (nextString == ImGuiEditorMacros::editorIgnoreFileString)
			{
				DOMLOG_ERROR_IF(bCodeFileContainsEditorData, "Ignoring a file that is mid way through being parsed")
				return;
			}
			
			// Check for end of scope
			if (!activeScopedFactories.empty())
			{
				CodeParseTokenFactoryBase* pScopedTokenType = activeScopedFactories.top();
				std::string endString = pScopedTokenType->GetEndString();

				if (substring == endString)
				{
					activeScopedFactories.pop();

					if (activeScopedFactories.empty())
					{
						// #JANK: We're just assuming if the outermost scope ends that we want newline for now because I can't be arsed. This should instead be tied to the scope type (i.e. class, struct, etc.) 
						allTokens.push_back(new CodeParseTokenDelimiter);
					}

					continue;
				}
			}

			// Check factories
			for (CodeParseTokenFactoryBase* pFactory : pFactories)
			{
				if (pFactory->CanFactoryBeUsed(activeScopedFactories) && pFactory->IsKeyword(substring))
				{
					CodeParseTokenBase* pToken = pFactory->CreateToken(substring, stream);
					if (pToken)
					{
						allTokens.push_back(pToken);

						bCodeFileContainsEditorData = true;

						if (pFactory->IsScopedTokenType())
						{
							activeScopedFactories.push(pFactory);
						}

						break;
					}
					else
					{
						DOMLOG_ERROR("Cannot create token for keyword:", substring, "in file", codeFile);
					}
				}
			}
		}
	}

	if (bCodeFileContainsEditorData)
	{
		allRelevantCodeFiles.push_back(codeFile);
	}
	
	DOMLOG_ERROR_IF(!activeScopedFactories.empty(), "Scoped factories not detecting end of scope?")
}

void HeaderTool::SortCodeParseTokens()
{
	std::vector<CodeParseTokenBase*> sortedTokens;
	sortedTokens.reserve(allTokens.size());

	std::set<std::string> satisfiedDependencies; // Ever growing vector of satisfied dependencies

	// While tokens remain...
	while (!allTokens.empty())
	{
		int numTokensBeforeLoop = allTokens.size();
		int tokenIndex = numTokensBeforeLoop - 2; // Last token will be delimiter, so skip it
		int lastSegmentIndex = numTokensBeforeLoop - 1; 
		std::set<std::string> requiredDependencies;

		while (tokenIndex >= -1) // use "-1" so we can tell if we've reached the end (vs hitting a delimiter)
		{
			// Iterate all tokens backwards, finding segments (between delimiters and EOF) that can be added.
			
			CodeParseTokenBase* pToken = tokenIndex >= 0 ? allTokens[tokenIndex] : nullptr;
			if (tokenIndex == -1 || dynamic_cast<CodeParseTokenDelimiter*>(pToken))
			{
				// Delimiter found, iterate segment
				bool bDependenciesSatisfied = true;
				for (const std::string& requiredDependency : requiredDependencies)
				{
					if (satisfiedDependencies.find(requiredDependency) == satisfiedDependencies.end())
					{
						bDependenciesSatisfied = false;
						break;
					}
				}
				
				if (bDependenciesSatisfied)
				{
					// Dependencies satisfied, add the segment to the sorted array
					for (int segmentIndex = tokenIndex + 1; segmentIndex <= lastSegmentIndex; ++segmentIndex)
					{
						sortedTokens.push_back(allTokens[segmentIndex]);

						std::string requiredDependency = allTokens[segmentIndex]->GetSatisfiedDependency();
						if (requiredDependency != "")
						{
							// Mark satisfied dependencies as such
							satisfiedDependencies.emplace();
						}
					}

					// Remove the segment from the unsorted array
					allTokens.erase(allTokens.begin() +  tokenIndex + 1, allTokens.begin() + lastSegmentIndex + 1);
				}

				lastSegmentIndex = tokenIndex;
				--tokenIndex;
				requiredDependencies.erase(requiredDependencies.begin(), requiredDependencies.end());
			}
			else
			{
				// No delimiter, continue along section
				std::string requiredDependency = pToken->GetRequiredDependency();
				if (requiredDependency != "")
				{
					requiredDependencies.emplace();
				}
				--tokenIndex;
			}
		}

		if (allTokens.size() == numTokensBeforeLoop)
		{
			DOMLOG_ERROR("Infinite Loop! Circular Dependencies?")
			return;
		}
	}

	allTokens = sortedTokens;
}

std::vector<std::string> HeaderTool::BreakDownParseString(const std::string& parseString) const
{
	// #TODO: If we ever add "(" or ")" we need to make sure they don't collide with EDITORCLASS() etc.
	static const std::string relevantStringsToExtract[] =
	{
		"//",
		"{",
		"}",
		ImGuiEditorMacros::editorClassString,
		ImGuiEditorMacros::editorStructString,
		ImGuiEditorMacros::editorPropertyString,
		ImGuiEditorMacros::editorIgnoreFileString,
	};

	struct FoundRelevantString
	{
		size_t index;
		std::string relevantString;
	};
	std::vector<FoundRelevantString> foundRelevantStrings;

	// Populate foundRelevantStrings
	for (const std::string& relevantString : relevantStringsToExtract)
	{
		size_t pos = (size_t)-1;
		while ((pos = parseString.find(relevantString, pos + 1)) != std::string::npos)
		{
			FoundRelevantString foundRelevantString;
			foundRelevantString.index = pos;
			foundRelevantString.relevantString = relevantString;
			foundRelevantStrings.push_back(foundRelevantString);
		}
	}
	std::sort(foundRelevantStrings.begin(), foundRelevantStrings.end(), [](const FoundRelevantString& a, const FoundRelevantString& b) { return a.index < b.index; });

	// Break down parse string
	std::vector<std::string> subStrings;
	size_t lastPos = 0;
	for (const FoundRelevantString& foundRelevantString : foundRelevantStrings)
	{
		if (foundRelevantString.relevantString == "//")
		{
			// If we find an inline comment then ignore any preceding strings.
			// #TODO: This won't work with multiline comments
			return subStrings;
		}
		
		std::string prevString = parseString.substr(lastPos, foundRelevantString.index - lastPos);
		if (prevString != "")
		{
			subStrings.push_back(prevString);
		}
		subStrings.push_back(foundRelevantString.relevantString);
		lastPos = foundRelevantString.index + foundRelevantString.relevantString.size();
	}
	std::string finalString = parseString.substr(lastPos, parseString.size() - lastPos);
	if (finalString != "")
	{
		subStrings.push_back(finalString);
	}

	return subStrings;
}

void HeaderTool::WriteGeneratedCodeFiles(const std::string& directory)
{
	const std::string& fileNameNoExtension = directory + "/" + ImGuiEditorGlobals::generatedFileNames;

	std::ofstream headerFile(fileNameNoExtension + ".h");
	if (headerFile.is_open())
	{
		WriteGlobalHeaderFile(headerFile);
	}
	else
	{
		DOMLOG_ERROR("Cannot create Header");
	}

	std::ofstream cppFile(fileNameNoExtension + ".cpp");
	if (cppFile.is_open())
	{
		WriteGlobalCppFile(cppFile);
	}
	else
	{
		DOMLOG_ERROR("Cannot create CPP");
	}
}

void HeaderTool::WriteGlobalHeaderFile(std::ofstream& file)
{
	file << "#pragma once\n"
		 <<	ImGuiEditorMacros::editorIgnoreFileString << // Flag generated files as "do not parse"
			"\nclass EditorTypePropertyBase;\n"
			"namespace __Generated\n"
			"{\n"
			"	// This works for both structs and classes.\n"
			"	extern std::unordered_map<std::string, void* (*)(const std::vector<EditorTypePropertyBase*>&)> stringToCreateObjectFunction;\n"
			"}\n";
}

void HeaderTool::WriteGlobalCppFile(std::ofstream& file)
{
	// Includes
	file << "#include \"pch.h\"\n"
			"#include \"" << ImGuiEditorGlobals::generatedFileNames << ".h\"\n"
			"#include \"EditorTypePropertyClass.h\"\n"
			"#include \"EditorTypePropertyFloat.h\"\n"
			"#include \"EditorTypePropertyStruct.h\"\n";
	
	
	// Include all relevant code files (since they will be used in InitFromProperties functions
	// #TODO: These will be full filepaths like "C:/Users/Dominic/Desktop/...". They will need to be relative if we plan on using the editor .exe from another PC without compiling (unlikely)
	for (std::string relevantCodeFile : allRelevantCodeFiles)
	{
		file << "#include \"" << relevantCodeFile << "\"\n";
	}
	file << "\n";
	
	// Add all class and struct InitFromProperties() functions
	for (int index = 0; index < allTokens.size(); ++index)
	{
		// #JANK: This can be done with inheritence instead of casting to 2 classes
		std::string name = "";
		if (CodeParseTokenClass* pClassToken = dynamic_cast<CodeParseTokenClass*>(allTokens[index]))
		{
			name = pClassToken->className;
		}
		else if (CodeParseTokenStruct* pStructToken = dynamic_cast<CodeParseTokenStruct*>(allTokens[index]))
		{
			name = pStructToken->structName;
		}
		
		if (name != "")
		{
			file << "void* " << name << "::InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)\n"
					"{\n"
				 << name << "* pReturn = new " << name << ";\n";

			int propertyIndex = 0;
			while (true)
			{
				CodeParseTokenBase* pToken = allTokens[++index];

				if (dynamic_cast<CodeParseTokenScope*>(pToken))
				{
					// Ignore scope tokens, they are allowed to be interspersed within a class.
				}
				else if (CodeParseTokenPropertyBase* pPropertyToken = dynamic_cast<CodeParseTokenPropertyBase*>(pToken))
				{
					std::string propertyTypeCode = "properties[" + std::to_string(propertyIndex++) + "]";
					std::string lValueCode = "pReturn->" + pPropertyToken->propertyName;
					file << pPropertyToken->GenerateSetPropertyCode(lValueCode, propertyTypeCode);
				}
				else
				{
					// If we end up here there's an unexpected token, so we assume the class has ended
					break;
				}
			}

			file << "return pReturn;\n"
					"}\n\n";
		}
	}

	file << "namespace __Generated\n"
			"{\n"
			"	std::unordered_map<std::string, void* (*)(const std::vector<EditorTypePropertyBase*>&)> stringToCreateObjectFunction\n"
			"	{\n";

	// Add all gathered classes to the map of class names to functions
	for (CodeParseTokenBase* token : allTokens)
	{
		std::string name = "";
		if (CodeParseTokenClass* pClassToken = dynamic_cast<CodeParseTokenClass*>(token))
		{
			name = pClassToken->className;
		}
		else if (CodeParseTokenStruct* pStructToken = dynamic_cast<CodeParseTokenStruct*>(token))
		{
			name = pStructToken->structName;
		}
		
		if (name != "")
		{
			// {"MyClass", &MyClass::InitFromProperties},
			file << "		{\"" << name << "\", &" << name << "::InitFromProperties},\n";
		}
	}

	file << "	};\n"
			"}\n";
}

#pragma optimize("", on)