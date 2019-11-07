add_executable(server_run server.cpp)
target_link_libraries(server_run dbapi loraapi epollwifiapi)
add_dependencies(server_run dbapi loraapi)

add_executable(client_run client.cpp)
target_link_libraries(client_run  loraapi macapi sensorapi)
add_dependencies(client_run dbapi loraapi)
