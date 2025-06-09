#include "psql_db_manager.hpp"
#include "datamodels.hpp"
#include "return_codes.hpp"
#include <pqxx/pqxx>
#include <iostream>

pqxx::connection* PsqlDbManager::con = nullptr;

int PsqlDbManager::connect(){
  if(con == nullptr){
    // TODO use env vars
    con = new pqxx::connection("postgresql://postgres:abcdefg@localhost/testdb");
    std::cout << "Connected to db " << con->dbname() << "on port " << con->port() << std::endl;
  } else {
    std::cout << "Tried connecting psql db, but already connected" << std::endl;
  }
  return SUCCESS;
}

int PsqlDbManager::disconnect(){
  if(con != nullptr){
    con->close();
    delete con;
    con = nullptr;
  } else {
    std::cout << "Tried disconnecting psql db, but isn't connected" << std::endl;
  }
  return SUCCESS;
}

int PsqlDbManager::check_db_setup(){
  bool has_queue_tbl = false, has_msg_tbl = false, has_qrel_tbl = false;
  pqxx::connection* con = get_db_con();
  if(con == nullptr){  
    return DB_ERR_NO_CON;
  }
  pqxx::work tx(*con);
  pqxx::result all_tbls = tx.exec("SELECT tablename FROM pg_tables");
  for(int rid = 0; rid < all_tbls.size(); rid++){
    std::string tname = all_tbls[rid][0].as<std::string>();
    if(!has_queue_tbl && tname.compare(DB_QUEUE_TABLE) == 0){
      has_queue_tbl = true;
    } else if(!has_msg_tbl && tname.compare(DB_MESG_TABLE) == 0){
      has_msg_tbl = true;
    } else if(!has_qrel_tbl && tname.compare(DB_QREL_TABLE) == 0){
      has_qrel_tbl = true;
    } 
  }

  if(!has_qrel_tbl || !has_msg_tbl || !has_queue_tbl){
    std::cout << "DB is missing some tables, make sure you properly set ip up via init script\n"
      << (has_queue_tbl ? "" : "\tMissing table '" DB_QUEUE_TABLE "'\n")
      << (has_qrel_tbl ? "" : "\tMissing table '" DB_QREL_TABLE "'\n")
      << (has_msg_tbl ? "" : "\tMissing table '" DB_MESG_TABLE "'\n")
      << std::endl;
    return DB_BAD_SETUP;
  }

  // TODO if has all tables, also check for columns

  return SUCCESS;
}

pqxx::connection* PsqlDbManager::get_db_con(){
  return con;
}
