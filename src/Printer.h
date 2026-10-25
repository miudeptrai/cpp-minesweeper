#pragma once

#include <string>
#include <stack>
#include <unordered_set>

#include "Point.h"
#include "Input.h"

class Printer
{
private:
	static const std::unordered_set<std::string> s_FUNCTIONS;

	static std::stack<std::string> s_Texts;

	static size_t GetLine();
	static std::string ToLower(std::string text);
public:
	static constexpr const char* s_DIG_STR = "dig";
	static constexpr const char* s_FLAG_STR = "flag";

	static void Print(size_t count, const std::string& text);
	static void CursorUpBy(size_t count);
	static Point GetPoint();
	static Input ParseInput();
	static Point PromptPoint(const std::string& prompt); // These Prompt funcs make sure the retun val will be valid
	static Input PromptInput(const std::string& prompt);
};

