#include "../include/help_func.h"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "При запуске программы не было найден файл логгера и/или "
                 "уровень сообщения по умолчанию!\n";
    return 1;
  }

  try {
    FileLogger logger(string_to_level(argv[2]), argv[1]);
    std::vector<std::unique_ptr<std::thread>> threads;
    int choice = 0;

    do {
      std::cout << "\nВведите номер действия: \n"
                << "1. Ввод сообщения, \n"
                << "2. Изменение уровня логгера по умолчанию, \n"
                << "3. Выход из программы. \n";
      
      // обработка ситуации, когда пользователь ввел вместо числа что-то другое (цикл, который закончится, когда пользователь введет число)
      while (!(std::cin >> choice)) {
        std::cout << "Ошибка! Введите число от 1 до 3: ";
        
        // игноруем все, что ввел пользователь до конца строки
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        // убираем флаг ошибки
        std::cin.clear();
      }

      switch (choice) {
      case 1: {
        std::string str;

        std::cout << "\nВведите сообщение: ";
        std::cin.clear();
        std::cin.ignore();
        getline(std::cin, str);

        try {
          Severity_level level = get_level();

          // создаем поток и добавляем его в очередь
          threads.emplace_back(
              std::make_unique<std::thread>(write_thread, &logger, level, str));
        } catch (std::runtime_error e) {
          std::cerr << "Ошибка: " << e.what() << "\n";
        }

        break;
      }
      case 2: {
        try {
          Severity_level level = get_level();
          threads.emplace_back(
              std::make_unique<std::thread>(change_thread, &logger, level));
        } catch (std::runtime_error e) {
          std::cerr << "Ошибка: " << e.what() << "\n";
        }

        break;
      }
      case 3:
        break;
      default:
        std::cout << "Число вне диапозона!\n";
        break;
      }

    } while (choice != 3);

    for (auto &t : threads) {
      if (t->joinable())
        t->join();
    }
  } catch (std::runtime_error e) {
    std::cerr << "Ошибка: " << e.what() << '\n';
  }

  return 0;
}