cmake_minimum_required(VERSION 3.30)
project(ArkanoidCopy C)

set(CMAKE_C_STANDARD 17)

include(FetchContent)

function(include_dependency libName gitURL gitTag)
    FetchContent_Declare(${libName}
            GIT_REPOSITORY ${gitURL}
            GIT_TAG ${gitTag}
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(${libName})
endfunction()

set(LIB1 raylib)
find_package(${LIB1} QUIET)
if (NOT ${LIB1}_FOUND)
    include_dependency(${LIB1} https://github.com/raysan5/raylib.git 5.0)
endif ()


add_executable(ArkanoidCopy main.c)

target_include_directories(ArkanoidCopy PRIVATE ${raylib_INCLUDE_DIRS})

target_link_libraries(ArkanoidCopy PRIVATE ${LIB1})
