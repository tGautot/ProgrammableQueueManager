#include "psql_db_manager.hpp" 
#include "datamodels.hpp"
#include <iostream>

int main(/*int argc, char** argv*/){
  PsqlDbManager dbm;
  dbm.connect();

  pqxx::work tx(*(dbm.get_db_con()));

  std::string queueTblCreationQuery = "CREATE TABLE " DB_QUEUE_TABLE " ("
      DM_QUEUE_COL_NAME " VARCHAR(" DM_QUEUE_NAME_MAX_LEN_STR ") PRIMARY KEY,"
      DM_QUEUE_COL_MAX_MSG " INT,"
      DM_QUEUE_COL_TYPE " INT,"
      DM_QUEUE_COL_PROC_TYPE " INT"
      ");";

  std::string qrelTblCreationQuery = "CREATE TABLE " DB_QREL_TABLE " ("
      DM_QREL_COL_ID " BIGSERIAL PRIMARY KEY,"
      DM_QREL_COL_WAY " INT,"
      DM_QREL_COL_TYPE " INT,"
      DM_QREL_COL_MAIN_QUEUE " VARCHAR(" DM_QUEUE_NAME_MAX_LEN_STR ") REFERENCES " DB_QUEUE_TABLE "(" DM_QUEUE_COL_NAME "),"
      DM_QREL_COL_OTHR_QUEUE " VARCHAR(" DM_QUEUE_NAME_MAX_LEN_STR ") REFERENCES " DB_QUEUE_TABLE "(" DM_QUEUE_COL_NAME ")"
      ");";


  std::string msgTblCreationQuery = "CREATE TABLE " DB_MESG_TABLE " ("
      DM_MESG_COL_ID " BIGSERIAL PRIMARY KEY,"
      DM_MESG_COL_NAME " VARCHAR(" DM_MESG_NAME_MAX_LEN_STR "),"
      DM_MESG_COL_PAYLOAD " BYTEA,"
      DM_MESG_COL_INPT_DATE " TIMESTAMP,"
      DM_MESG_COL_UPDT_DATE " TIMESTAMP,"
      DM_MESG_COL_QUEUE " VARCHAR(" DM_QUEUE_NAME_MAX_LEN_STR ") REFERENCES " DB_QUEUE_TABLE "(" DM_QUEUE_COL_NAME ")"
      ");";

  std::cout << "Creating queue table via statement:\n\t" << queueTblCreationQuery << std::endl;
  tx.query(queueTblCreationQuery);
  
  std::cout << "Creating mesg table via statement:\n\t" << msgTblCreationQuery << std::endl;
  tx.query(msgTblCreationQuery);
   std::cout << "Creating qrel table via statement:\n\t" << qrelTblCreationQuery << std::endl;
  tx.query(qrelTblCreationQuery);


  tx.commit();

  return 0;
}
