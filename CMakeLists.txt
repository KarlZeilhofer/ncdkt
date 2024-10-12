cmake_minimum_required(VERSION 3.5)

project(ncdkt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# NCurses-Bibliothek finden und einbinden
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# Hinzufügen des ausführbaren Ziels und der Quellcode-Dateien
set(SOURCE_FILES
    main.cpp
    spiel.h
    spiel.cpp
    spieler.h
    spieler.cpp
    spielfeld.h
    spielfeld.cpp
)

add_executable(ncdkt ${SOURCE_FILES})

# Verlinken der NCurses-Bibliothek
target_link_libraries(ncdkt ${CURSES_LIBRARIES})
