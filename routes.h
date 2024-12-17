#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include "config.h"

std::string openFile(const std::string &path, const std::unordered_map<std::string, std::string> &config)
{
    std::ifstream file("public/" + path);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + path);
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    for (const auto &[key, value] : config)
    {
        std::string placeholder = "ENV_" + key;
        size_t pos = 0;
        while ((pos = content.find(placeholder, pos)) != std::string::npos)
        {
            content.replace(pos, placeholder.length(), value);
            pos += value.size();
        }
    }

    return content;
}

std::string renderView(const std::string &path, const std::string &request, const std::unordered_map<std::string, std::string> &config)
{
    std::vector<std::string> headers;
    std::istringstream requestStream(request);
    std::string line;
    while (std::getline(requestStream, line))
    {
        headers.push_back(line);
    }

    return openFile(path == "/" ? "index.html" : "404.html", config);
}