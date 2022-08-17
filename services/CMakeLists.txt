add_library(clientservices
    Query.cpp
    Tx.cpp
    Status.cpp
    TxBatch.cpp
    Query.hpp
    Tx.hpp
    Status.hpp
    TxBatch.hpp
    LibsAndClassDeclarations.h
    ClassDeclarations.h
)

target_link_libraries(clientservices
    grpcclient
)

target_include_directories(clientservices PUBLIC
    ${PROJECT_SOURCE_DIR}/iroha-cpp
)
