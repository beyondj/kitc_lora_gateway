add_executable(server_run server.cpp)
target_link_libraries(server_run macapi dbapi loraapi epollwifiapi)
add_dependencies(server_run dbapi loraapi)

add_executable(client_run client.cpp)
target_link_libraries(client_run  loraapi macapi sensorapi)
add_dependencies(client_run dbapi loraapi)

add_executable(nbiot_gateway_run gateway_nbiot.cpp)
target_link_libraries(nbiot_gateway_run macapi dbapi loraapi epollwifiapi nbiotapi pipeapi)
add_dependencies(nbiot_gateway_run dbapi loraapi)

add_executable(mimicErrorclient_run mimicclientWithError.cpp)
target_link_libraries(mimicErrorclient_run  loraapi macapi sensorapi)
add_dependencies(mimicErrorclient_run dbapi loraapi)

add_executable(pnbiot_server_run public_nbiot_server.cpp)
target_link_libraries(pnbiot_server_run  loraapi macapi sensorapi)
