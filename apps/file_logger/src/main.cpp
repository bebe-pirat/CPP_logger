#include <logger.h> 
#include <stdexcept>
#include <iostream>
#include <map>
#include <string>
#include <thread>

// исправить слово на цифру, а то ничего не раотает 
Severity_level get_level() {
    int level_num;
    Severity_level level;

    std::cout << "\nВведите номер уровня сообщения: \n" <<
                        "1. DEBUG, \n" << 
                        "2. INFO, \n" << 
                        "3. ERROR. \n";
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

void write_thread(FileLogger* logger, Severity_level level, const std::string& message) {
    logger->write(level, message);
}

void change_thread(FileLogger* logger, Severity_level level) {
    logger->setLevel(level);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "При запуске программы не было найден файл логгера и/или уровень сообщения по умолчанию!\n";
        return 1;
    }

    try {
        FileLogger logger(string_to_level(argv[2]), argv[1]);
        std::vector<std::unique_ptr<std::thread>> threads;
        int choice = 0;

        do {
            std::cout << "\nВведите номер действия: \n" << 
                                "1. Ввод сообщения, \n" << 
                                "2. Изменение уровня логгера по умолчанию, \n" << 
                                "3. Выход из программы. \n";
            std::cin >> choice;

            switch (choice)
            {
            case 1:
            {
                std::string str;

                std::cout << "\nВведите сообщение: ";
                std::cin.clear();
                std::cin.ignore();
                getline(std::cin, str);
                
                try {
                    Severity_level level = get_level();
                    threads.emplace_back(
                        std::make_unique<std::thread>(
                            write_thread, &logger, level, str
                        )
                    );                
                }
                catch (std::runtime_error e) {
                    std::cerr << "Ошибка: " << e.what() << "\n";
                }                

                break;
            }         
            case 2: 
            {
                try {
                    Severity_level level = get_level();
                    threads.emplace_back(
                        std::make_unique<std::thread>(
                            change_thread, &logger, level
                        )
                    ); 
                }
                catch (std::runtime_error e) {
                    std::cerr << "Ошибка: " << e.what() << "\n";
                }
                
                break;
            }
            case 3:
                break;
            default:
                "Такого номера нет! Попробуйте ввести снова!";
                break;
            }
            
        } while (choice != 3);

        for(auto& t : threads) {
            if(t->joinable()) t->join();
        }
    }
    catch(std::runtime_error e) {
        std::cerr << "Ошибка: " << e.what() << '\n';
    }
    
    return 0;
}