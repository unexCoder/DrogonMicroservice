#pragma once

#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <drogon/drogon.h>

/**
 * @brief Loads environment variables from a .env file
 * 
 * This utility function reads a .env file and parses environment variables
 * in the format: KEY=VALUE
 * 
 * Features:
 * - Ignores empty lines and comments (lines starting with #)
 * - Handles KEY=VALUE format
 * - Returns a map of key-value pairs
 * - Logs warning if file cannot be opened
 * 
 * @param filename Path to the .env file
 * @return std::map<std::string, std::string> Parsed environment variables
 * 
 * @example
 * auto envVars = loadEnvFile(".env");
 * std::string dbHost = envVars["DB_HOST"];
 * std::string dbPassword = envVars["DB_PASSWORD"];
 */
std::map<std::string, std::string> loadEnvFile(const std::string& filename) {
    std::map<std::string, std::string> env;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        LOG_WARN << "Cannot load " << filename << ", using system environment";
        return env;
    }
    
    std::string line;
    int lineNum = 0;
    
    while (std::getline(file, line)) {
        lineNum++;
        
        // Skip empty lines
        if (line.empty()) continue;
        
        // Skip comment lines
        if (line[0] == '#') continue;
        
        // Trim leading whitespace
        size_t start = line.find_first_not_of(" \t");
        if (start == std::string::npos) continue;
        
        // Skip if still a comment after trimming
        if (line[start] == '#') continue;
        
        // Find the '=' separator
        size_t pos = line.find('=');
        if (pos == std::string::npos) {
            LOG_WARN << "Invalid format in " << filename << " at line " << lineNum 
                     << ": missing '=' separator";
            continue;
        }
        
        std::string key = line.substr(start, pos - start);
        std::string value = line.substr(pos + 1);
        
        // Trim trailing whitespace from key
        size_t keyEnd = key.find_last_not_of(" \t");
        if (keyEnd != std::string::npos) {
            key = key.substr(0, keyEnd + 1);
        }
        
        // Trim leading/trailing whitespace from value
        size_t valueStart = value.find_first_not_of(" \t\"");
        size_t valueEnd = value.find_last_not_of(" \t\"");
        if (valueStart != std::string::npos) {
            value = value.substr(valueStart, valueEnd - valueStart + 1);
        }
        
        if (!key.empty()) {
            env[key] = value;
            LOG_DEBUG << "Loaded env variable: " << key;
        }
    }
    
    LOG_INFO << "Loaded " << env.size() << " environment variables from " << filename;
    return env;
}

/**
 * @brief Sets environment variables from a map
 * 
 * Converts a map of environment variables to actual system environment variables
 * using setenv()
 * 
 * @param vars Map of key-value pairs
 * @return true if all variables set successfully, false otherwise
 */
bool setEnvironmentVariables(const std::map<std::string, std::string>& vars) {
    for (const auto& [key, value] : vars) {
        if (setenv(key.c_str(), value.c_str(), 1) != 0) {
            LOG_ERROR << "Failed to set environment variable: " << key;
            return false;
        }
    }
    return true;
}

/**
 * @brief Gets an environment variable with optional default value
 * 
 * @param key The environment variable name
 * @param defaultValue Optional default value if variable not found
 * @return std::string The environment variable value or default
 */
std::string getEnvVariable(const std::string& key, const std::string& defaultValue = "") {
    const char* value = std::getenv(key.c_str());
    if (!value) {
        if (defaultValue.empty()) {
            LOG_WARN << "Environment variable not found: " << key;
        }
        return defaultValue;
    }
    return std::string(value);
}

/**
 * @brief Validates that all required environment variables are set
 * 
 * @param requiredVars List of required environment variable names
 * @return true if all variables are set, false otherwise
 */
bool validateRequiredEnvVariables(const std::vector<std::string>& requiredVars) {
    bool allPresent = true;
    for (const auto& var : requiredVars) {
        if (!std::getenv(var.c_str())) {
            LOG_ERROR << "Missing required environment variable: " << var;
            allPresent = false;
        }
    }
    return allPresent;
}
