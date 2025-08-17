cmake_minimum_required(VERSION 3.5)
project(file_logger VERSION 1.0.0 LANGUAGES CXX)

# sources
set(FILE_LOGGER_SOURCES src/main.cpp src/help_func.cpp)
set(FILE_LOGGER_INCLUDES include/help_func.h)

# Добавляем поддиректорию с библиотекой
add_subdirectory(../../libs/logger ${CMAKE_BINARY_DIR}/libs/logger)

# Создаем исполняемый файл
add_executable(file_logger ${FILE_LOGGER_SOURCES})

# Связываем с библиотекой
target_link_libraries(file_logger PRIVATE logger)

# Добавляем пути к заголовочным файлам
target_include_directories(file_logger PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/libs/logger/include
)