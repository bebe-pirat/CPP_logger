#include <logger.h> 
#include <stdexcept>
#include <iostream>
#include <map>
#include <string>
#include <thread>

Severity_level get_level();
void write_thread(FileLogger* logger, Severity_level level, const std::string& message);
void change_thread(FileLogger* logger, Severity_level level);