#include "../include/logger.h"

// функция для получения логга 
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

// функция для преобразования данной строки в уровень из перечисления 
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

// функция для преобразования уровня из перечисления в строку 
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

// функция для записи логга в файл
void FileLogger::write(Severity_level level, const std::string& text) {
    if (level < getLevel()) return;
    
    std::lock_guard<std::mutex> lock(mutex_); 

    if (!out_.is_open()) throw std::runtime_error("Файл для записи не открыт!");

    out_ << getMessage(level, text); 
    out_.flush();
}

SocketLogger::SocketLogger(Severity_level level, unsigned short port, std::string host) : port_(port), host_(host)
{
    connect_sock();
    setLevel(level);
}

// функция для создания и соединения
void SocketLogger::connect_sock() {
    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_ < 0) {
        throw std::runtime_error("Сокет не удалось создать!");
    }

    sockaddr_in server_addr_;
    server_addr_.sin_family = AF_INET;
    server_addr_.sin_port = htons(port_);

    if (inet_pton(AF_INET, host_.c_str(), &server_addr_.sin_addr) <= 0) {
            close(sockfd_);
            throw std::runtime_error("Неверный адрес!");
        }


    if (connect(sockfd_, (sockaddr*)&server_addr_, sizeof(server_addr_)) < 0) {
        close(sockfd_);
        throw std::runtime_error("Подключение не удалось совершить!");
    }
}

SocketLogger::~SocketLogger() {
    close(sockfd_);
}

// функция для записи логга в сокет
void SocketLogger::write(Severity_level level, const std::string& text) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (sockfd_ < 0) {
        try {
            connect_sock();
        }
        catch(...) {
            throw std::runtime_error("Переподключение не удалось совершить!");
        }
    }

    std::string str = getMessage(level, text);
    if (send(sockfd_, str.c_str(), str.size(), 0) < 0) {
        close(sockfd_);
        sockfd_ = -1;
        throw std::runtime_error("Отправление не удалось совершить: " + std::string(strerror(errno))); 
    }
}