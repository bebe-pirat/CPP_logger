#include <iostream>
#include <logger.h>
#include <map>
#include <stdexcept>
#include <string>
#include <thread>

// функция для получения уровня от пользователя
Severity_level get_level();

// функция для записи логга в файл
void write_thread(FileLogger *logger, Severity_level level,
                  const std::string &message);

// функция для изменения уровня
void change_thread(FileLogger *logger, Severity_level level);