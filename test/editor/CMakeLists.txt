# leo
project(propertyEditor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH ${QT_DIR})

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

file(GLOB_RECURSE SRC_FILES "*.cpp")
file(GLOB_RECURSE HDR_FILES "*.h")
file(GLOB_RECURSE UI_FILES "*.ui")

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HDR_FILES} ${UI_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets mao_property_tree)
target_link_libraries(${PROJECT_NAME} PRIVATE mao_library)
target_link_libraries(${PROJECT_NAME} PRIVATE mao_serialization)
target_link_libraries(${PROJECT_NAME} PRIVATE mao_reflection)

set_target_properties(${PROJECT_NAME} PROPERTIES
                      MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
                      MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
                      MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
                      MACOSX_BUNDLE TRUE
                      WIN32_EXECUTABLE TRUE
                      )

install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
