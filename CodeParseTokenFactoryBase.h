#pragma once

// Just dumping all the headers in here as I CBA to include everywhere in this self-contained .exe
#include <string>
#include <stack>
#include <vector>
#include <sstream>

#include "DomLog/DomLog.h"

class CodeParseTokenBase;
class HeaderTool;

/** CodeParseTokenFactoryBase
*
* This class is used for reading in code files. It is used to create tokens like "Class" or "Scope" from the code file.
* See CodeParseTokenBase for more information on tokens.
*/
class CodeParseTokenFactoryBase
{
public:

	// Should be overridden to return whether the keyword is a match for the token type. (i.e "class" or "enum")
	virtual bool IsKeyword(const std::string& keyword) = 0;

	// Override to populate the token with metadata. The input ifstream will be start after the keyword. 
	// Used to create and populate a token. can return nullptr if there was an error creating a token to log an error.
	// pOuterScopedFactory is useful if inner things need a scope context (e.g. enum values need to increase by 1 each time)
	virtual CodeParseTokenBase* CreateToken(const std::string& keyword, std::stringstream& stream, CodeParseTokenFactoryBase* pOuterScopedFactory) { return nullptr; }

	// Optionally override to define an "end string". Token types with this are considered "scoped" tokens and will remain valid and on the stack until the end string is found.
	// Used for things like enums and classes.
	virtual std::string GetEndString() const { return ""; }

	// Whether this factory actually producted a token (some are just used to block others via AddBlockedByPrecedingToken() etc.)
	virtual bool CanCreateToken() const { return true; }

	// Whether this scoped class should add a delimiter in the EditorTypes file after it pops off
	virtual bool ShouldAddDelimiterAfter() const { return false; }
	
	// See pPrecedingTokenTypes
	void AddRequiredPrecedingToken(CodeParseTokenFactoryBase* pInPrecedingTokenType) { pPrecedingTokenTypes.push_back(pInPrecedingTokenType); }

	// See pBlockedByTokenTypes
	void AddBlockedByPrecedingToken(CodeParseTokenFactoryBase* pBlockTokenType) { pBlockedByTokenTypes.push_back(pBlockTokenType); }

	// Returns whether this token type can be created based on the current stack. For example, only if the previous token was a class token, then a property token can be created.
	bool CanFactoryBeUsed(std::stack<CodeParseTokenFactoryBase*> currentStack) const;

	// Returns whether this is a "scoped" token type and should be added to the stack. See GetEndString()
	bool IsScopedTokenType() const { return GetEndString() != ""; }

private: 

	// Block any of these token factories while this is scoped 
	std::vector<CodeParseTokenFactoryBase*> pBlockedByTokenTypes;
	
	// Token that must be on the top of the stack before this token can be parsed. (i.e "enum" must be on the stack before "enum value" can be parsed)
	std::vector<CodeParseTokenFactoryBase*> pPrecedingTokenTypes;
};