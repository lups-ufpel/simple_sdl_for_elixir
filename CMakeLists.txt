cmake_minimum_required(VERSION 3.12)
project(SDL2_NIFS VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Find Erlang development libraries - it is REQUIRED, we can't proceed without this!! ---- 

# Get erlang executable
find_program(ERLANG_EXECUTABLE erl)

if(NOT ERLANG_EXECUTABLE)
    message(FATAL_ERROR "Erlang executable (erl) not found! Please make sure you have Erlang and Elixir installed and their executables are in your PATH.")
endif()

# Get Erlang include directory by running a command in the shell and capturing its output
execute_process(
    COMMAND ${ERLANG_EXECUTABLE} -noshell -eval "io:format(\"~s\", [lists:concat([code:root_dir(), \"/erts-\", erlang:system_info(version), \"/include\"])]), halt()."
    OUTPUT_VARIABLE ERLANG_INCLUDE_DIR
    RESULT_VARIABLE ERLANG_RESULT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Check if the command executed successfully and if the expected header file exists in the include directory
if(NOT ERLANG_RESULT EQUAL 0 OR NOT EXISTS "${ERLANG_INCLUDE_DIR}/erl_nif.h")
    message(FATAL_ERROR "Could not find erl_nif.h inside ${ERLANG_INCLUDE_DIR}")
endif()

# If everything works, this will print the path to the include directory where erl_nif.h is located
message(STATUS "Found Erlang include directory: ${ERLANG_INCLUDE_DIR}")

# ---- Find SDL2 ---- 
find_package(SDL2 REQUIRED)

# ---- Set sources ----

set(SDL2_NIFS_SRC
    c_src/sdl2_nifs.cpp
)

set (SDL2_INTERFACE_SRC
    c_src/SDL2Interface/SDL2Interface.cpp
)

# ---- Creating libraries used by the NIFs and making their headers available ----

add_library(SDL2_INTERFACE STATIC ${SDL2_INTERFACE_SRC})
target_include_directories(SDL2_INTERFACE PUBLIC c_src)

# Marking the library as POSITION_INDEPENDENT_CODE for shared library compatibility
set_target_properties(SDL2_INTERFACE PROPERTIES POSITION_INDEPENDENT_CODE ON)

# ---- Creating the NIF library and linking it ----
add_library(sdl2_nifs SHARED ${SDL2_NIFS_SRC})
target_include_directories(sdl2_nifs PRIVATE ${ERLANG_INCLUDE_DIR})

target_link_libraries(sdl2_nifs PRIVATE SDL2_INTERFACE)
target_link_libraries(sdl2_nifs PRIVATE SDL2::SDL2 SDL2::SDL2main)

# Removing the 'lib' prefix from the generated shared library
set_target_properties(sdl2_nifs PROPERTIES PREFIX "")

# ---- Copy the generated shared library to the 'priv' folder after building ---
if(DEFINED COMPILED_APP_ROOT)
    set(PRIV_DIR "${COMPILED_APP_ROOT}/priv")
else()
    set(PRIV_DIR "${CMAKE_SOURCE_DIR}/priv")
endif()

add_custom_target(copy_sdl2_nifs_to_priv ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PRIV_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:sdl2_nifs>" "${PRIV_DIR}/$<TARGET_FILE_NAME:sdl2_nifs>"
    DEPENDS sdl2_nifs
    COMMENT "Copying sdl2_nifs to priv/ directory on '${PRIV_DIR}'..."
)