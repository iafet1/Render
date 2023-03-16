cmake_minimum_required(VERSION 3.0.0)
project(Render VERSION 0.1.0)

include(CTest)
enable_testing()

# Configure CMake global variables
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)

add_compile_definitions(_LOG)

include_directories(
    ./src/
    ./src/shaders/
    ./src/platform/
    ./src/render/
    $ENV{VULKAN_SDK}/Include/
)

# Перебор файлов шейдоров и их компиляция
file(GLOB_RECURSE SHADERS CONFIGURE_DEPENDS src/shaders/*.glsl)
foreach(shader IN ITEMS  ${SHADERS})
    get_filename_component(file_name ${shader} NAME_WLE)
    add_custom_command(
        OUTPUT data/${file_name}.spv
        DEPENDS ${shader} 
        COMMAND $ENV{VULKAN_SDK}/Bin/glslangValidator -V ${shader} -o data/${file_name}.spv
    )
    
    list(APPEND SHADERS_RESULTS data/${file_name}.spv)
endforeach(shader IN ITEMS ${SHADERS})

# Добавление в контекст данных
add_custom_target(
    Data_Context ALL
    DEPENDS ${SHADERS_RESULTS}
)

# Перебор исходных файлов и их компиляция
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(Render ${SOURCES} main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
