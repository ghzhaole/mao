# leo
project(test_thread VERSION 0.1 LANGUAGES CXX)

file(GLOB_RECURSE SRC_FILES "*.cpp")
file(GLOB_RECURSE HDR_FILES "*.h")

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HDR_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE mao_threadPool)

