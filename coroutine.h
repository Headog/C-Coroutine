#ifndef _COROUTINE_H_
#define _COROUTINE_H_

#include "queue.h"
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define _CO_CONTINUE 1

#define MAX_TASKS 5
#define DEFAULT_LOOP_SIZE 1

#define CO_YIELD                                                               \
  if (setjmp(current_task->env) == 0)                                          \
    longjmp(yield_env, _CO_CONTINUE);

// #define YIELD_TO(X) longjmp(yield_env, (X))

#define CO_INIT                                                                \
  task_t current_task;                                                         \
  struct __jmp_buf_tag yield_env[1];

#define CO_END                                                                 \
  current_task->next_time = -1;                                                \
  longjmp(yield_env, 1);

typedef struct task_loop_s *task_loop_t;
typedef struct task_s *task_t;
typedef struct loop_pool_s *loop_pool_t;
typedef void (*task_func_t)(void);
typedef void (*error_handler_t)(int, task_loop_t);

struct task_loop_s {
  int id;
  queue_t task_queue;
  error_handler_t error_handler;
};

struct task_s {
  task_func_t task_func;
  struct __jmp_buf_tag env[1];
#ifdef __USE_TIME_BITS64
  __suseconds64_t
#else
  __suseconds_t next_time;
  bool executed;
#endif
};

struct loop_pool_s {
  task_loop_t *loops;
  int max_size;
  int size;
};

// Function declarations
task_loop_t create_task_loop(int id, error_handler_t error_handler);
int add_task(task_func_t task_func, task_loop_t loop);
void run_loop(task_loop_t loop);
void yield();
void sleep(int loop_id, int seconds);
void await(int loop_id, task_func_t task_func);

void _run_loop(task_loop_t loop);
void _default_error_handler(int retval, task_loop_t loop);

#endif // _COROUTINE_H_