add_library(iroha_lib_model
    Query.cpp
    Tx.cpp
    TxBatch.cpp
    Query.hpp
    Tx.hpp
    TxBatch.hpp
    generators/CommandGenerator.cpp
    generators/CommandGenerator.hpp
    generators/QueryGenerator.cpp
    generators/QueryGenerator.hpp
    utils/Utils.h
    utils/Utils.cpp
    LibsAndClassDeclarations.h
    ClassDeclarations.h
)

target_link_libraries(iroha_lib_model
    grpc_client
)

target_include_directories(iroha_lib_model PUBLIC
    ${PROJECT_SOURCE_DIR}/iroha-lib
)
