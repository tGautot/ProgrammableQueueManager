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
  ModelAdapter();
  ~ModelAdapter();
  virtual int create(T* i_ent) = 0;
  virtual int read(T* io_ent) = 0;
  virtual int update(T* i_ent) = 0;
  virtual int del(T* i_ent) = 0;

};

class QueueModelAdapter : ModelAdapter<queue_t> {
  int create(queue_t* i_ent) override;
  int read(queue_t* io_ent) override;
  int update(queue_t* i_ent) override;
  int del(queue_t* i_ent) override;
};

class MessageModelAdatpter : ModelAdapter<message_t> {
  int create(message_t* i_ent) override;
  int read(message_t* io_ent) override;
  int update(message_t* i_ent) override;
  int del(message_t* i_ent) override;
};
class QueueRelationModelAdapter : ModelAdapter<queue_rel_t> {
  int create(queue_rel_t* i_ent) override;
  int read(queue_rel_t* io_ent) override;
  int update(queue_rel_t* i_ent) override;
  int del(queue_rel_t* i_ent) override;
};


#endif

