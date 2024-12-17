#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <boost/json.hpp>

boost::json::value getConfig()
{
    std::ifstream input("config.json");
    if (!input.is_open())
    {
        throw std::runtime_error("Unable to open config.json");
    }

    std::stringstream buffer;
    buffer << input.rdbuf();
    std::string content = buffer.str();
    return boost::json::parse(content);
}

std::unordered_map<std::string, std::string> getConfigMap()
{
    boost::json::value json_data = getConfig();
    std::unordered_map<std::string, std::string> config;
    for (const auto &[key, value] : json_data.as_object())
    {
        if (value.is_string())
        {
            config[key.data()] = value.as_string().c_str();
        }
        else
        {
            config[key.data()] = boost::json::serialize(value);
        }
    }
    return config;
}

#endif