#include "highlight.h"

#include <map>
#include <string>
#include <vector>
#include <fstream>

#include <clang-c/Index.h>

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

// use libclang to get tokens from a file and make a vector for each line with a vector of tokens. Spaces must be preserved.
std::vector<std::vector<Token>> get_colors(const std::vector<std::string>& linesVector) {
	std::vector<std::vector<Token>> lines;

	// Create a temp file
	std::ofstream file("temp.cpp");
	for (const auto& line : linesVector) {
		file << line << '\n';
	}
	file.close();

	// Index the text
	CXIndex index = clang_createIndex(0, 0);
	CXTranslationUnit unit = clang_parseTranslationUnit(index, "temp.cpp", nullptr, 0, nullptr, 0, CXTranslationUnit_None);

	// Tokenize
	CXToken* tokens;
	unsigned int numTokens;
	CXSourceRange range = clang_getCursorExtent(clang_getTranslationUnitCursor(unit));
	clang_tokenize(unit, range, &tokens, &numTokens);

	int tokenIdx = 0;
	for (const std::string& currentLine : linesVector) {
		std::vector<Token> lineTokens;
		int position = 0;

		while (position < currentLine.size()) {
			if (tokenIdx < numTokens) {
				CXToken token = tokens[tokenIdx];
				CXString tokenText = clang_getTokenSpelling(unit, token);
				const char* tokenCString = clang_getCString(tokenText);

				if (currentLine.compare(position, strlen(tokenCString), tokenCString) == 0) {
					// We found a token match in the line
					Color color = color_scheme[IDENTIFIER];
					switch (clang_getTokenKind(token)) {
					case CXToken_Keyword:
						color = color_scheme[KEYWORD];
						break;
					case CXToken_Punctuation:
						color = color_scheme[OPERATOR];
						break;
					case CXToken_Literal:
						color = color_scheme[LITERAL];
						break;
					case CXToken_Comment:
						color = color_scheme[COMMENT];
						break;
					case CXToken_Identifier:
						color = color_scheme[IDENTIFIER];
						break;
					default:
						break;
					}
					lineTokens.push_back({tokenCString, color});
					position += strlen(tokenCString);
					clang_disposeString(tokenText);
					tokenIdx++;
					continue;
				}
				clang_disposeString(tokenText); // Dispose token if not used.
			}

			if (currentLine[position] == ' ') {
				lineTokens.push_back(Token{ " ", color_scheme[UNKNOWN] });
			} else {
				// Handle individual non-space, non-token characters
				lineTokens.push_back(Token{std::string(1, currentLine[position]), color_scheme[UNKNOWN]});
			}
			position++;
		}

		lines.push_back(lineTokens);
	}

	clang_disposeTokens(unit, tokens, numTokens);
	clang_disposeTranslationUnit(unit);
	clang_disposeIndex(index);

	// Remove the temporary file
	remove("temp.cpp");

	return lines;
}


