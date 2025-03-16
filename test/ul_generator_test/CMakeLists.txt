# Link main file to executable name
set(testName "ul_generator_test")
add_executable(${testName} ${testName}.cpp)

# Add required libraries and include directories
target_include_directories(${testName} PRIVATE "." "${CMAKE_SOURCE_DIR}/main/include" $ENV{CADMIUM})

# Set c++ standard
target_compile_options(${testName} PUBLIC -std=gnu++2b)
