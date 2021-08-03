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
  int k,t=2;
  printf("Program B....\n");
  listid = createList(1);
  printList(listid);
  printf("Press enter");
  getchar();
  for(int i=5; i<10; ++i)
  {
    addNode(listid, i);
  }
  printf("Press enter in program A!");
  getchar();
  while(t--){
    printf("give a no to search in linked list: ");
    scanf("%d",&k);
    if(isPresentList(listid, k))
    {
      printf("%d is present in the linked list\n",k);
    }
    else{
      printf("%d is NOT present in the linkedlist\n",k);
    }
  }
  printf("Enter a value to delete from shared linkedlist: ");
  scanf("%d",&k);
  if(deleteNode(listid, k))
  {
    printf("susccesfully deleted...\nTo see the result press enter in program A\n");
  }

}
