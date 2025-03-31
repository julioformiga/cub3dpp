cmake_minimum_required(VERSION 3.20)
project(cub3dpp VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# First try to find SFML 3.x installed on the system
find_package(
  SFML 3
  COMPONENTS Graphics Window System
  QUIET)

if(SFML_FOUND)
  message(STATUS "Found SFML ${SFML_VERSION} installed on the system")
  set(USE_SYSTEM_SFML TRUE)
else()
  message(STATUS "SFML 3.x not found on the system, downloading SFML 3.0.0...")
  # Use FetchContent to download and build SFML 3.0.0
  include(FetchContent)
  FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0)
  FetchContent_MakeAvailable(SFML)
  message(STATUS "SFML 3.0.0 downloaded and configured")
  set(USE_SYSTEM_SFML FALSE)
endif()

file(GLOB SOURCES ${SRC_DIR}/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

# Use the appropriate target names based on whether we're using system SFML or
# not
target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics SFML::System
                                              SFML::Window)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -MP -O1)
elseif(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

if(WIN32 AND NOT USE_SYSTEM_SFML)
  # Only need to copy DLLs if we're using the downloaded version
  add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND
      ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:sfml-graphics>
      $<TARGET_FILE:sfml-window> $<TARGET_FILE:sfml-system>
      $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()

# Set runtime dependencies search paths for Linux if we're using downloaded SFML
if(UNIX
   AND NOT APPLE
   AND NOT USE_SYSTEM_SFML)
  set_target_properties(
    ${PROJECT_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN" BUILD_WITH_INSTALL_RPATH
                                                       TRUE)
endif()

message(STATUS "CMake configuration complete!")
