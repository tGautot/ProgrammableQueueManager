
set(DAEMON_EXEC_NAME qmd)

add_executable(${DAEMON_EXEC_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

target_compile_options(${DAEMON_EXEC_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)

#target_include_directories(qmd PUBLIC thirdparty common)
target_compile_features(${DAEMON_EXEC_NAME} PRIVATE cxx_std_17)
set_target_properties(${DAEMON_EXEC_NAME} PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF CXX_STANDARD_REQUIRED ON)

target_link_libraries(${DAEMON_EXEC_NAME} PRIVATE datalayer)
