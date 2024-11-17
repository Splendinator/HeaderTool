#include "CodeParseTokenFactoryBase.h"

bool CodeParseTokenFactoryBase::CanFactoryBeUsed(std::stack<CodeParseTokenFactoryBase*> currentStack, ECodeParseTokenType inPreceedingTokenType) const
{
	// Check that the token directly prior is correct
	if (preceedingTokenType != ECodeParseTokenType::UNUSED && preceedingTokenType != inPreceedingTokenType)
	{
		return false;
	}
	
	// Check that the token directly above is there if required
	if (!pPrecedingScopes.empty())
	{
		if (currentStack.empty())
		{
			return false;
		}
		if (std::find(pPrecedingScopes.begin(), pPrecedingScopes.end(), currentStack.top()) == pPrecedingScopes.end())
		{
			return false;
		}
	}

	// Check that the token directly above isn't blocking us
	if (!currentStack.empty())
	{
		for (CodeParseTokenFactoryBase* pBlockedByTokenType : pBlockedByScopes)
		{
			if (currentStack.top() == pBlockedByTokenType)
			{
				return false;
			}
		}
	}
	
	return true;
}
