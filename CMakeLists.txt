set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_subdirectory(model)

add_library(grpc_client
    GrpcClient.cpp
    GrpcClient.hpp
)

target_link_libraries(grpc_client
    grpc_channel_factory
    model_generators
    command_client
    query_client
)

target_include_directories(grpc_client PUBLIC
    ${PROJECT_SOURCE_DIR}/iroha-lib
)

add_executable(tx_example
    TxExample.cpp
)

target_link_libraries(tx_example
    iroha_lib_model
    logger_manager
)

add_install_step_for_bin(tx_example)

add_executable(batch_example
    BatchExample.cpp
)

target_link_libraries(batch_example
    iroha_lib_model
    logger_manager
)

add_install_step_for_bin(batch_example)
