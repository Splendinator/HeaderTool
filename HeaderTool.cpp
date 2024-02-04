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
#include <sstream>

#include "CodeParseTokenPropertyBase.h"

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
	CodeParseTokenFactoryProperty codeParseTokenFactoryProperty;
	CodeParseTokenFactoryScope codeParseTokenFactoryScope;

	codeParseTokenFactoryScope.AddRequiredPrecedingToken(&codeParseTokenFactoryClass);
	codeParseTokenFactoryScope.AddRequiredPrecedingToken(&codeParseTokenFactoryScope);
	codeParseTokenFactoryProperty.AddRequiredPrecedingToken(&codeParseTokenFactoryClass);

	CodeParseTokenFactoryBase* pFactories[] =
	{
		&codeParseTokenFactoryClass,
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

std::vector<std::string> HeaderTool::BreakDownParseString(const std::string& parseString) const
{
	// #TODO: If we ever add "(" or ")" we need to make sure they don't collide with EDITORCLASS() etc.
	static const std::string relevantStringsToExtract[] =
	{
		"{",
		"}",
		ImGuiEditorMacros::editorClassString,
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
			"\nclass EditorAssetBase;\n"
			"namespace __Generated\n"
			"{\n"
			"	extern std::unordered_map<std::string, void* (*)(EditorAssetBase*)> stringToCreateClassFunction;\n"
			"}\n";
}

void HeaderTool::WriteGlobalCppFile(std::ofstream& file)
{
	// Includes
	file << "#include \"pch.h\"\n"
			"#include \"" << ImGuiEditorGlobals::generatedFileNames << ".h\"\n"
			"#include \"EditorAssetClass.h\"\n"
			"#include \"EditorTypePropertyClass.h\"\n"
			"#include \"EditorTypePropertyFloat.h\"\n";
	
	
	// Include all relevant code files (since they will be used in InitFromAsset functions
	// #TODO: These will be full filepaths like "C:/Users/Dominic/Desktop/...". They will need to be relative if we plan on using the editor .exe from another PC without compiling (unlikely)
	for (std::string relevantCodeFile : allRelevantCodeFiles)
	{
		file << "#include \"" << relevantCodeFile << "\"\n";
	}
	file << "\n";
	
	// Add all class InitFromAsset() functions
	for (int index = 0; index < allTokens.size(); ++index)
	{
		if (CodeParseTokenClass* pClassToken = dynamic_cast<CodeParseTokenClass*>(allTokens[index]))
		{
			file << "void* " << pClassToken->className << "::InitFromAsset(EditorAssetBase* pAsset)\n"
					"{\n"
				 << pClassToken->className << "* pReturn = new " << pClassToken->className << ";\n"
					"EditorAssetClass* pEditorAssetClass = static_cast<EditorAssetClass*>(pAsset);\n"
					"const std::vector<EditorTypePropertyBase*>& properties = pEditorAssetClass->GetProperties();\n";

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
					file << "pReturn->" << pPropertyToken->propertyName << " = " << pPropertyToken->GeneratedCodePropertySetString(propertyTypeCode);
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
			"	std::unordered_map<std::string, void* (*)(EditorAssetBase*)> stringToCreateClassFunction\n"
			"	{\n";

	// Add all gathered classes to the map of class names to functions
	for (CodeParseTokenBase* token : allTokens)
	{
		if (CodeParseTokenClass* pClassToken = dynamic_cast<CodeParseTokenClass*>(token))
		{
			// {"MyClass", &MyClass::InitFromAsset},
			file << "		{\"" << pClassToken->className << "\", &" << pClassToken->className << "::InitFromAsset},\n";
		}
	}

	file << "	};\n"
			"}\n";
}
