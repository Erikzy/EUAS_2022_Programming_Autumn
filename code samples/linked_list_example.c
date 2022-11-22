#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 struct node {
     int key;
     int value;
     char name[20];
     struct node * next;
  };

struct node * insertnode(int key, int value, char * name,struct node * head );
struct node * findnode( int key,struct node * head);


void linked_list_example(){
  struct node* head = NULL;
  struct node* searchable;

  // head = null
  head = insertnode(1,24,"Erik", head);
  // head is the node with Erik

  head = insertnode(2,22,"Ender", head);
  //Ender be the head , and point to Erik as the nex

  head = insertnode(3,25,"Yennefer", head);
  //Yennefer as the head,
  head = insertnode(4,29,"Sylvanas", head);
  //Sylvanas as the head

  searchable = findnode(2,head);


  printf("\nSearched for node with the key of %d  and found %s ",searchable->key,searchable->name);

}

struct node * insertnode(int key, int value, char * name, struct node * head ){

    struct node * newelement = (struct node *) malloc(sizeof(struct node));


    if(head != NULL){
          printf("\n Current head key: %d - %s ", head->key, head->name);
    }

    newelement->key = key;
    newelement->value = value;
    strcpy(newelement->name, name);

    newelement->next = head;
    head = newelement;


    printf("\n Added key: %d - %s ", newelement->key, newelement->name);
    return head;
}

struct node * findnode( int key, struct node * head){
   struct node * current;

   current = head;

   if(head == NULL){
    printf("\n List is empty");
    return NULL;
   }


   while(current->key != key) {

    printf("\n Comparing %d - %d ",current->key, key);
  
       if(current->next == NULL){
        printf("\n Last element reached");
        return NULL;
    }else{
      current = current->next;
    }
  }
    
    
  printf("\n found %d - %s", current->key, current->name);
  return current;
}
