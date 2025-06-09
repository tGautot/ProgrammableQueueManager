#include "model_adapter.hpp"
#include "datamodels.hpp"
#include "psql_db_manager.hpp"
#include "return_codes.hpp"

#include <pqxx/pqxx>
#include <iostream>
#include <string>
#include <cstdint>
#include <cstddef>

#define GET_CON(adpt_desc) pqxx::connection* con; \
  con = dbm.get_db_con(); \
  if(con == nullptr){ \
    std::cout << "Tried calling " #adpt_desc " without db connection " << __FILE__ << ":" << __LINE__ << std::endl;  \
    return DB_ERR_NO_CON; \
  }

static PsqlDbManager dbm;



int QueueModelAdapter::create1(queue_t* i_q){
  GET_CON(QueueModelAdapter::create);
  static bool stt_prep = false;
  if(!stt_prep){
    con->prepare("insert1_queue", "INSERT INTO " DB_QUEUE_TABLE 
      "(" DM_QUEUE_COL_NAME "," DM_QUEUE_COL_MAX_MSG "," DM_QUEUE_COL_TYPE "," DM_QUEUE_COL_PROC_TYPE ")"
      " VALUES ($1, $2, $3, $4);");
    stt_prep = true;
  }
  
  pqxx::work tx(*con);

  tx.exec_prepared("insert1_queue", i_q->name, i_q->maxMessages, static_cast<int>(i_q->type), static_cast<int>(i_q->proc_type));
  tx.commit();

  return SUCCESS;
}

int QueueModelAdapter::read1(queue_t* ioq){
  GET_CON(QueueModelAdapter::create);
  static bool stt_prep = false;
  if(!stt_prep){
    con->prepare("read1_queue", "SELECT " DM_QUEUE_COL_MAX_MSG "," DM_QUEUE_COL_TYPE "," DM_QUEUE_COL_PROC_TYPE    
        " FROM " DB_QUEUE_TABLE " WHERE " DM_QUEUE_COL_NAME " = $1;");
    // This is only read1, no need for wildcard
    //con->prepare("read_queue_wc", "SELECT * FROM " DB_QUEUE_TABLE " WHERE " DM_QUEUE_COL_NAME " LIKE $1");
    stt_prep = true;
  }
  
  pqxx::work tx(*con);

  pqxx::row r = tx.exec_prepared1("read1_queue", ioq->name);
  ioq->maxMessages = r[0].as<int32_t>();
  ioq->type = static_cast<queue_types_t>(r[1].as<int/*queue_types_t*/>());
  ioq->proc_type = static_cast<queue_processing_types_t>(r[2].as<int/*queue_processing_types_t*/>());

  tx.commit();

  return SUCCESS;
}

int QueueModelAdapter::update1(queue_t* iq){
  GET_CON(QueueModelAdapter::create);
  static bool stt_prep = false;
  if(!stt_prep){
    con->prepare("update1_queue", "UPDATE " DB_QUEUE_TABLE 
      " SET " DM_QUEUE_COL_MAX_MSG " = $1 "
      " SET " DM_QUEUE_COL_TYPE " = $2 "
      " SET " DM_QUEUE_COL_PROC_TYPE " = $3 "
      " WHERE " DM_QUEUE_COL_NAME " = $4");
    stt_prep = true;
  }
  
  pqxx::work tx(*con);

  tx.exec_prepared("update1_queue", iq->maxMessages, static_cast<int>(iq->type), static_cast<int>(iq->proc_type), iq->name);
  tx.commit();

  return SUCCESS;
}

int QueueModelAdapter::del1(queue_t* iq){
  GET_CON(QueueModelAdapter::create);
  static bool stt_prep = false;
  if(!stt_prep){
    con->prepare("del1_queue", "DELETE FROM " DB_QUEUE_TABLE 
      " WHERE " DM_QUEUE_COL_NAME " = $1");
    stt_prep = true;
  }

  pqxx::work tx(*con);
  tx.exec_prepared("del1_queue", iq->name);
  tx.commit();
  return SUCCESS;
}


/**
 * Adapter for message_t
 */


int MessageModelAdapter::create1(message_t* im){
  GET_CON(MessageModelAdapter::create1);
  static bool stt_prep = false;
  if(!stt_prep){
    con->prepare("insert1_mesg", "INSERT INTO " DB_MESG_TABLE 
      "(" DM_MESG_COL_ID "," DM_MESG_COL_NAME "," DM_MESG_COL_PAYLOAD "," DM_MESG_COL_INPT_DATE "," DM_MESG_COL_UPDT_DATE ")"
      " VALUES (DEFAULT, $1, $2, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP);");
    stt_prep = true;
  }
  
  pqxx::work tx(*con);

  tx.exec_prepared("insert1_mesg", im->name, im->payload);
  tx.commit();

  return SUCCESS;
}

void mesg_from_row(pqxx::row& r, message_t* o_mesg){
  o_mesg->id = r[0].as<mesg_pk_t>();
  o_mesg->name = r[1].as<std::string>();
  o_mesg->payload = r[2].as<std::basic_string<std::byte>>();
  o_mesg->input_dt = r[3].as<std::string>();
  o_mesg->update_dt = r[4].as<std::string>();
  o_mesg->curr_queue = r[5].as<queue_pk_t>();
}

int MessageModelAdapter::read1(message_t* iom){
  GET_CON(MessageModelAdapter::read1);
  static bool stt_prep = false;
  if(!stt_prep){
    con->prepare("read1_mesg", "SELECT *" //DM_MESG_COL_ID "," DM_MESG_COL_PAYLOAD "," DM_MESG_COL_INPT_DATE "," DM_MESG_COL_UPDT_DATE "," DM_MESG_COL_QUEUE    
        " FROM " DB_MESG_TABLE " WHERE " DM_MESG_COL_ID " = $1;");
    // This is only read1, no need for wildcard
    //con->prepare("read_queue_wc", "SELECT * FROM " DB_QUEUE_TABLE " WHERE " DM_QUEUE_COL_NAME " LIKE $1");
    stt_prep = true;
  }
  
  pqxx::work tx(*con);

  pqxx::row r = tx.exec_prepared1("read1_mesg", iom->id);
  mesg_from_row(r, iom);
  tx.commit();

  return SUCCESS;
}

int MessageModelAdapter::read_in_queue(queue_pk_t qpk, int* limit, int offset, message_t** o_mesgs){
  GET_CON(MessageModelAdapter::read1);
  static bool stt_prep = false;
  if(!stt_prep){
    con->prepare("read_mesg_in_queue", "SELECT *"     
        " FROM " DB_MESG_TABLE " WHERE " DM_MESG_COL_QUEUE " = $1 LIMIT $2 OFFSET $3");
    // This is only read1, no need for wildcard
    //con->prepare("read_queue_wc", "SELECT * FROM " DB_QUEUE_TABLE " WHERE " DM_QUEUE_COL_NAME " LIKE $1");
    stt_prep = true;
  }
  
  pqxx::work tx(*con);

  pqxx::result res = tx.exec_prepared("read_mesg_in_queue", qpk, *limit, offset);
  int nrows = res.size();
  *o_mesgs = new message_t[nrows];
  *limit = nrows;
  for(int rowid = 0; rowid < nrows; rowid++){
    pqxx::row r = res[rowid];
    mesg_from_row(r, &((*o_mesgs)[rowid]) );
  }
  tx.commit();

  return SUCCESS;
}




int MessageModelAdapter::update1(message_t* im){
  GET_CON(MessageModelAdapter::update1);
  static bool stt_prep = false;
  if(!stt_prep){
    con->prepare("update1_mesg", "UPDATE " DB_MESG_TABLE 
      " SET " DM_MESG_COL_NAME " = $1 "
      " SET " DM_MESG_COL_PAYLOAD " = $2 "
      " SET " DM_MESG_COL_UPDT_DATE " = CURRENT_TIMESTAMP "
      " SET " DM_MESG_COL_QUEUE " = $3"
      " WHERE " DM_MESG_COL_ID " = $4");
    stt_prep = true;
  }
  
  pqxx::work tx(*con);

  tx.exec_prepared("update1_mesg", im->name, pqxx::binary_cast(im->payload), im->curr_queue, im->id);
  tx.commit();

  return SUCCESS;
}

int MessageModelAdapter::del1(message_t* im){
  GET_CON(QueueModelAdapter::create);
  static bool stt_prep = false;
  if(!stt_prep){
    con->prepare("del1_mesg", "DELETE FROM " DB_MESG_TABLE 
      " WHERE " DM_MESG_COL_ID " = $1");
    stt_prep = true;
  }

  pqxx::work tx(*con);
  tx.exec_prepared("del1_mesg", im->id);
  tx.commit();
  return SUCCESS;
}



/**
 * Adapter for queue_rel_t
 */


int QueueRelationModelAdapter::create1(queue_rel_t* iqrel){
  GET_CON(QueueRelationModelAdapter::create1);
  static bool stt_prep = false;
  if(!stt_prep){
    con->prepare("insert1_qrel", "INSERT INTO " DB_QREL_TABLE 
      "(" DM_QREL_COL_ID "," DM_QREL_COL_MAIN_QUEUE "," DM_QREL_COL_OTHR_QUEUE "," DM_QREL_COL_TYPE "," DM_QREL_COL_WAY ")"
      " VALUES (DEFAULT, $1, $2, $3, $4);");
    stt_prep = true;
  }
  
  pqxx::work tx(*con);

  tx.exec_prepared("insert1_qrel", iqrel->main_queue, iqrel->other_queue, static_cast<int>(iqrel->rel_type), static_cast<int>(iqrel->rel_way));
  tx.commit();

  return SUCCESS;
}


void qrel_from_row(pqxx::row& r, queue_rel_t* o_qrel){
  o_qrel->id = r[0].as<qrel_pk_t>();
  o_qrel->main_queue = r[1].as<std::string>();
  o_qrel->other_queue = r[2].as<std::string>();
  o_qrel->rel_type = static_cast<qrel_type_t>(r[3].as<int/*qrel_type_t*/>());
  o_qrel->rel_way = static_cast<qrel_way_t>(r[4].as<int/*qrel_way_t*/>());
}

int QueueRelationModelAdapter::read1(queue_rel_t* ioqr){
  GET_CON(QueueRelationModelAdapter::read1);
  static bool stt_prep = false;
  if(!stt_prep){
    con->prepare("read1_qrel", "SELECT *"
        " FROM " DB_QREL_TABLE " WHERE " DM_QREL_COL_ID " = $1;");
    stt_prep = true;
  }
  
  pqxx::work tx(*con);

  pqxx::row r = tx.exec_prepared1("read1_qrel", ioqr->id);
  qrel_from_row(r, ioqr);
  tx.commit();

  return SUCCESS;
}

int QueueRelationModelAdapter::read_queue_rels(queue_pk_t qpk, bool main, int* limit, int offset, queue_rel_t** o_qrels){
  GET_CON(QueueRelationModelAdapter::read1);
  static bool stt_prep = false;
  if(!stt_prep){
    con->prepare("read_qrel_of_queue", "SELECT *"
        " FROM " DB_QREL_TABLE " WHERE " DM_QREL_COL_MAIN_QUEUE " = $1 LIMIT $2 OFFSET $3;");
    con->prepare("read_qrel_to_queue", "SELECT *"
        " FROM " DB_QREL_TABLE " WHERE " DM_QREL_COL_OTHR_QUEUE " = $1 LIMIT $2 OFFSET $3;");
    stt_prep = true;
  }

  pqxx::work tx(*con);

  pqxx::result res = tx.exec_prepared( (main ? "read_qrel_of_queue" : "read_qrel_to_queue") , qpk, *limit, offset);
  int nrows = res.size();
  *o_qrels = new queue_rel_t[nrows];
  *limit = nrows;
  for(int i = 0; i < nrows; i++){
    pqxx::row r = res[i];
    qrel_from_row(r, &((*o_qrels)[i]) );
  }

  return SUCCESS;
}


int QueueRelationModelAdapter::update1(queue_rel_t* iqr){
  GET_CON(MessageModelAdapter::update1);
  static bool stt_prep = false;
  if(!stt_prep){
    con->prepare("update1_qrel", "UPDATE " DB_QREL_TABLE 
      " SET " DM_QREL_COL_MAIN_QUEUE " = $1 "
      " SET " DM_QREL_COL_OTHR_QUEUE " = $2 "
      " SET " DM_QREL_COL_TYPE " = $3 "
      " SET " DM_QREL_COL_WAY " = $4 "
      " WHERE " DM_MESG_COL_ID " = $5");
    stt_prep = true;
  }
  
  pqxx::work tx(*con);

  tx.exec_prepared("update1_qrel", iqr->main_queue, iqr->other_queue, static_cast<int>(iqr->rel_type), static_cast<int>(iqr->rel_way), iqr->id); 
  tx.commit();

  return SUCCESS;
}

int QueueRelationModelAdapter::del1(queue_rel_t* iqr){
  GET_CON(QueueModelAdapter::del1);
  static bool stt_prep = false;
  if(!stt_prep){
    con->prepare("del1_qrel", "DELETE FROM " DB_QREL_TABLE 
      " WHERE " DM_QREL_COL_ID " = $1");
    stt_prep = true;
  }

  pqxx::work tx(*con);
  tx.exec_prepared("del1_qrel", iqr->id);
  tx.commit();
  return SUCCESS;
}

