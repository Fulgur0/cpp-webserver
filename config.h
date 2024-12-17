#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <fstream>
#include <sstream>
#include <boost/json/src.hpp>

boost::json::value getConfig()
{
    std::ifstream input("config.json");
    std::stringstream buffer;

    buffer << input.rdbuf();
    std::string content = buffer.str();
    return boost::json::parse(content);
}

#endif