#include "coroutine.h"
#include <errno.h>
#include <stdlib.h>

CO_INIT

void task1() {
  puts("task1 begin");
  CO_YIELD;
  puts("task1 end");
  CO_END;
}

void task2() {
  puts("task2 begin");
  CO_YIELD;
  puts("task2 middle");
  CO_YIELD;
  puts("task2 end");
  CO_END;
}

void task3() {
  puts("task3 begin");
  CO_YIELD;
  puts("task3 end");
  CO_END;
}

void task4() {
  puts("task4 begin");
  CO_YIELD;
  puts("task4 end");
  CO_END;
}

int main(void) {
  task_loop_t loop = NULL;
  loop = create_task_loop(0, NULL);
  if (loop == NULL) {
    exit(EXIT_FAILURE);
  }
  add_task(task1, loop);
  add_task(task2, loop);
  add_task(task3, loop);
  add_task(task4, loop);
  run_loop(loop);
  return 0;
}
