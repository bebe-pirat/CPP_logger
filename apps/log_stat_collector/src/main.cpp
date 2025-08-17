#include "stat_collector.h"
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cerr << "Использование: " << argv[0]
              << " <порт> <число сообщений> <количество секунд>\n";
    return 1;
  }

  try {
    unsigned short port = static_cast<unsigned short>(std::stoi(argv[1]));
    size_t n_messages = static_cast<size_t>(std::stoi(argv[2]));
    unsigned t_seconds = static_cast<unsigned>(std::stoi(argv[3]));

    StatCollector collector(port, n_messages, t_seconds);
    collector.run();
  } catch (const std::exception &e) {
    std::cerr << "Ошибка: " << e.what() << "\n";
    return 2;
  }

  return 0;
}