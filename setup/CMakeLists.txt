add_executable(setup_psql psql_db_setup.cpp)

find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)

#target_compile_options(setup_psql PRIVATE -E)
target_link_libraries(setup_psql ${PQXX_LIB} ${PQ_LIB} datalayer)
