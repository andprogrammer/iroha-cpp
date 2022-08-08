add_library(clientservices
    Query.cpp
    Tx.cpp
    Status.cpp
    Request.cpp
    Query.hpp
    Tx.hpp
    Status.hpp
    Request.hpp
    LibsAndClassDeclarations.h
    ClassDeclarations.h
)

target_link_libraries(clientservices
    grpcclient
)

target_include_directories(clientservices PUBLIC
    ${PROJECT_SOURCE_DIR}/iroha-cpp
)
