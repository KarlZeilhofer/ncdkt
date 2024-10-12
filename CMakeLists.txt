cmake_minimum_required(VERSION 3.5)

# Projektname und Sprache festlegen
project(ncdkt LANGUAGES CXX)

# C++ Standard setzen
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# NCurses-Bibliothek finden und einbinden
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# Hinzufügen des ausführbaren Ziels und der Quellcode-Dateien
add_executable(ncdkt main.cpp spiel.cpp spielfeld.cpp spieler.cpp)

# Verlinken der NCurses-Bibliothek
target_link_libraries(ncdkt ${CURSES_LIBRARIES})
