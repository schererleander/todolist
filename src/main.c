#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_TASKS 256

typedef struct {
  char description[256];
  bool done;
} Task;

void printTasks(Task tasks[], int taskCount, int selectedTask) {
  for (int i = 0; i < taskCount; i++) {
    if (i == selectedTask) {
      attron(A_REVERSE);
    }
    if(!tasks[i].done) {
      printw("%s\n", tasks[i].description);
    } else {
      printw("✓ %s\n", tasks[i].description);
    }
    if (i == selectedTask) {
      attroff(A_REVERSE);
    }
  }
}

void deleteTask(Task tasks[], int *taskCount, int selectedTask) {
  if(*taskCount <= 0) {
    return;
  }

  for (int i = selectedTask; i < *taskCount; i++) {
    tasks[i] = tasks[i + 1];
  }
  (*taskCount)--;
}

int compareTasks(const void *a, const void *b) {
  Task *taskA = (Task *)a;
  Task *taskB = (Task *)b;
  return (taskA->done - taskB->done);
}

int main() {
  Task tasks[MAX_TASKS];
  int ch;
  int taskCount = 0;
  int selectedTask = 0;
  int row;
  bool quit = false;

  initscr();
  cbreak();
  noecho();
  setlocale(LC_CTYPE, "");
  keypad(stdscr, TRUE);

  strcpy(tasks[0].description, "Do laundry");
  strcpy(tasks[1].description, "Clean room");
  strcpy(tasks[2].description, "Buy groceries");
  tasks[2].done = true;
  taskCount = 3;


  while(!quit) {
    clear();
    move(0,0);
    attron(A_BOLD);
    printw("Task Manager\n");
    attroff(A_BOLD);
    printTasks(tasks, taskCount, selectedTask);
    row = getmaxy(stdscr);

    move(row - 1, 0);
    printw("Press 'q' to quit and 'a' to add task");

    qsort(tasks, taskCount, sizeof(Task), compareTasks);

    ch = getch();
    switch (ch) {
      case 'q':
        quit = true;
        break;
      case KEY_UP:
        selectedTask--;
        if (selectedTask < 0) {
          selectedTask = 0;
        }
        break;
      case KEY_DOWN:
        selectedTask++;
        if (selectedTask > taskCount) {
          selectedTask = taskCount;
        }
        break;
      case '\n':
        tasks[selectedTask].done = !tasks[selectedTask].done;
        break;
      case KEY_DC:
        deleteTask(tasks, &taskCount, selectedTask);
        if (selectedTask >= taskCount) {
          selectedTask = taskCount - 1; // Adjust selectedTask if needed
        }
        break;
    }

  }
  endwin();
  return 0;

}
