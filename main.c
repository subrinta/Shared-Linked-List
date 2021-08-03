#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

int listid;

// void printInt(void* a)
// {
//   printf("%d", *((int*)a));
// }
//
// int compareInt(void* a, void* b)
// {
//   int* x = (int*)a;
//   int* y = (int*)b;
//   if(*x>*y)
//   {
//     return 1;
//   }
//   if(*x==*y)
//   {
//     return 0;
//   }
//   return -1;
// }

int main()
{
  printf("Program A....\n");
  listid = createList(1);
  for(int i=0; i<5; i++)
  {
    addNode(listid, i);
  }
  printf("wait!! **run program B**");
  getchar();
  printList(listid);
  printf("Press enter in Prigram B");
  getchar();
  printf("Linked list after deletion of one node:\n");
  printList(listid);
  printf("Press enter in here to remove tha linkedlist");
  getchar();
  removeList(listid);
}
