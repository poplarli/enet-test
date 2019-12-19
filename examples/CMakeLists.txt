
ADD_EXECUTABLE(enet_client enet_client.cpp)
TARGET_LINK_LIBRARIES(enet_client pthread enet)

ADD_EXECUTABLE(enet_server enet_server.cpp)
TARGET_LINK_LIBRARIES(enet_server pthread enet)
