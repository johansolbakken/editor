#pragma once

#include <string>
#include <vector>
#include <map>
#include <filesystem>

class Search
{
public:
    Search(const std::string& directory);
    void index_subdirectory(const std::filesystem::path& path);
    std::vector<std::string> match_path(const std::string& query);

private:
    std::string m_directory;

    std::map<std::string, std::vector<std::string>> m_index;
    std::vector<std::string> m_paths;
};