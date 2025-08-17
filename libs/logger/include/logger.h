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
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <cerrno> 

enum class Severity_level {
    DEBUG, 
    INFO,
    ERROR
};

std::string level_to_string(Severity_level level);
Severity_level string_to_level(std::string str);

// базовый класс ILogger
class ILogger {
    private: 
        Severity_level level_;
public:
    virtual ~ILogger() = default;
    virtual void write(Severity_level level, const std::string& text) = 0;
    void setLevel(Severity_level level);
    Severity_level getLevel() const;
    std::string getMessage(Severity_level level, const std::string& text) const; // метод для формирования строки лога
};

// дочерний класс для записи лога в файл
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

// дочерний класс для записи лога в сокет
class SocketLogger : public ILogger {
    std::string host_;
    unsigned short port_;
    int sockfd_ = -1;
    std::mutex mutex_;
    
    void connect_sock();
    
public:
    SocketLogger(Severity_level level, unsigned short port, std::string host);
    ~SocketLogger();
    void write(Severity_level level, const std::string& text) override;
};

#endif  