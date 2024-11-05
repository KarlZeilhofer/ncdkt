cmake_minimum_required(VERSION 3.5)

project(ncdkt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Wide-Character-Version von NCurses verwenden
set(CURSES_NEED_WIDE TRUE)
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
    csr_karte.h
    csr_karte.cpp
)


add_executable(ncdkt ${SOURCE_FILES})

# Verlinken der Wide-Character-Version von NCurses
target_link_libraries(ncdkt ${CURSES_LIBRARIES})
