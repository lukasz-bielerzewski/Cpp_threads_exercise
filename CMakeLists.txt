cmake_minimum_required(VERSION 3.5)

project(Zadanie_SCR LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Zadanie_SCR main.cpp
                           zadanie1.cpp
                           zadanie2.cpp
                           zadanie3.cpp
                           zadanie4.cpp
                           zadanie5.cpp)

install(TARGETS Zadanie_SCR
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
