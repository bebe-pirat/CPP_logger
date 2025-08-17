cmake_minimum_required(VERSION 3.5)
project(stat_collector)

# sources
set(STAT_COLLECTOR_SOUCRES src/main.cpp src/stat_collector.cpp)
set(STAT_COLLECTOR_INCLUDES include/stat_collector.h)

# Добавляем поддиректорию с библиотекой
add_subdirectory(../../libs/logger ${CMAKE_BINARY_DIR}/libs/logger)

# Создаем исполняемый файл
add_executable(stat_collector ${STAT_COLLECTOR_SOUCRES})

# Связываем с библиотекой
target_link_libraries(stat_collector PRIVATE logger)

# Добавляем пути к заголовочным файлам
target_include_directories(stat_collector PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/libs/logger/include
)