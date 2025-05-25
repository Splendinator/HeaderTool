#include "CodeParseTokenDelimiter.h"

#include "Editor/ImGuiEditorGlobals.h"

void CodeParseTokenDelimiter::WriteToFile(std::ofstream& outputFile)
{
	outputFile << ImGuiEditorGlobals::delimiterString << std::endl;
}
