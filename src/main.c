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

Task tasks[MAX_TASKS];
int taskCount = 0;
int selectedTask = 0;

void printTasks() {
  move(1,0);
  for (int i = 0; i < taskCount; i++) {
    if (i == selectedTask) {
      attron(A_REVERSE);
    }
    if (tasks[i].done) {
      attron(COLOR_PAIR(1) | A_BOLD);
    }
    printw("%s\n", tasks[i].description);
    if (tasks[i].done) {
      attroff(COLOR_PAIR(1) | A_BOLD);
    }
    if (i == selectedTask) {
      attroff(A_REVERSE);
    }
  }
}

void addTask() {
  echo();
  char newTask[256];
  int row = getmaxy(stdscr);
  mvprintw(row - 2, 0, "Enter task description: ");
  getnstr(newTask, 256);
  if (strlen(newTask) == 0 || strspn(newTask, " \t\n") == strlen(newTask)) {
    noecho();
    return;
  }
  if (taskCount < MAX_TASKS) {
    strcpy(tasks[taskCount].description, newTask);
    tasks[taskCount].done = false;
    taskCount++;
  }
  noecho();

  if(selectedTask < 0) {
    selectedTask = 0;
  }
}

void removeTask() {
  if (taskCount <= 0) {
    return;
  }
  for (int i = selectedTask; i < taskCount - 1; i++) {
    tasks[i] = tasks[i + 1];
  }
  taskCount--;
  if (selectedTask >= taskCount) {
    selectedTask = taskCount - 1;
  }
}

int main() {
  int ch;
  bool quit = false;

  initscr();
  cbreak();
  start_color();
  noecho();
  init_pair(1, COLOR_GREEN, COLOR_BLACK);

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
    printw("Task Manager");
    attroff(A_BOLD);
    keypad(stdscr, TRUE);

    printTasks();

    move(row - 1, 0);
    printw("Press 'q' to quit, 'a' to add task and 'd' to delete");

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
        removeTask();
        break;
      case 'a':
        addTask();
        break;
    }

  }
  endwin();
  return 0;

}
