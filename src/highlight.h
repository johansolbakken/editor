#pragma once

#include "raylib.h"

#include <string>

struct Token
{
	std::string text;
	Color color;
};

std::vector<std::vector<Token>> get_colors(const std::vector<std::string>& linesVector);