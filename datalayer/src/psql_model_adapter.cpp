#include "model_adapter.hpp"
#include "datamodels.hpp"
#include "psql_db_manager.hpp"
#include "return_codes.hpp"

#include <pqxx/pqxx>
#include <iostream>

#define GET_CON(adpt_desc) pqxx::connection* con; \
  con = dbm.get_db_con(); \
  if(con == nullptr){ \
    std::cout << "Tried calling " #adpt_desc " without db connection " << __FILE__ << ":" << __LINE__ << std::endl;  \
    return DB_ERR_NO_CON; \
  }

static PsqlDbManager dbm;

int QueueModelAdapter::create(queue_t* i_q){
  GET_CON(QueueModelAdapter::create);
  pqxx::work tx(*con);

  return SUCCESS;
}

