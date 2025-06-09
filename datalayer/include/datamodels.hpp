#ifndef _DL_DATAMODELS_HPP_
#define _DL_DATAMODELS_HPP_


#include <cstdint>
#include <string>
#include <string>
#include <cstddef>

// ===== QUEUE

enum class queue_types_t {
  INPUT, OUTPUT, MIDDLE
};

enum class queue_processing_types_t {
  STP, // Straight Through 
  MAN  // Manual
};

#define DB_QUEUE_TABLE "queue"
#define DM_QUEUE_NAME_MAX_LEN 100
#define DM_QUEUE_NAME_MAX_LEN_STR "100"
#define DM_QUEUE_COL_NAME "name"
#define DM_QUEUE_COL_MAX_MSG "max_mesg"
#define DM_QUEUE_COL_TYPE "type"
#define DM_QUEUE_COL_PROC_TYPE "proc_type"

typedef std::string queue_pk_t;

typedef struct {
  queue_pk_t name; // PK
  int32_t maxMessages;
  queue_types_t type;
  queue_processing_types_t proc_type;
} queue_t;



// ===== MESSAGE
#define DB_MESG_TABLE "mesg"
#define DM_MESG_NAME_MAX_LEN 255
#define DM_MESG_NAME_MAX_LEN_STR "255"
#define DM_MESG_COL_ID "id"
#define DM_MESG_COL_NAME "name"
#define DM_MESG_COL_PAYLOAD "payload"
#define DM_MESG_COL_INPT_DATE "input_date"
#define DM_MESG_COL_UPDT_DATE "update_date"
#define DM_MESG_COL_QUEUE "curr_queue"

typedef int64_t mesg_pk_t;

typedef struct {
  mesg_pk_t id; // PK
  std::string name;//[DM_MESG_NAME_MAX_LEN];
  std::basic_string<std::byte> payload;
  std::string input_dt; // Timestamp
  std::string update_dt; // Timestamp
  queue_pk_t curr_queue;
} message_t;





// ===== QUEUE RELATIONS

enum class qrel_type_t {
  BLACKLIST,
  WHITELIST
};

enum class qrel_way_t {
  IN, OUT
};

#define DB_QREL_TABLE "qrel"
#define DM_QREL_COL_ID "id"
#define DM_QREL_COL_MAIN_QUEUE "main_queue"
#define DM_QREL_COL_OTHR_QUEUE "othr_queue"
#define DM_QREL_COL_TYPE "type"
#define DM_QREL_COL_WAY "way"

typedef int qrel_pk_t;

typedef struct {
  qrel_pk_t id; // PK
  queue_pk_t main_queue;
  queue_pk_t other_queue;
  qrel_type_t rel_type;
  qrel_way_t rel_way;
} queue_rel_t;




#endif

