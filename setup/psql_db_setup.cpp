#include "psql_db_manager.hpp" 
#include "model_adapter.hpp"
#include "datamodels.hpp"
#include <iostream>
#include <cstring>

int main(int argc, char** argv){
  
  PsqlDbManager dbm;
  dbm.connect();
  
  pqxx::work tx(*(dbm.get_db_con()));
  
  if(argc >= 2 && strcmp(argv[1], "-d") == 0){
    std::cout << "-d specified, are you sure you want to drop all tables? [YN]: ";
    char ans;
    std::cin >> ans;
    if(ans != 'y' && ans != 'Y'){
      std::cout << "Aborting..." << std::endl;
      dbm.disconnect();
      return 0;
    } 

    tx.exec("DROP TABLE IF EXISTS " DB_QUEUE_TABLE "," DB_MESG_TABLE "," DB_QREL_TABLE);
    tx.commit();
    dbm.disconnect();
    return 0;
  }


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
  tx.exec(queueTblCreationQuery);
  std::cout << "Creating mesg table via statement:\n\t" << msgTblCreationQuery << std::endl;
  tx.exec(msgTblCreationQuery);
  std::cout << "Creating qrel table via statement:\n\t" << qrelTblCreationQuery << std::endl;
  tx.exec(qrelTblCreationQuery);
  tx.commit();

  queue_t def_in, def_out;
  def_in.name = "IN";
  def_in.maxMessages = 1000;
  def_in.type = queue_types_t::INPUT;
  def_in.proc_type = queue_processing_types_t::MAN;

  def_out.name = "OUT";
  def_out.maxMessages = 1000;
  def_out.type = queue_types_t::OUTPUT;
  def_out.proc_type = queue_processing_types_t::MAN;

  QueueModelAdapter qma;
  qma.create1(&def_in);
  qma.create1(&def_out);

  dbm.disconnect();
  return 0;
}
