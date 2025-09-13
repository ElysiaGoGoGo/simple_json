
file(GLOB_RECURSE SOURCES ./*.cpp)

add_executable(simple_json  ${SOURCES})

target_include_directories(simple_json PUBLIC ../inc)
