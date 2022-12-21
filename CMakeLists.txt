cmake_minimum_required(VERSION 3.5)

project(RTCFD_Code VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(wxWidgets REQUIRED gl core base propgrid OPTIONAL_COMPONENTS net)
include(${wxWidgets_USE_FILE})

# OpenMP
FIND_PACKAGE(OpenMP REQUIRED)

if(OPENMP_FOUND)
    message("OpenMP found")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

include_directories("${PROJECT_SOURCE_DIR}/include")

# set(TS_FILES RTCFD_Code_en_US.ts)
set(PROJECT_SOURCES
    include/GUIFrame.h
    include/settings.h
    include/wxRTCFD_Code_App.h
    include/wxRTCFD_Code_Main.h
    src/GUIFrame.cpp
    src/WxWizFrame.fbp
    src/settings.cpp
    src/wxRTCFD_Code_App.cpp
    src/wxRTCFD_Code_Main.cpp
    src/fluid.cpp
    include/fluid.h
    src/region.cpp
    include/region.h
    src/draw.cpp
    include/draw.h
)

add_executable(RTCFD_Code ${PROJECT_SOURCES})

target_link_libraries(RTCFD_Code PRIVATE ${wxWidgets_LIBRARIES} -O3 -fopenmp)

set_target_properties(RTCFD_Code PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS RTCFD_Code
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
