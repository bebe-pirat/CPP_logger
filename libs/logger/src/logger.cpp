#include "../include/logger.h"

std::string ILogger::getMessage(Severity_level level, const std::string& text) const {
    auto now = std::chrono::system_clock::now();
    std::time_t time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm{};

    localtime_r(&time_t_now, &local_tm);

    std::ostringstream oss;
    oss << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S") 
        << " [" << level_to_string(level) << "]\t - " << text << "\n";
    return oss.str();
}

Severity_level string_to_level(std::string str) {
    static const std::map<std::string, Severity_level> levels_map = {
        {"DEBUG", Severity_level::DEBUG},
        {"INFO", Severity_level::INFO},
        {"ERROR", Severity_level::ERROR}, 
    };

    auto it = levels_map.find(str);

    if (it == levels_map.end()) throw std::runtime_error("Уровень не найден!");
    
    return it->second;
}

std::string level_to_string(Severity_level level) {
    static const std::map<Severity_level, std::string> levels_map = {
        {Severity_level::DEBUG, "DEBUG"},
        {Severity_level::INFO, "INFO"},
        {Severity_level::ERROR, "ERROR"},  
    };
    auto it = levels_map.find(level);

    if (it != levels_map.end()) return it->second;
    else return "Уровень не найден!";
}

void ILogger::setLevel(Severity_level level) {
    level_ = level;
}

Severity_level ILogger::getLevel() const {
    return level_;
}

FileLogger::FileLogger(Severity_level level, std::string filename) : filename_(filename), out_(filename, std::ios::app) {
    if (!out_.is_open()) throw std::runtime_error("Указанный файл не удалось открыть!");
    setLevel(level);
}

FileLogger::~FileLogger() 
{
    out_.close();
}

void FileLogger::write(Severity_level level, const std::string& text) {
    if (level < getLevel()) return;
    
    std::lock_guard<std::mutex> lock(mutex_); 

    if (!out_.is_open()) throw std::runtime_error("Файл для записи не найден!");

    out_ << getMessage(level, text); 
}
