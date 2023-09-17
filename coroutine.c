#include "coroutine.h"
#include <stdint.h>

extern task_t current_task;
extern struct __jmp_buf_tag yield_env[1];

// void _default_error_handler(int retval, task_loop_t loop) {
//   fprintf(stderr,
//           "An error occured in task_loop(id = %d) with a return code %d.\n",
//           loop->id, retval);
// }

// inline void _loop_not_found(int id) {
//   fprintf(stderr, "Cannot find task_loop(id = %d). Exiting...\n", id);
//   exit(EXIT_FAILURE);
// }

task_loop_t create_task_loop(int id, error_handler_t error_handler) {
  task_loop_t result;
  result = malloc(sizeof(queue_s));
  if (result == NULL) {
    return NULL;
  }
  result->task_queue = createQueue(MAX_TASKS);
  if (result->task_queue == NULL) {
    return NULL;
  }
  result->id = id;
  result->error_handler = error_handler;
  
  return result;
}

int add_task(task_func_t task_func, task_loop_t loop) {
  struct timeval now;
  if (isFull(loop->task_queue)) {
    return 1;
  }
  gettimeofday(&now, NULL);
  task_t task = malloc(sizeof(struct task_s));
  task->task_func = task_func;
  task->next_time = now.tv_usec - 1;
  task->executed = false;
  enqueue(loop->task_queue, task);
  return 0;
}

inline void _run_loop(task_loop_t loop) {
  struct timeval now;
  task_t task;

  setjmp(yield_env);
  while (!isEmpty(loop->task_queue)) {
    gettimeofday(&now, NULL);
    task = (task_t)front(loop->task_queue);
    dequeue(loop->task_queue);
    enqueue(loop->task_queue, task);
    if (task->next_time == -1) {
      dequeue(loop->task_queue);
      continue;
    }
    if (now.tv_usec < task->next_time) {
      continue;
    }
    current_task = task;
    if (!task->executed) {
      task->executed = true;
      task->task_func();
    } else {
      longjmp(task->env, _CO_CONTINUE);
    }

    //   if (retval) {
    //     if (loop->error_handler != NULL) {
    //       loop->error_handler(retval, loop);
    //       continue;
    //     } else {
    //       _default_error_handler(retval, loop);
    //       continue;
    //     }
    //   }
  }
}

// inline void yield(int loop_id) {
//   task_t task;
//   task_loop_t loop;
//   for (int i = 0; i < loop_pool->size; i++) {
//     loop = loop_pool->loops[i];
//     if (loop->id == loop_id) {
//       _run_loop(loop);
//       return;
//     }
//   }
//   exit(EXIT_FAILURE);
// }

inline void sleep(int loop_id, int seconds) {}

inline void await(int loop_id, task_func_t task_func) {}

void run_loop(task_loop_t loop) { _run_loop(loop); }
