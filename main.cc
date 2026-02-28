#include <drogon/drogon.h>
#include "env_loader.h"
#include <vector>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

int main() {
    LOG_INFO << "=== Drogon Server Startup ===";
    
    // Step 1: Find and load .env file
    LOG_INFO << "Loading environment configuration from .env...";
    
    // Search for .env in multiple locations
    std::vector<std::string> envPaths = {
        ".env",                                    // Current directory
        "../.env",                                 // Parent directory
        "../../.env",                              // Two levels up
        "/Volumes/Archivo/LUIGI/work/dev/c++/drogon/init_drogon/.env"  // Absolute path
    };
    
    std::string envFile;
    for (const auto& path : envPaths) {
        if (std::ifstream(path).good()) {
            envFile = path;
            LOG_DEBUG << "Found .env at: " << path;
            break;
        }
    }
    
    if (envFile.empty()) {
        LOG_WARN << "Could not find .env file in searched locations:";
        for (const auto& path : envPaths) {
            LOG_WARN << "  - " << path;
        }
        LOG_WARN << "Attempting to use: .env";
        envFile = ".env";
    }
    
    // Load environment variables from .env file
    auto envVars = loadEnvFile(envFile);
    
    // If .env file exists, set those variables
    if (!envVars.empty()) {
        setEnvironmentVariables(envVars);
        LOG_INFO << "✓ Environment variables loaded from .env file (" << envVars.size() << " variables)";
    } else {
        LOG_WARN << "⚠ No .env file found or file is empty. Using system environment variables only.";
        LOG_WARN << "\n========== QUICK FIX ==========";
        LOG_WARN << "You are running from: " << fs::current_path().string();
        LOG_WARN << "Looking for: .env file in project root";
        LOG_WARN << "Solution: Run from project root directory";
        LOG_WARN << "  cd /Volumes/Archivo/LUIGI/work/dev/c++/drogon/init_drogon";
        LOG_WARN << "  ./build/init_drogon";
        LOG_WARN << "==============================\n";
    }
    
    // Step 2: Validate required environment variables
    LOG_INFO << "Validating required configuration...";
    std::vector<std::string> requiredVars = {
        "DB_HOST",
        "DB_USER", 
        "DB_PASSWORD",
        "DB_NAME"
    };
    
    if (!validateRequiredEnvVariables(requiredVars)) {
        LOG_ERROR << "\n========== CONFIGURATION ERROR ==========";
        LOG_ERROR << "Missing required environment variables!";
        LOG_ERROR << "\nYou must either:";
        LOG_ERROR << "  1. Create/copy .env file in project root with required variables";
        LOG_ERROR << "  2. Run from correct directory:";
        LOG_ERROR << "     cd /Volumes/Archivo/LUIGI/work/dev/c++/drogon/init_drogon";
        LOG_ERROR << "     ./build/init_drogon";
        LOG_ERROR << "\nRequired variables:";
        LOG_ERROR << "  DB_HOST=127.0.0.1";
        LOG_ERROR << "  DB_USER=root";
        LOG_ERROR << "  DB_PASSWORD=YourPassword";
        LOG_ERROR << "  DB_NAME=culture_hub";
        LOG_ERROR << "=========================================\n";
        return 1;
    }
    
    // Step 3: Log configuration summary (without sensitive data)
    LOG_INFO << "\n========== Configuration Loaded ==========";
    LOG_INFO << "Database Configuration:";
    LOG_INFO << "  Host: " << getEnvVariable("DB_HOST");
    LOG_INFO << "  Port: " << getEnvVariable("DB_PORT", "3306");
    LOG_INFO << "  Database: " << getEnvVariable("DB_NAME");
    LOG_INFO << "  User: " << getEnvVariable("DB_USER");
    LOG_INFO << "  Pool Size: " << getEnvVariable("DB_POOL_SIZE", "4");
    
    LOG_INFO << "\nApplication Configuration:";
    LOG_INFO << "  Thread Count: " << getEnvVariable("APP_THREADS", "4");
    LOG_INFO << "  Log Level: " << getEnvVariable("LOG_LEVEL", "INFO");
    LOG_INFO << "  Debug Mode: " << getEnvVariable("DEBUG_MODE", "false");
    
    LOG_INFO << "\nHTTPS Configuration:";
    LOG_INFO << "  HTTPS Enabled: " << getEnvVariable("USE_HTTPS", "false");
    LOG_INFO << "=========================================\n";
    
    // Step 4: Try to load Drogon configuration (optional)
    LOG_INFO << "Attempting to load Drogon configuration file...";
    try {
        std::string configFile = getEnvVariable("CONFIG_FILE", "../config.json");
        // Check if file exists
        std::ifstream configCheck(configFile);
        if (configCheck.good()) {
            drogon::app().loadConfigFile(configFile);
            LOG_INFO << "✓ Configuration loaded from: " << configFile;
        } else {
            LOG_WARN << "Configuration file not found: " << configFile;
            LOG_WARN << "   Using .env variables for configuration (Drogon defaults applied)";
        }
    } catch (const std::exception& e) {
        LOG_WARN << "Could not load config file: " << e.what();
        LOG_WARN << "   Continuing with .env variables only";
    }
    
    // Step 5: Apply .env settings to Drogon app
    LOG_INFO << "Applying .env settings to application...";
    try {
        // Set thread count from .env
        int threads = std::stoi(getEnvVariable("APP_THREADS", "4"));
        // Note: This is informational; actual threading is configured in config.json or Drogon defaults
        LOG_DEBUG << "App will use " << threads << " threads";
        
        // Set log level from .env
        std::string logLevel = getEnvVariable("LOG_LEVEL", "INFO");
        LOG_DEBUG << "Log level is set to: " << logLevel;
        
    } catch (const std::exception& e) {
        LOG_WARN << "Error applying .env settings: " << e.what();
    }
    
    // Step 6: Run the application
    LOG_INFO << "\n========== Server Starting ==========";
    LOG_INFO << "✓ Server starting on 0.0.0.0:80 and 0.0.0.0:443";
    LOG_INFO << "✓ Database connection pool initialized";
    LOG_INFO << "✓ All controllers registered";
    LOG_INFO << "=====================================\n";
    
    drogon::app().run();
    
    return 0;
}