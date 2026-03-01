#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

struct Config {
    enum class Environment {
        Local,
        Real
    };

    static Environment currentEnv;

    static QString getBaseUrl() {
        if (currentEnv == Environment::Real) {
            // AlwaysData URL - adjust if actual AlwaysData host is different
            // Given the DB is on AlwaysData, the API likely is too
            return "http://guemoune4.alwaysdata.net"; 
        }
        return "http://127.0.0.1:8080";
    }

    static QString getEnvName() {
        return (currentEnv == Environment::Real) ? "REAL (AlwaysData)" : "LOCAL (Virtual/Local)";
    }
};

// Definition of static member should be in a .cpp if used in multiple places,
// but for simplicity in this project we can define it in main.cpp
// Environment Config::currentEnv = Config::Environment::Local;

#endif // CONFIG_H
