#include "../include/help_func.h"

Severity_level get_level() {
  int level_num;
  Severity_level level;

  std::cout << "\nВведите номер уровня сообщения: \n"
            << "1. DEBUG, \n"
            << "2. INFO, \n"
            << "3. ERROR. \n";
  std::cin >> level_num;

  switch (level_num) {
  case 1:
    level = Severity_level::DEBUG;
    break;
  case 2:
    level = Severity_level::INFO;
    break;
  case 3:
    level = Severity_level::ERROR;
    break;
  default:
    throw std::runtime_error("Номера не существует попробуйте снова!");
  }

  return level;
}

void write_thread(FileLogger *logger, Severity_level level,
                  const std::string &message) {
  logger->write(level, message);
}

void change_thread(FileLogger *logger, Severity_level level) {
  logger->setLevel(level);
}