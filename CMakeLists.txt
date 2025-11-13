cmake_minimum_required(VERSION 3.3)
project(VideoEditor)

set(CMAKE_CXX_STANDARD 11) # no explicit compiler flags if possible
set(OpenGL_GL_PREFERENCE GLVND)
set(CMAKE_BUILD_TYPE Debug)

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLWF REQUIRED glfw3)
pkg_search_module(FREETYPE REQUIRED freetype2)
# find_package(glfw3 REQUIRED)
add_subdirectory(lib/ffmpeg)

include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${FREETYPE_INCLUDE_DIRS})
include_directories(. lib include)

# Collect source files for main application (recursively)
file(GLOB_RECURSE VideoEditor_SRC
     "src/*.cpp"
     "lib/*.cpp"
     "lib/glad/*.c"
)

message(abc ${VideoEditor_SRC})

# Main executable
add_executable(VideoEditor ${VideoEditor_SRC})
target_link_libraries(VideoEditor OpenGL::GL ${GLWF_LIBRARIES} ${FREETYPE_LIBRARIES} FFmpeg)

# ==============================================================================
# GTest Integration for Unit Testing
# ==============================================================================

# Download and configure Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Collect library sources (everything except main.cpp for test executable)
file(GLOB_RECURSE VideoEditor_LIB_SRC
     "src/*.cpp"
     "lib/*.cpp"
     "lib/glad/*.c"
)
# Remove main.cpp from library sources
list(FILTER VideoEditor_LIB_SRC EXCLUDE REGEX ".*main\\.cpp$")

# Collect test source files
file(GLOB VideoEditor_TEST_SRC
     "tests/*.cpp"
)

# Create test executable
add_executable(VideoEditorTests ${VideoEditor_TEST_SRC} ${VideoEditor_LIB_SRC})

# Link test executable with GTest and project dependencies
target_link_libraries(VideoEditorTests
    gtest_main
    OpenGL::GL
    ${GLWF_LIBRARIES}
    ${FREETYPE_LIBRARIES}
    FFmpeg
)

# Discover tests for CTest
include(GoogleTest)
gtest_discover_tests(VideoEditorTests)

