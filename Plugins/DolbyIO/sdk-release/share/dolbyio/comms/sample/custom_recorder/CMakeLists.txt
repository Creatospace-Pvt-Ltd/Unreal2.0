cmake_minimum_required(VERSION 3.0...3.21)

add_library(custom_recorder STATIC
	custom_recorder.h
	custom_recorder.cc
)

target_link_libraries(custom_recorder
	DolbyioComms::sdk
)

# Enable the deprecated wait
add_compile_definitions(DOLBYIO_COMMS_ENABLE_DEPRECATED_WAIT)
