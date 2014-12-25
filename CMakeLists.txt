set(CMAKE_PREFIX_PATH "C:\\Work\\Tools\\Qt\\5.4.0\\5.4\\mingw491_32")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)


cmake_minimum_required(VERSION 2.8.11)
project(cellular_automata)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)


find_package(OpenGL)
include_directories(${OPENGL_INCLUDE_DIR})
add_definitions(${OPENGL_DEFINITIONS})
find_package(GLEW)

find_package(OpenMP)

# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
include_directories(${Qt5Widgets_INCLUDES})
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(${Qt5Gui_DEFINITIONS})

find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Gui)

get_target_property(QtCore_location Qt5::Core LOCATION)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OpenMP_SHARED_LINKER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -std=c++11 -msse4.1 -O3 -ffast-math -falign-functions -falign-jumps -finline-functions -fweb -ftree-vectorize -mfpmath=sse")

set(SOURCE_FILES
    src/ColorCalculator.cpp
    src/ColorCalculator.h
    src/draw3ds.cpp
    src/draw3ds.h
    src/drawfence.cpp
    src/drawfence.h
    src/drawmushroom.cpp
    src/drawmushroom.h
    src/drawobject.cpp
    src/drawobject.h
    src/drawsnow.cpp
    src/drawsnow.h
    src/glmushroom_logger.cpp
    src/glmushroom_logger.h
    src/glmushroom_model.cpp
    src/glmushroom_model.h
    src/globals.h
    src/textureloader.cpp
    src/textureloader.h
    src/l3ds_generic/l3ds.cpp
    src/l3ds_generic/l3ds.h
    src/main.cpp
    src/mainwindow.h
    src/mainwindow.cpp
    )

include_directories(src)
add_executable(cellular_automata ${SOURCE_FILES})

# The Qt5Widgets_LIBRARIES variable also includes QtGui and QtCore
TARGET_LINK_LIBRARIES(cellular_automata ${Qt5Widgets_LIBRARIES} ${OPENGL_LIBRARIES})

qt5_use_modules(cellular_automata Widgets)