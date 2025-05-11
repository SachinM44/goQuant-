cmake_minimum_required(VERSION 3.20)
project(GoQuant)

set(CMAKE_CXX_STANDARD 17)

# Find packages (assuming you installed them via vcpkg)
find_package(Boost REQUIRED)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(SQLite3 REQUIRED)

# Add all source files
set(SOURCES
    goQuant/src/main.cpp
    goQuant/src/ui/MainWindow.cpp
    goQuant/src/ui/InputPanel.cpp
    goQuant/src/ui/OutputPanel.cpp
    goQuant/src/websocket/WebSocketClient.cpp
    goQuant/src/models/AlmgrenChriss.cpp
    goQuant/src/models/FeeModel.cpp
    goQuant/src/models/SlippageModel.cpp
)

# Add executable with all source files
add_executable(goquant ${SOURCES})

# Link the libraries
target_link_libraries(goquant
    Boost::boost
    Qt5::Core Qt5::Gui Qt5::Widgets
    Eigen3::Eigen
    nlohmann_json::nlohmann_json
    SQLite::SQLite3
)