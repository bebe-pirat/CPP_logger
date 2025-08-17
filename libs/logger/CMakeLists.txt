cmake_minimum_required(VERSION 3.5) 
project(logger VERSION 1.0.0 LANGUAGES CXX)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)  

# sources
set(LOGGER_SOURCES src/logger.cpp)
set(LOGGER_INCLUDES include/logger.h)

# создание библиотеки тип определяется BUILD_SHARED_LIBS
add_library(logger ${LOGGER_SOURCES})

# публичные заголовочные файлы
target_include_directories(logger PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# установочные правила 
install(TARGETS logger
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(FILES ${LOGGER_INCLUDES} DESTINATION include)