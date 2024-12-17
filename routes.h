#include <string>
#include <fstream>
#include <iostream>

std::string openFile(const std::string &path)
{
    std::ifstream file("public/" + path);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
}

std::string renderView(const std::string &path)
{
    if (path == "/")
    {
        return openFile("index.html");
    }
    else
    {
        return openFile("404.html");
    }
}