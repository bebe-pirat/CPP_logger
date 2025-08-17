#ifndef STAT_COLLECTOR_H
#define STAT_COLLECTOR_H

#include <atomic>
#include <ctime>
#include <logger.h>
#include <map>
#include <mutex>
#include <thread>
#include <vector>

constexpr unsigned SECONDS_IN_HOUR = 3600;
constexpr unsigned TIME_TO_FINISH = 10;

class StatCollector {
  struct MessageInfo {
    Severity_level level;
    size_t length;
    std::time_t timestamp;
  };

  unsigned short port_;
  size_t n_messages_;
  unsigned t_seconds_;

  std::vector<MessageInfo> messages_;
  std::map<Severity_level, int> levels_counts_{{Severity_level::DEBUG, 0},
                                               {Severity_level::INFO, 0},
                                               {Severity_level::ERROR, 0}};
  size_t min_lenght_ = 0;
  size_t max_lenght_ = 0;
  size_t total_lenght_ = 0;

  bool changed_{false};

  // метод для обработки полученного сообщения
  void process_message(std::string message);

  // метод для печати статистики
  void print_stats() const;

public:
  StatCollector(unsigned short port, size_t n_messages, unsigned t_seconds);

  // метод, запускающий коллектор
  void run();
};

#endif