#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <chrono>
#include <map>
#include <fstream>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include <mutex>

enum class Severity_level {
    DEBUG, 
    INFO,
    ERROR
};

std::string level_to_string(Severity_level level); // function for turning Severity level into string
Severity_level string_to_level(std::string str); // function for turning string into Severity level

class ILogger {
    private: 
        Severity_level level_;
public:
    virtual ~ILogger() = default;
    virtual void write(Severity_level level, const std::string& text) = 0;
    void setLevel(Severity_level level);
    Severity_level getLevel() const;
    std::string getMessage(Severity_level level, const std::string& text) const;
};

class FileLogger : public ILogger {
    private: 
        std::string filename_;
        std::ofstream out_;
        std::mutex mutex_;
    public: 
        ~FileLogger();
        FileLogger(Severity_level level, std::string filename);
        void write(Severity_level level, const std::string& text) override;
};

#endif  