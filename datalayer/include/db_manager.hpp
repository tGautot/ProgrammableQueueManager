#ifndef _DL_DB_MANAGER_HPP_
#define _DL_DB_MANAGER_HPP_

#define DB_QUEUE_TABLE "queue"
#define DB_MESG_TABLE "mesg"
#define DB_QREL_TABLE "qrel"

class DbManager {
public:
  virtual int connect() = 0;
  virtual int disconnect() = 0;
  virtual int check_db_setup() = 0;
};


#endif

