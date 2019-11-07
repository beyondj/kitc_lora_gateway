add_library(loraapi SHARED lora.cpp)
target_link_libraries(loraapi pigpio rt pthread m)
