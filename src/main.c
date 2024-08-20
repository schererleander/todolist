#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_TASKS 256
#define DEBUG true

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

int main() {
  Task tasks[MAX_TASKS];
  int ch;
  int taskCount = 0;
  int selectedTask = 0;
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
    int row = getmaxy(stdscr);
    move(0,0);
    attron(A_BOLD);
    printw("Task Manager\n");
    attroff(A_BOLD);
    printTasks(tasks, taskCount, selectedTask);

    if(DEBUG) {
      move(row - 4, 0);
      printw("Selected task: %i", selectedTask);
      move(row - 3, 0);
      printw("Task Count: %i", taskCount);
    }

    move(row - 1, 0);
    printw("Press 'q' to quit, 'a' to add task and 'd' to delte task");

       if (selectedTask < 0) {
      selectedTask = 0;
    }

    ch = getch();
    switch (ch) {
      case 'q':
        quit = true;
        break;
      case KEY_UP:
        if (selectedTask > 0) {
          selectedTask--;
        }
        break;
      case KEY_DOWN:
        if (selectedTask < taskCount - 1) {
          selectedTask++;
        }
        break;
      case '\n':
        tasks[selectedTask].done = !tasks[selectedTask].done;
        break;
      case 'd':
        if (taskCount <= 0) {
          break;
        }
        for (int i = selectedTask; i < taskCount; i++) {
          tasks[i] = tasks[i + 1];
        }
        taskCount--;
        if (selectedTask >= taskCount) {
          selectedTask = taskCount - 1; // Adjust selectedTask if needed
        }
        break;
      case 'a':
        echo();
        char newTask[256];
        mvprintw(row -2, 0, "Enter task description: ");
        getnstr(newTask, 256);
        if (strlen(newTask) == 0 || strspn(newTask, " \t\n") == strlen(newTask)) {
          break;
        }
        if (taskCount < MAX_TASKS) {
          strcpy(tasks[taskCount].description, newTask);
          taskCount++;
        }
        noecho();
        break;
    }

  }
  endwin();
  return 0;

}
