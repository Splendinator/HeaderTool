#include "CodeParseTokenDelimiter.h"

#include "ImGuiEditorGlobals.h"

void CodeParseTokenDelimiter::WriteToFile(std::ofstream& outputFile)
{
	outputFile << ImGuiEditorGlobals::delimiterString << std::endl;
}
