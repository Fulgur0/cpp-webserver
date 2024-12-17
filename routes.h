#include <string>
#include <fstream>
#include <iostream>
#include "config.h"

std::string openFile(const std::string &path)
{
    boost::json::value json_data = getConfig();
    std::ifstream file("public/" + path);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    for (const auto &[key, value] : json_data.as_object())
    {
        std::string placeholder = "ENV_" + std::string(key.data(), key.size());
        size_t pos = 0;
        while ((pos = content.find(placeholder, pos)) != std::string::npos)
        {
            content.replace(pos, placeholder.length(), value.as_string().c_str());
            pos += value.as_string().size();
        }
    }

    return content;
}

std::string renderView(const std::string &path, const std::string &request)
{
    boost::json::value json_data = getConfig();
    std::vector<std::string> headers;
    std::istringstream requestStream(request);
    std::string line;
    while (std::getline(requestStream, line))
    {
        headers.push_back(line);
    }

    if (path == "/")
    {
        return openFile("index.html");
    }
    else
    {
        return openFile("404.html");
    }
}