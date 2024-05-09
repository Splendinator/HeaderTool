#include "CodeParseTokenFactoryBase.h"

bool CodeParseTokenFactoryBase::CanFactoryBeUsed(std::stack<CodeParseTokenFactoryBase*> currentStack) const
{
	// Check that the token directly above is there if required
	if (!pPrecedingTokenTypes.empty())
	{
		if (currentStack.empty())
		{
			return false;
		}
		if (std::find(pPrecedingTokenTypes.begin(), pPrecedingTokenTypes.end(), currentStack.top()) == pPrecedingTokenTypes.end())
		{
			return false;
		}
	}

	// Check that the token directly above isn't blocking us
	if (!currentStack.empty())
	{
		for (CodeParseTokenFactoryBase* pBlockedByTokenType : pBlockedByTokenTypes)
		{
			if (currentStack.top() == pBlockedByTokenType)
			{
				return false;
			}
		}
	}
	
	return true;
}
