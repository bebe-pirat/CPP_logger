cmake_minimum_required(VERSION 3.5)
project(logging_system VERSION 1.0.0 LANGUAGES CXX)

# Общие настройки для всего проекта
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Опция для сборки shared библиотек
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# Добавляем подпроекты
add_subdirectory(libs/logger)
add_subdirectory(apps/log_stat_collector)
add_subdirectory(apps/log_sender_additional)
add_subdirectory(apps/file_logger)

# Создаем список всех целей для экспорта
set(EXPORT_TARGETS logger stat_collector log_sender file_logger)

# Установка для всего проекта
install(TARGETS ${EXPORT_TARGETS}
    EXPORT logging_system-targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(EXPORT logging_system-targets
    FILE logging_systemTargets.cmake
    NAMESPACE logging_system::
    DESTINATION lib/cmake/logging_system
)