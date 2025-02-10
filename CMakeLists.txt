cmake_minimum_required(VERSION 3.26 FATAL_ERROR)
project(AutoBackup VERSION 0.2.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_AUTOMOC true)


file(GLOB SOURCE src/*.cpp)
file(GLOB HEADER include/*.h)
set(PROJECT_SOURCES ${SOURCE} ${HEADER})

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

find_package(Qt6 REQUIRED COMPONENTS Core)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Core)

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_compile_definitions(${PROJECT_NAME} PRIVATE
                           -DAPP_VERSION="${PROJECT_VERSION}"
                           -DPROJECT_NAME="${PROJECT_NAME}")
