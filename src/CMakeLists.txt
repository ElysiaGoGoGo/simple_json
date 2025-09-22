
file(GLOB_RECURSE SOURCES ./*.cpp)

add_executable(simple_json  ${SOURCES})

# 启用C++异常处理支持
if(MSVC)
    target_compile_options(simple_json PRIVATE /EHsc /utf-8)
endif()

target_include_directories(simple_json PRIVATE ../inc)
