#include "HeaderTool.h"

#include "CodeParseTokenBase.h"
#include "CodeParseTokenDelimiter.h"
#include "CodeParseTokenFactoryClass.h"
#include "CodeParseTokenFactoryClassMetadata.h"
#include "CodeParseTokenFactoryClassMetadataFlagAbstract.h"
#include "CodeParseTokenFactoryClassMetadataFlagInstanced.h"
#include "CodeParseTokenFactoryClassMetadataFlagSingleton.h"
#include "CodeParseTokenFactoryCommentBlock.h"
#include "CodeParseTokenFactoryEnum.h"
#include "CodeParseTokenFactoryEnumValue.h"
#include "CodeParseTokenFactoryProperty.h"
#include "CodeParseTokenFactoryScope.h"
#include "CodeParseTokenFactoryStruct.h"
#include "CodeParseTokenFactoryStructMetadata.h"
#include "CodeParseTokenPropertyBase.h"
#include "CodeParseTokenScope.h"
#include "CodeParseTokenStruct.h"
#include "DomImport/DomImport.h"
#include "HeaderToolUtils.h"
#include "ImGuiEditorGlobals.h"
#include "ImGuiEditorMacros.h"

#include <filesystem>
#include <fstream>
#include <set>
#include <sstream>

#include "CodeParseTokenClassMetadata.h"

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
	CodeParseTokenFactoryCommentBlock codeParseTokenFactoryCommentBlock;
	CodeParseTokenFactoryClass codeParseTokenFactoryClass;
	CodeParseTokenFactoryStruct codeParseTokenFactoryStruct;
	CodeParseTokenFactoryProperty codeParseTokenFactoryProperty;
	CodeParseTokenFactoryEnum codeParseTokenFactoryEnum;
	CodeParseTokenFactoryEnumValue codeParseTokenFactoryEnumValue;
	CodeParseTokenFactoryScope codeParseTokenFactoryScope;
	CodeParseTokenFactoryClassMetadata codeParseTokenFactoryClassMetadata;
	CodeParseTokenFactoryStructMetadata codeParseTokenFactoryStructMetadata;
	CodeParseTokenFactoryClassMetadataFlagAbstract codeParseTokenFactoryClassMetadataFlagAbstract;  
	CodeParseTokenFactoryClassMetadataFlagSingleton codeParseTokenFactoryClassMetadataFlagSingleton;
	CodeParseTokenFactoryClassMetadataFlagInstanced codeParseTokenFactoryClassMetadataFlagInstanced;
	

	// Preceeding scopes (Only allow properties inside classes etc.)
	codeParseTokenFactoryScope.AddRequiredPrecedingScope(&codeParseTokenFactoryClass);
	codeParseTokenFactoryScope.AddRequiredPrecedingScope(&codeParseTokenFactoryStruct);
	codeParseTokenFactoryScope.AddRequiredPrecedingScope(&codeParseTokenFactoryScope);
	codeParseTokenFactoryProperty.AddRequiredPrecedingScope(&codeParseTokenFactoryClass);
	codeParseTokenFactoryProperty.AddRequiredPrecedingScope(&codeParseTokenFactoryStruct);
	codeParseTokenFactoryEnumValue.AddRequiredPrecedingScope(&codeParseTokenFactoryEnum);
	codeParseTokenFactoryClassMetadataFlagAbstract.AddRequiredPrecedingScope(&codeParseTokenFactoryClassMetadata);  
	codeParseTokenFactoryClassMetadataFlagSingleton.AddRequiredPrecedingScope(&codeParseTokenFactoryClassMetadata);
	codeParseTokenFactoryClassMetadataFlagInstanced.AddRequiredPrecedingScope(&codeParseTokenFactoryClassMetadata);

	// Preceeding tokens (Only allow class after class metadata, i.e "class" after "EDITORCLASS(Abstract)"
	codeParseTokenFactoryClass.SetPrecedingTokenType(ECodeParseTokenType::ClassMetadata);
	codeParseTokenFactoryStruct.SetPrecedingTokenType(ECodeParseTokenType::StructMetadata);

	// Blocked scopes (don't allow anything in comment blocks)
	codeParseTokenFactoryClass.AddBlockedByPrecedingScope(&codeParseTokenFactoryCommentBlock);
	codeParseTokenFactoryStruct.AddBlockedByPrecedingScope(&codeParseTokenFactoryCommentBlock);
	codeParseTokenFactoryProperty.AddBlockedByPrecedingScope(&codeParseTokenFactoryCommentBlock);
	codeParseTokenFactoryScope.AddBlockedByPrecedingScope(&codeParseTokenFactoryCommentBlock);
	codeParseTokenFactoryEnum.AddBlockedByPrecedingScope(&codeParseTokenFactoryCommentBlock);
	codeParseTokenFactoryEnumValue.AddBlockedByPrecedingScope(&codeParseTokenFactoryCommentBlock);
	codeParseTokenFactoryClassMetadata.AddBlockedByPrecedingScope(&codeParseTokenFactoryCommentBlock);
	codeParseTokenFactoryStructMetadata.AddBlockedByPrecedingScope(&codeParseTokenFactoryCommentBlock);
	codeParseTokenFactoryClassMetadataFlagAbstract.AddBlockedByPrecedingScope(&codeParseTokenFactoryCommentBlock);
	codeParseTokenFactoryClassMetadataFlagSingleton.AddBlockedByPrecedingScope(&codeParseTokenFactoryCommentBlock);
	codeParseTokenFactoryClassMetadataFlagInstanced.AddBlockedByPrecedingScope(&codeParseTokenFactoryCommentBlock);

	CodeParseTokenFactoryBase* pFactories[] =
	{
		&codeParseTokenFactoryCommentBlock,
		&codeParseTokenFactoryClass,
		&codeParseTokenFactoryStruct,
		&codeParseTokenFactoryProperty,
		&codeParseTokenFactoryEnum,
		&codeParseTokenFactoryEnumValue,
		&codeParseTokenFactoryScope,
		&codeParseTokenFactoryClassMetadata,
		&codeParseTokenFactoryStructMetadata,
		&codeParseTokenFactoryClassMetadataFlagAbstract,
		&codeParseTokenFactoryClassMetadataFlagSingleton,
		&codeParseTokenFactoryClassMetadataFlagInstanced,
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
		for (const std::string& substring : subStrings)
		{
			// Check for file requesting to be ignored
			if (substring == ImGuiEditorMacros::editorIgnoreFileString)
			{
				DOMLOG_ERROR_IF(bCodeFileContainsEditorData, "Ignoring a file that is mid way through being parsed")
				return;
			}

			// If we find a comment flush the rest of the line
			if (substring == "//")
			{
				// #TODO: Is it better to have comments as a CokeToken? We could then export them to the EditorTypes to allow in-editor tooltips 
				std::getline(stream, nextString);
				break;
			}
			
			// Check for end of scope
			CodeParseTokenFactoryBase* pOuterScopedFactory = activeScopedFactories.empty() ? nullptr : activeScopedFactories.top();
			if (pOuterScopedFactory)
			{
				std::string endString = pOuterScopedFactory->GetEndString();

				if (substring == endString)
				{
					activeScopedFactories.pop();

					if (pOuterScopedFactory->ShouldAddDelimiterAfter())
					{
						allTokens.push_back(new CodeParseTokenDelimiter);
					}

					continue;
				}
			}

			// Check factories
			for (CodeParseTokenFactoryBase* pFactory : pFactories)
			{
				ECodeParseTokenType priorTokenType = allTokens.size() > 0 ? allTokens.back()->GetCodeParseTokenType() : ECodeParseTokenType::UNUSED;
				if (pFactory->IsKeyword(substring) && pFactory->CanFactoryBeUsed(activeScopedFactories, priorTokenType))
				{
					if (pFactory->CanCreateToken())
					{
						
						if (CodeParseTokenBase* pToken = pFactory->CreateToken(substring, stream, pOuterScopedFactory))
						{
							allTokens.push_back(pToken);
							bCodeFileContainsEditorData = true;
						}
						else
						{
							DOMLOG_ERROR("Cannot create token for keyword:", substring, "in file", codeFile);
						}
					}
					
					if (pFactory->IsScopedTokenType())
					{
						activeScopedFactories.push(pFactory);
					}

					break;
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
	// STEP 1 -- Sort by dependencies
	{
		std::vector<CodeParseTokenBase*> sortedTokens;
		sortedTokens.reserve(allTokens.size());
	
		std::set<std::string> satisfiedDependencies; // Ever growing vector of satisfied dependencies
		
		// Start off with default types as a given
		for (std::string& basicType : HeaderToolUtils::GetBasicTypes())
		{
			satisfiedDependencies.emplace(basicType);
		}

		// While tokens remain...
		while (!allTokens.empty())
		{
			int numTokensBeforeLoop = (int)allTokens.size();
			int tokenIndex = numTokensBeforeLoop - 2; // Last token will be delimiter, so skip it
			int lastSegmentIndex = numTokensBeforeLoop - 1; 
			std::set<std::string> allRequiredDependencies;

			while (tokenIndex >= -1) // use "-1" so we can tell if we've reached the end (vs hitting a delimiter)
			{
				// Iterate all tokens backwards, finding segments (between delimiters and EOF) that can be added.
			
				CodeParseTokenBase* pToken = tokenIndex >= 0 ? allTokens[tokenIndex] : nullptr;
				if (tokenIndex == -1 || pToken->GetCodeParseTokenType() == ECodeParseTokenType::Delimiter)
				{
					// Delimiter found, iterate segment
					bool bDependenciesSatisfied = true;
					for (const std::string& requiredDependency : allRequiredDependencies)
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

						// Within each block sort further by ECodeParseTokenPriority 
						for (int priority = (int)ECodeParseTokenPriority::COUNT - 1; priority >= 0; --priority)
						{
							for (int segmentIndex = tokenIndex + 1; segmentIndex <= lastSegmentIndex; ++segmentIndex)
							{
								if ((int)allTokens[segmentIndex]->GetPriorityWithinBlock() != priority)
								{
									continue;
								}
								
								sortedTokens.push_back(allTokens[segmentIndex]);

								std::string requiredDependency = allTokens[segmentIndex]->GetSatisfiedDependency();
								if (requiredDependency != "")
								{
									// Mark satisfied dependencies as such
									satisfiedDependencies.emplace(requiredDependency);
								}
							}
						}

						// Remove the segment from the unsorted array
						allTokens.erase(allTokens.begin() +  tokenIndex + 1, allTokens.begin() + lastSegmentIndex + 1);
					}

					lastSegmentIndex = tokenIndex;
					--tokenIndex;
					allRequiredDependencies.erase(allRequiredDependencies.begin(), allRequiredDependencies.end());
				}
				else
				{
					// No delimiter, continue along section
					std::vector<std::string> requiredDependencies = pToken->GetRequiredDependencies();
					for(const std::string& requiredDependency : requiredDependencies)
					{
						allRequiredDependencies.emplace(requiredDependency);
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
		allTokens = std::move(sortedTokens);
	}
}

std::vector<std::string> HeaderTool::BreakDownParseString(const std::string& parseString) const
{
	// #NOTE: If we ever add "(" or ")" we need to make sure they don't collide with EDITORCLASS() etc.
	// #TODO: Might be worth doing this as a pre-pass rather than as we go. Right now these relevant strings aren't extracted when doing CodeParseTokenFactoryBase::CreateToken()
	static const std::string relevantStringsToExtract[] =
	{
		"//",
		"/*",
		"*/",
		"{",
		"}",
		",",
		"=",
		ImGuiEditorMacros::editorClassString,
		ImGuiEditorMacros::editorStructString,
		ImGuiEditorMacros::editorEnumString,
		ImGuiEditorMacros::editorPropertyString,
		ImGuiEditorMacros::editorIgnoreFileString,
		"Abstract",
		"Instanced",
		"Singleton",
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
			"#include \"EditorTypePropertyInt.h\"\n"
			"#include \"EditorTypePropertyBool.h\"\n"
			"#include \"EditorTypePropertyString.h\"\n"
			"#include \"EditorTypePropertyStruct.h\"\n"
			"#include \"EditorTypePropertyVector.h\"\n"
			"#include \"EditorTypePropertyEnum.h\"\n"
			"#include \"EditorTypePropertyInstancedAssetPtr.h\"\n";
	

	
	
	// Include all relevant code files (since they will be used in InitFromProperties functions
	// #TODO: These will be full filepaths like "C:/Users/Dominic/Desktop/...". They will need to be relative if we plan on using the editor .exe from another PC without compiling (unlikely)
	for (std::string relevantCodeFile : allRelevantCodeFiles)
	{
		file << "#include \"" << relevantCodeFile << "\"\n";
	}
	file << "\n";

	file << "#pragma warning( disable : 4189 )"; // Suppress "local variable is initialized but not referenced". Necessary else EDITORCLASSES with no properties cause compile errors
	
	file << "\n";
	file << "\n";
	
	// Add all class and struct InitFromProperties() and InitFromPropertiesSubset() functions
	for (int index = 0; index < allTokens.size(); ++index)
	{
		if (CodeParseTokenUDT* pUDTToken = dynamic_cast<CodeParseTokenUDT*>(allTokens[index]))
		{
			std::string name = pUDTToken->udtName;

			// InitFromPropertiesSubset()
			{
				file <<	"// " << name << "\n"
						"void " << name << "::InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)\n"
						"{\n" 
						"\t" << name << "* p" << name << " = static_cast<" << name << "*>(pObject);\n";
			
				// Call base class init functions
				for (std::string baseName : pUDTToken->baseUdtNames)
				{
					file << "\t" << baseName << "::InitFromPropertiesSubset(static_cast<" << baseName << "*>(p" << name << "), properties, propertyIndex);\n"; 
				}

				// Set properties of this class
				while (true)
				{
					CodeParseTokenBase* pToken = allTokens[++index];

					if (dynamic_cast<CodeParseTokenScope*>(pToken) || pToken->GetCodeParseTokenType() == ECodeParseTokenType::ClassMetadata || pToken->GetCodeParseTokenType() == ECodeParseTokenType::StructMetadata)
					{
						// Ignore scope tokens, they are allowed to be interspersed within a class.
					}
					else if (CodeParseTokenPropertyBase* pPropertyToken = dynamic_cast<CodeParseTokenPropertyBase*>(pToken))
					{
						std::string propertyTypeCode = "properties[propertyIndex++]";
						std::string lValueCode = "p" + name + "->" + pPropertyToken->propertyName;
						file << "\t" << pPropertyToken->GenerateSetPropertyCode(lValueCode, propertyTypeCode, *this);
					}
					else
					{
						// If we end up here there's an unexpected token, so we assume the class has ended
						break;
					}
				}
			}

			// InitFromProperties()
			file << "}\n\n"
					"void* " << name << "::InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)\n"
					"{\n" 
					"\t" << name << "* p" << name << " = new " << name << ";\n"
					"\t" << "int propertyIndex = 0;\n"
					"\t" << name << "::InitFromPropertiesSubset(p" << name << ", properties, propertyIndex);\n"
					"\treturn p" << name << ";\n"
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
		if (CodeParseTokenUDT* pUDTToken = dynamic_cast<CodeParseTokenUDT*>(token))
		{
			std::string name = pUDTToken->udtName;
			
			// {"MyClass", &MyClass::InitFromProperties},
			file << "		{\"" << name << "\", &" << name << "::InitFromProperties},\n";
		}
	}

	file << "	};\n"
			"}\n";
}
