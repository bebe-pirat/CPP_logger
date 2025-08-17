cmake_minimum_required(VERSION 3.5)
project(logging_system VERSION 1.0.0 LANGUAGES CXX)

# Общие настройки для всего проекта
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Опция для сборки shared библиотек
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# Добавляем подпроекты
add_subdirectory(libs/logger)
add_subdirectory(apps/log_stat_collector)
add_subdirectory(apps/log_sender)
add_subdirectory(apps/file_logger)

# Установка для всего проекта (опционально)
install(EXPORT logging_system-targets
    FILE logging_systemTargets.cmake
    NAMESPACE logging_system::
    DESTINATION lib/cmake/logging_system
)