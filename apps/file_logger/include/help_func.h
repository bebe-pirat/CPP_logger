#include <iostream>
#include <logger.h>
#include <map>
#include <stdexcept>
#include <string>
#include <thread>

Severity_level get_level();
void write_thread(FileLogger *logger, Severity_level level,
                  const std::string &message);
void change_thread(FileLogger *logger, Severity_level level);