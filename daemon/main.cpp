#include <iostream>
#include "config.h"

#include "psql_db_manager.hpp"

int main(/*int argc, char** argv*/){
  std::cout << "Hello, welcome to " _PROG_NAME_ " v" _VERSION_MAJOR_STR_ "." _VERSION_MINOR_STR_ << std::endl;
  PsqlDbManager dbm;
  dbm.connect();
  dbm.disconnect();
  return 0;
}
