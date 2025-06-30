cmake_minimum_required(VERSION 3.31)
project(CurveVisualizer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        PrintSupport
        REQUIRED
)

include(FetchContent)
FetchContent_Declare(
        qcustomplot
        URL https://www.qcustomplot.com/release/2.1.1/QCustomPlot-source.tar.gz
)
FetchContent_MakeAvailable(qcustomplot)
add_library(qcustomplot
        "${qcustomplot_SOURCE_DIR}/qcustomplot.h"
        "${qcustomplot_SOURCE_DIR}/qcustomplot.cpp"
)
target_include_directories(qcustomplot PRIVATE ${Qt6Core_INCLUDE_DIRS})
target_link_libraries(qcustomplot PRIVATE Qt6::Core)

add_executable(${PROJECT_NAME} src/main.cpp
        src/include/MainWindow.h
        src/MainWindow.cpp
        src/PlotController.cpp
        src/PlotSplitter.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
        src/include
)

target_link_libraries(${PROJECT_NAME}
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::PrintSupport
        qcustomplot
)
