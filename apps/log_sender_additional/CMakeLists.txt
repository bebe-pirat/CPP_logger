cmake_minimum_required(VERSION 3.5)
project(log_sender)

# sources
set(LOG_SENDER_SOURCES src/main.cpp)

# Создаем исполняемый файл
add_executable(log_sender ${LOG_SENDER_SOURCES})

# Связываем с библиотекой
target_link_libraries(log_sender PRIVATE logger)

# Добавляем пути к заголовочным файлам
target_include_directories(log_sender PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/libs/logger/include
)