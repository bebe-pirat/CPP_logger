#include "logger.h"
#include <thread>

// принимает порт и ip адрес, посылает 100 сообщений с интервалом 1 секунда через сокет
int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Использование: " << argv[0] << " <порт> <ip адрес>\n";
    return 1;
  }

  try {
    unsigned short port = static_cast<unsigned short>(std::stoi(argv[1]));

    SocketLogger logger(Severity_level::DEBUG, port, argv[2]);

    for (int i = 0; i < 100; ++i) {
      logger.write(Severity_level::INFO, "Test message " + std::to_string(i));
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
  } catch (const std::invalid_argument e) {
    std::cerr << "Некорректный номер порта: не является числом" << std::endl;
  } catch (std::runtime_error e) {
    std::cerr << e.what() << std::endl;
  }
}