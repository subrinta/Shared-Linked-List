#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

int listid;

int main()
{
  listid = createList(1);
  removeList(listid);
}
