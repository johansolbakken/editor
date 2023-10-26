#include "search.h"

#include <iostream>
#include <filesystem>

Search::Search(const std::string& directory)
    : m_directory(directory)
{
    std::filesystem::path path = std::filesystem::absolute(directory);
    index_subdirectory(path);
}

void Search::index_subdirectory(const std::filesystem::path& path)
{
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_directory()) {
            index_subdirectory(entry.path());
        } else {
            m_paths.push_back(entry.path().string());
        }
    }
}

std::vector<std::string> Search::match_path(const std::string& query)
{
    std::vector<std::string> matches;
    for (const auto& path : m_paths) {
        if (path.contains(query)) {
            matches.push_back(path);
        }
    }
    return matches;
}