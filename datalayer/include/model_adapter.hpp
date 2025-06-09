#ifndef _DL_MODEL_ADAPTER_HPP_
#define _DL_MODEL_ADAPTER_HPP_

#include "datamodels.hpp"

enum model_adapter_type {
  QUEUE,
  MESSAGE,
  QUEUE_RELATION
};


template<typename T>
class ModelAdapter {
public:
  //ModelAdapter();
  //~ModelAdapter();
  virtual int create1(T* i_ent) = 0;
  virtual int read1(T* io_ent) = 0;
  virtual int update1(T* i_ent) = 0;
  virtual int del1(T* i_ent) = 0;
  //virtual int create(T** i_ent, int n) = 0;
  //virtual int read(T** io_ent, int n) = 0;
  //virtual int update(T** i_ent, int n) = 0;
  //virtual int del(T** i_ent, int n) = 0;

};

class QueueModelAdapter : ModelAdapter<queue_t> {
public:
  int create1(queue_t* i_ent) override;
  int read1(queue_t* io_ent) override;
  int update1(queue_t* i_ent) override;
  int del1(queue_t* i_ent) override;
};

class MessageModelAdapter : ModelAdapter<message_t> {
public:
  int create1(message_t* i_ent) override;
  int read1(message_t* io_ent) override;

  int read_in_queue(queue_pk_t qpk, int* limit, int offset, message_t** o_mesgs);

  int update1(message_t* i_ent) override;
  int del1(message_t* i_ent) override;
};

class QueueRelationModelAdapter : ModelAdapter<queue_rel_t> {
public:
  int create1(queue_rel_t* i_ent) override;
  int read1(queue_rel_t* io_ent) override;

private:
  int read_queue_rels(queue_pk_t qpk, bool main, int* limit, int offset, queue_rel_t** o_qrels);

public:
  int read_rels_of_queue(queue_pk_t qpk, int* limit, int offset, queue_rel_t** o_qrels){ return read_queue_rels(qpk, true, limit, offset, o_qrels); }
  int read_rels_to_queue(queue_pk_t qpk, int* limit, int offset, queue_rel_t** o_qrels){ return read_queue_rels(qpk, false, limit, offset, o_qrels); }

  int update1(queue_rel_t* i_ent) override;
  int del1(queue_rel_t* i_ent) override;
};


#endif

