#include "stat_collector.h"

StatCollector::StatCollector(unsigned short port, size_t n_messages,
                             unsigned t_seconds)
    : port_(port), n_messages_(n_messages), t_seconds_(t_seconds) {}

void StatCollector::process_message(std::string message) {
  std::tm time_info = {};
  char level_str[10] = {0};
  char msg_text[1024] = {0};

  changed_ = true;

  // парсинг строки
  int parsed = sscanf(message.c_str(), "%d-%d-%d %d:%d:%d [%[^]]] - %[^\n]",
                      &time_info.tm_year, &time_info.tm_mon, &time_info.tm_mday,
                      &time_info.tm_hour, &time_info.tm_min, &time_info.tm_sec,
                      level_str, msg_text);

  if (parsed != 8) {
    throw std::runtime_error("Преобразовать сообщение не удалось: " + message);
  }

  // преобразование времени
  std::time_t timestamp = std::mktime(&time_info);
  if (timestamp == -1) {
    throw std::runtime_error("Неверное время: " + message);
    return;
  }

  Severity_level level;
  try {
    level = string_to_level(level_str);
  } catch (const std::runtime_error &e) {
    throw e;
  }

  size_t length = message.length();

  // Обновление статистики
  MessageInfo info;
  info.level = level;
  info.length = length;
  info.timestamp = timestamp;

  messages_.push_back(info);
  levels_counts_[level]++;

  // Обновление статистики по длине
  if (length < min_lenght_ || min_lenght_ == 0)
    min_lenght_ = length;
  if (length > max_lenght_)
    max_lenght_ = length;
  total_lenght_ += length;
}

void StatCollector::print_stats() const {
  int total_mes_count = messages_.size();
  int mes_count_in_hour = 0;

  auto now = std::chrono::system_clock::now();

  // цикл, который считает, сколько сообщений пришло за час
  for (size_t i = 0; i < messages_.size(); ++i) {
    auto msg_time =
        std::chrono::system_clock::from_time_t(messages_.at(i).timestamp);
    auto diff = now - msg_time;

    if (std::chrono::duration_cast<std::chrono::seconds>(diff) <=
        std::chrono::seconds(SECONDS_IN_HOUR))
      mes_count_in_hour++;
  }

  size_t average_len = total_lenght_ / messages_.size();

  std::cout << "Сообщений всего: " << total_mes_count << std::endl
            << "Количество сообщений уровеня важности "
            << level_to_string(Severity_level::DEBUG) << ": "
            << levels_counts_.at(Severity_level::DEBUG) << std::endl
            << "Количество сообщений уровеня важности "
            << level_to_string(Severity_level::INFO) << ": "
            << levels_counts_.at(Severity_level::INFO) << std::endl
            << "Количество сообщений уровеня важности "
            << level_to_string(Severity_level::ERROR) << ": "
            << levels_counts_.at(Severity_level::ERROR) << std::endl
            << "Минимальная длина сообщения: " << min_lenght_ << std::endl
            << "Максимальная длина сообщения: " << max_lenght_ << std::endl
            << "Средняя длина сообщения: " << average_len << std::endl;
}

void StatCollector::run() {
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    throw std::runtime_error("Создание сокета провалилось!");
  }

  sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port_);

  if (bind(server_socket, (sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    throw std::runtime_error("Связывание сокета провалилось!");
  }

  if (listen(server_socket, 5) < 0) {
    throw std::runtime_error("Прослушивание провалилось");
  }

  auto last_print = std::chrono::system_clock::now();

  while (1) {
    sockaddr_in client_addr{};
    socklen_t addr_len = sizeof(client_addr);
    int client_sock =
        accept(server_socket, (sockaddr *)&client_addr, &addr_len);

    if (client_sock < 0) {
      throw std::runtime_error("Установка соединения провалилась!");
    }

    char buffer[1024];

    while (1) {
      int bytes = recv(client_sock, buffer, sizeof(buffer), 0);
      if (bytes <= 0)
        break;

      std::string message(buffer, bytes);
      std::cout << "Полученное сообщение: " << message << "\n\n";
      process_message(message);

      auto now = std::chrono::system_clock::now();
      if (messages_.size() != 0 && n_messages_ != 0 &&
              messages_.size() % n_messages_ == 0 ||
          std::chrono::duration_cast<std::chrono::seconds>(now - last_print)
                      .count() >= t_seconds_ &&
              changed_) {
        print_stats();
        last_print = now;
        changed_ = false;
      }
    }
  }

  close(server_socket);
}
