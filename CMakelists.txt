cmake_minimum_required(VERSION 3.16)
project(KU_Student_Expense_Tracker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# Find Qt5 or Qt6 Widgets package automatically
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(include)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/MainWindow.cpp
    src/Expense.cpp
    src/Budget.cpp
    src/AlertSystem.cpp
    src/FileManager.cpp
    src/ActivityLog.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)