#include "linkedlist.h"

#define P(s) semop(s, &Pop, 1);
#define V(s) semop(s, &Vop, 1);

struct sembuf Pop;
struct sembuf Vop;

union semun {
  int              val;
  struct semid_ds *buf;
  unsigned short  *array;
  struct seminfo  *__buf;
} setvalArg;


int createList(/*int (*compare)(void*, void*), void (*print)(void*),*/int listno)
{
  int shared_key = ftok("/home/subi/OS_lab_18_2_21/shared_ll/sll_lib.c",1);
  int id = shmget(shared_key, sizeof(list), IPC_CREAT | IPC_EXCL | 0666);
  if(id==-1)
  {
    id = shmget(shared_key, sizeof(list), IPC_CREAT | 0666);
    list* l = (list*) shmat(id, NULL, 0);
    l->my_key = ftok("/home/subi/OS_lab_18_2_21/shared_ll/sll_lib.c", listno);
    l->semid = semget(l->my_key, 1, IPC_CREAT | 0777 );
  }
  else{
    list* l = (list*) shmat(id, NULL, 0);
    l->head = 0;
    l->count = 0;
    l->i = 2;
    l->my_key = ftok("/home/subi/OS_lab_18_2_21/shared_ll/sll_lib.c", listno);
    l->semid = semget(l->my_key, 1, IPC_CREAT | 0777 );
    setvalArg.val = 1;
    semctl(l->semid, 0, SETVAL, setvalArg);
    // l->compareFn = compare;
    // l->printFn = print;
  }
  return id;

}

void addNode(int listid, int data)
{
  Pop.sem_num = 0;
  Pop.sem_op = -1;
  Pop.sem_flg = SEM_UNDO;

  Vop.sem_num = 0;
  Vop.sem_op = 1;
  Vop.sem_flg = SEM_UNDO;

  list* l = (list*)shmat(listid, NULL, 0);
  if(l==(void*)-1)
  {
    perror("unable to attach listid");
    exit(1);
  }
  int k = P(l->semid);
  if(k==-1)
  {
    perror("Sem P problem");
    exit(1);
  }

  int new_key = ftok("/home/subi/OS_lab_18_2_21/shared_ll/sll_lib.c", l->i);
  if(new_key==-1)
  {
    perror("new_key fork error");
    exit(1);
  }
  l->i++;
  int shmid = shmget(new_key, sizeof(node), IPC_CREAT | 0666);
  if(shmid == -1)
  {
    perror("Unable to create node");
    exit(1);
  }
  node* new = (node*) shmat(shmid, NULL, 0);
  if(new == (void *)-1)
  {
    perror("unable to access new node");
    exit(1);
  }

  new->data = data;
  new->next = -1;
  if(l->count==0)
  {
    l->head = shmid;
    l->count++;
    int k = V(l->semid);
    if(k==-1)
    {
      perror("Sem v problem");
      exit(1);
    }
    return;
  }
  node* curr = (node*)shmat(l->head, NULL, 0);
  if(curr == (void*)-1)
  {
    perror("unable to access head");
    exit(1);
  }
  int nid=curr->next;
  while(nid!=-1)
  {
    shmdt(curr);
    curr = (node*)shmat(nid, NULL, 0);
    if(curr == (void*)-1)
    {
      perror("unable to access middle node");
      exit(1);
    }
    nid=curr->next;
  }

  curr->next = shmid;
  l->count++;
  k = V(l->semid);
  if(k==-1)
  {
    perror("Sem v problem");
    exit(1);
  }
}

void printList(int listid)
{
  Pop.sem_num = 0;
  Pop.sem_op = -1;
  Pop.sem_flg = SEM_UNDO;

  Vop.sem_num = 0;
  Vop.sem_op = 1;
  Vop.sem_flg = SEM_UNDO;

  list* l = (list*)shmat(listid, NULL, 0);
  if(l==(void*)-1)
  {
    perror("in printlist unable to attach the listid");
    exit(1);
  }
  P(l->semid);
  node* curr = (node*)shmat(l->head, NULL, 0);
  if(curr == (void*)-1)
  {
    perror("unable to attach head in printll");
    exit(1);
  }
  int nid=curr->next;
  while(nid!=-1)
  {
    printf("%d",curr->data);
    printf("\n");
    shmdt(curr);
    curr = (node*)shmat(nid, NULL, 0);
    if(curr == (void*)-1)
    {
      perror("unabel to attach with middle node in printll");
      exit(1);
    }
    nid=curr->next;
  }
  printf("%d",curr->data);
  printf("\n");
  V(l->semid);
}

bool isPresentList(int listid, int data)
{
  Pop.sem_num = 0;
  Pop.sem_op = -1;
  Pop.sem_flg = SEM_UNDO;

  Vop.sem_num = 0;
  Vop.sem_op = 1;
  Vop.sem_flg = SEM_UNDO;

  list* l = (list*)shmat(listid, NULL, 0);
  if(l==(void*)-1)
  {
    perror("in isPresentList unable to attach the listid");
    exit(1);
  }
  P(l->semid);
  node* curr = (node*)shmat(l->head, NULL, 0);
  if(curr == (void*)-1)
  {
    perror("unable to attach head in isPresentList");
    exit(1);
  }
  int nid=curr->next;
  while(nid!=-1)
  {
    if(curr->data == data)
    {
      V(l->semid);
      return true;
    }
    shmdt(curr);
    curr = (node*)shmat(nid, NULL, 0);
    if(curr == (void*)-1)
    {
      perror("unabel to attach with middle node in printll");
      exit(1);
    }
    nid=curr->next;
  }
  if(curr->data == data)
  {
    V(l->semid);
    return true;
  }
  V(l->semid);
  return false;

}

bool deleteNode(int listid, int data)
{
  Pop.sem_num = 0;
  Pop.sem_op = -1;
  Pop.sem_flg = SEM_UNDO;

  Vop.sem_num = 0;
  Vop.sem_op = 1;
  Vop.sem_flg = SEM_UNDO;

  list* l = (list*)shmat(listid, NULL, 0);
  if(l==(void*)-1)
  {
    perror("in isPresentList unable to attach the listid");
    exit(1);
  }
  P(l->semid);
  node* curr = (node*)shmat(l->head, NULL, 0);
  if(curr == (void*)-1)
  {
    perror("unable to attach head in isPresentList");
    exit(1);
  }
  int nid=curr->next;
  if(curr->data == data)
  {
    shmdt(curr);
    shmctl(l->head, IPC_RMID, NULL);
    l->head = nid;
    l->count--;
    V(l->semid);
    return true;
  }
  node* prev = (node*)shmat(l->head, NULL, 0);
  if(prev == (void*)-1)
  {
    perror("unable to attach head in isPresentList");
    exit(1);
  }
  int pnid=prev->next;
  shmdt(curr);
  curr = (node*)shmat(nid, NULL, 0);
  if(curr == (void*)-1)
  {
    perror("unable to attach head in isPresentList");
    exit(1);
  }
  nid=curr->next;

  while(nid != -1)
  {
    if(curr->data == data)
    {
      shmdt(curr);
      shmctl(prev->next, IPC_RMID, NULL);
      prev->next = nid;
      l->count--;
      V(l->semid);
      return true;
    }
    else
    {
      shmdt(prev);
      prev = (node*)shmat(pnid, NULL, 0);
      if(prev == (void*)-1)
      {
        perror("unable to attach head in isPresentList");
        exit(1);
      }
      pnid = prev->next;
      curr = (node*)shmat(nid, NULL, 0);
      if(curr == (void*)-1)
      {
        perror("unable to attach head in isPresentList");
        exit(1);
      }
      nid = curr->next;
    }
  }
  V(l->semid);
  return false;

}
void removeList(int listid)
{
  Pop.sem_num = 0;
  Pop.sem_op = -1;
  Pop.sem_flg = SEM_UNDO;

  Vop.sem_num = 0;
  Vop.sem_op = 1;
  Vop.sem_flg = SEM_UNDO;

  list* l = (list*)shmat(listid, NULL, 0);
  if(l==(void*)-1)
  {
    perror("in printlist unable to attach the listid");
    exit(1);
  }
  P(l->semid);
  node* curr = (node*)shmat(l->head, NULL, 0);
  if(curr == (void*)-1)
  {
    perror("unable to attach head in printll");
    exit(1);
  }
  int nid=curr->next;
  shmdt(curr);
  int dlt = shmctl(l->head, IPC_RMID, NULL);
  if(dlt == -1)
  {
    perror("Cannot remove list hade");
    exit(-1);
  }
  l->head = -1;
  int pnid;
  while(nid!=-1)
  {
    curr = (node*)shmat(nid, NULL, 0);
    if(curr == (void*)-1)
    {
      perror("unabel to attach with middle node in printll");
      exit(1);
    }
    pnid = nid;
    nid=curr->next;
    shmdt(curr);
    dlt = shmctl(pnid, IPC_RMID, NULL);
    if(dlt == -1)
    {
      perror("Cannot remove list node");
      exit(-1);
    }
  }
  int semid = l->semid;
  shmdt(l);
  dlt = shmctl(listid, IPC_RMID, NULL);
  if(dlt==0)
  {
    printf("linkedlist removed\n");
  }
  V(semid);
  dlt = semctl(semid, 0,  IPC_RMID);
  if(dlt == -1)
  {
    perror("unable to remove semaphore");
    exit(1);
  }

}
