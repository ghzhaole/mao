project(mao_serialization)

add_definitions(-DMAO_SERIALIZATION_EXPORTS_H)

file(GLOB_RECURSE SRC_FILES "*.cpp")
file(GLOB_RECURSE HDR_FILES "*.h")

# 使用rapidjson11
include(${THIRD_PARTY}/third.cmake)

# 生成动态库
add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${HDR_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${RAPID_JSON_DIR})
target_link_libraries(${PROJECT_NAME} mao_reflection)
set_target_properties(${PROJECT_NAME} PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
                      LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
                      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})