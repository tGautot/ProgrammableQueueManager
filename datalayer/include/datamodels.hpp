#ifndef _DL_DATAMODELS_HPP_
#define _DL_DATAMODELS_HPP_

#include <cstdint>
#include <string>


// ===== QUEUE

enum queue_types_t {
  INPUT, OUTPUT, MIDDLE
};

enum queue_processing_types_t {
  STP, // Straight Through 
  MAN  // Manual
};

#define DM_QUEUE_NAME_MAX_LEN 100
#define DM_QUEUE_NAME_MAX_LEN_STR "100"
#define DM_QUEUE_COL_NAME "name"
#define DM_QUEUE_COL_MAX_MSG "max_mesg"
#define DM_QUEUE_COL_TYPE "type"
#define DM_QUEUE_COL_PROC_TYPE "proc_type"

typedef struct {
  std::string name; // PK
  int64_t maxMessages;
  queue_types_t type;
  queue_processing_types_t proc_type;
} queue_t;



// ===== MESSAGE

#define DM_MESG_NAME_MAX_LEN 255
#define DM_MESG_NAME_MAX_LEN_STR "255"
#define DM_MESG_COL_ID "id"
#define DM_MESG_COL_NAME "name"
#define DM_MESG_COL_PAYLOAD "payload"
#define DM_MESG_COL_INPT_DATE "input_date"
#define DM_MESG_COL_UPDT_DATE "update_date"
#define DM_MESG_COL_QUEUE "curr_queue"
typedef struct {
  int64_t id; // PK
  char name[DM_MESG_NAME_MAX_LEN];
  char* payload;
  uint64_t pl_size;
  std::string input_dt; // Timestamp
  std::string update_dt; // Timestamp
  queue_t* curr_queue;
} message_t;





// ===== QUEUE RELATIONS

enum queue_relationship_type_t {
  BLACKLIST,
  WHITELIST
};

enum queue_relationship_way_t {
  IN, OUT
};

#define DM_QREL_COL_ID "id"
#define DM_QREL_COL_MAIN_QUEUE "main_queue"
#define DM_QREL_COL_OTHR_QUEUE "othr_queue"
#define DM_QREL_COL_TYPE "type"
#define DM_QREL_COL_WAY "way"
typedef struct {
  int32_t id; // PK
  queue_t* main_queue;
  queue_t* other_queue;
  queue_relationship_type_t rel_type;
  queue_relationship_way_t rel_way;
} queue_rel_t;




#endif

