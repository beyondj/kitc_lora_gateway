add_executable(lora_gateway_run lora_gateway.cpp)
target_link_libraries(lora_gateway_run macapi dbapi loraapi epollwifiapi)
add_dependencies(lora_gateway_run dbapi loraapi)

add_executable(lora_sensor_run lora_sensor.cpp)
target_link_libraries(lora_sensor_run  loraapi macapi sensorapi)
add_dependencies(lora_sensor_run dbapi loraapi)

add_executable(hybrid_gateway_run hybrid_gateway.cpp)
target_link_libraries(hybrid_gateway_run macapi dbapi loraapi epollwifiapi nbiotapi pipeapi)
add_dependencies(hybrid_gateway_run dbapi loraapi)

add_executable(mimicErrorclient_run mimicclientWithError.cpp)
target_link_libraries(mimicErrorclient_run  loraapi macapi sensorapi)
add_dependencies(mimicErrorclient_run dbapi loraapi)

add_executable(public_server_run public_server.cpp)
target_link_libraries(public_server_run  loraapi macapi sensorapi)
