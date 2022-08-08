set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_subdirectory(services)

add_library(grpcclient
    GrpcClient.cpp
    QueryResponseHandler.cpp
    TransactionResponseHandler.cpp
    GrpcResponseHandler.cpp
    GrpcClient.hpp
    QueryResponseHandler.hpp
    TransactionResponseHandler.hpp
    GrpcResponseHandler.hpp
)

target_link_libraries(grpcclient
    grpc_channel_factory
    model_generators
    command_client
    query_client
)

target_include_directories(grpcclient PUBLIC
    ${PROJECT_SOURCE_DIR}/iroha-cpp
)

add_executable(iroha-cpp
    main.cpp
)

target_link_libraries(iroha-cpp
    clientservices
    logger_manager
)

add_install_step_for_bin(iroha-cpp)
