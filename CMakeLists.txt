cmake_minimum_required(VERSION 3.31)
project(Euronext_Surveillance)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Charts
        REQUIRED)

include_directories(.)

add_executable(Euronext_Surveillance
        Alert.h
        AlertObserver.cpp
        AlertObserver.h
        CeilingProcessor.cpp
        CeilingProcessor.h
        ChartFraudObserver.h
        ChartOrderObserver.h
        DbConnector.cpp
        DbConnector.h
        FloorProcessor.cpp
        FloorProcessor.h
        FraudMonitor.cpp
        IFraudObserver.h
        IOrderObserver.h
        IProcessor.h
        main.cpp
        mainwindow.ui
        multipleViewsWindow.cpp
        multipleViewsWindow.h
        Order.h
        PumpDumpProcessor.cpp
        PumpDumpProcessor.h
        QuoteProcessor.cpp
        QuoteProcessor.h
        SearchableTableWidget.cpp
        SearchableTableWidget.h
        SpoofingProcessor.cpp
        SpoofingProcessor.h
        ui_mainwindow.h
        WashProcessor.cpp
        WashProcessor.h)


target_link_libraries(Euronext_Surveillance
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Charts
)