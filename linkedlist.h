#ifndef _THLIST_
#define _THLIST_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdbool.h>
#include <errno.h>

typedef struct Node
{
  int data;
  int next;
}node;
typedef struct linkedlist
{
  int head;
  int count;
  int my_key;
  int semid;
  int i;
//  int (*compareFn)(void*, void*);
//  void (*printFn)(void*);
}list;

int createList(/*int (*compare)(void*, void*), void (*print)(void*)*/int listno);
void addNode(int, int);
void printList(int);
bool isPresentList(int, int);
bool deleteNode(int, int);
void removeList(int);

#endif
