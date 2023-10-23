#include "highlight.h"

#include <map>
#include <regex>

// Token patterns
std::regex keywordPattern(R"(\b(int|float|void|if|else|return|while|for|switch|case)\b)");
std::regex operatorPattern(R"([\+\-\*\/=])");
std::regex literalPattern(R"(\".*?\"|\d+(\.\d+)?)");
std::regex commentPattern(R"(\/\/.*|\/\*.*?\*\/)");
std::regex preprocessorPattern(R"(^\s*#\w+)");
std::regex identifierPattern(R"(\b[_a-zA-Z][_a-zA-Z0-9]*\b)");

// Color definitions
enum TokenType {
	KEYWORD, OPERATOR, LITERAL, COMMENT, PREPROCESSOR, IDENTIFIER, UNKNOWN
};

std::map<TokenType, Color> color_scheme = {
		{KEYWORD,      Color{86, 156, 214, 255}},
		{OPERATOR,     Color{220, 220, 170, 255}},
		{LITERAL,      Color{209, 105, 105, 255}},
		{COMMENT,      Color{0, 128, 0, 255}},
		{PREPROCESSOR, Color{128, 0, 128, 255}},
		{IDENTIFIER,   Color{255, 255, 255, 255}},
		{UNKNOWN,      Color{255, 255, 255, 255}}
};

Color get_color_for_token(const std::string& token)
{
	if (std::regex_match(token, keywordPattern)) {
		return color_scheme[KEYWORD];
	}
	else if (std::regex_match(token, operatorPattern)) {
		return color_scheme[OPERATOR];
	}
	else if (std::regex_match(token, literalPattern)) {
		return color_scheme[LITERAL];
	}
	else if (std::regex_match(token, commentPattern)) {
		return color_scheme[COMMENT];
	}
	else if (std::regex_match(token, preprocessorPattern)) {
		return color_scheme[PREPROCESSOR];
	}
	else if (std::regex_match(token, identifierPattern)) {
		return color_scheme[IDENTIFIER];
	}
	else {
		return color_scheme[UNKNOWN];
	}
}
