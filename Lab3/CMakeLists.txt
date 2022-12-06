
#
# Simple CMakeLists file for a Simlib project.
#

# Define the project name
#
set(PROJECT_NAME "run")

# Tell cmake the project name.
#
project(${PROJECT_NAME})

# Set the build type so that debugging can occur.
#
set(CMAKE_BUILD_TYPE "Debug")

# Set some typical compiler flags.
#
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# List all the source files after the add_executable line.
#
add_executable(${PROJECT_NAME}
  call_arrival.c
  call_departure.c
  call_duration.c
  cleanup.c
  main.c
  output.c
  simlib.c
  )

# Link with the math library.
#
target_link_libraries(${PROJECT_NAME} m) 




