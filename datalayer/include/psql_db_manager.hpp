#ifndef _DL_PSQL_DB_MANAGER_HPP_
#define _DL_PSQL_DB_MANAGER_HPP_

#include "db_manager.hpp"

#include <pqxx/pqxx>

class PsqlDbManager : DbManager {
private:
  static pqxx::connection* con;

public:
  int connect() override;
  int disconnect() override;
  int check_db_setup() override;

  static pqxx::connection* get_db_con();
};


#endif

