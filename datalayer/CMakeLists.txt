#Dont use glob, as it wont rebuild when sources are added
#file(GLOB datalayer_SRC "src/*.cpp")

add_library(datalayer STATIC src/psql_model_adapter.cpp src/psql_db_manager.cpp)

find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)

target_include_directories(datalayer PUBLIC include)
target_link_libraries(datalayer ${PQXX_LIB} ${PQ_LIB})
