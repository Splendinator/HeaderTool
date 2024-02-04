#include "CodeParseTokenFactoryBase.h"

bool CodeParseTokenFactoryBase::CanFactoryBeUsed(std::stack<CodeParseTokenFactoryBase*> currentStack) const
{
	if (currentStack.empty())
	{
		return pPrecedingTokenTypes.empty();
	}
	else
	{
		return std::find(pPrecedingTokenTypes.begin(), pPrecedingTokenTypes.end(), currentStack.top()) != pPrecedingTokenTypes.end();
	}
}
