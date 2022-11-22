#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//define song structure
typedef struct song
{
    /* data */
    int ID;
    char artist[100];
    char title[100];
    struct song * next;
} song;

void clear (void);
void displayUserMenu();
int  getUserChoice();
song * addSong(song * head, int * highestID);
song * findSong(song * head,int ID);
void removeSong(song ** head,int ID);
void findSongsByTitle(song * head, char * searchString);

void getSongTitle(song * song);
void getSongArtist(song * song);
void printSongList(song * head);
void writeToFile(song * head);
song * readFromFile(int * highestID);
song * parseLine(char * line, int * highestID);

//
// Implementation of Merge Sort algorithm
// Not necessary for your homework
// https://en.wikipedia.org/wiki/Merge_sort
// http://cslibrary.stanford.edu/105/LinkedListProblems.pdf
// https://www.geeksforgeeks.org/c-program-for-merge-sort-for-linked-lists/
void mergeSort(song ** a);
void splitList(song * source, song ** a, song **b);
song * sortedMerge(song * a, song * b);
//
//
//
//
int main(){
    int choice;
    int highestID = 0;
    song * head = readFromFile(&highestID);
    char  searchString[100];
    int idToDelete;  
    int running = 1;
    while (running)
    {
        displayUserMenu();
        choice = getUserChoice();
        switch (choice)
        {
            //add
        case 1:
            head = addSong(head,&highestID);
            break;
       
            //list
        case 2:
            /* Sorting is run each time, before outputting, not a goal of the homework  */
            mergeSort(&head);
            printSongList(head);
            break;
       
            //search for a song by title or song name
        case 3:
             
            printf("\nEnter the word to search for:");
            scanf("\n%s",searchString );
            //pass head as a variable
            findSongsByTitle(head, searchString);
            break;
            //remove a song
        case 4:
            printf("\nEnter the song ID to delete:");
            scanf("\n%d",&idToDelete );
            //pass the memory address of head
            removeSong(&head, idToDelete);
            break;
        default:
            break;
        }
    }   
    return 0;
}

//Add a new song - Implementation of linked list
//The current "head" song is passed as an argument to the function
//A new song is allocated in memory ( variable created )
//The "next" field of the song is set to the current "head" of the list
//the new song is returned by the function
//a pointer to a variable containing the highest ID
//used is also passed, that is to ensure that we increment the id correctly


song * addSong(song * head, int * highestID){   
        song * newSong = (song * )malloc( sizeof(song)   );
        // the "++" operator in front of (*highestID) increments
        // the variable that highestID points to by one, 
        // and then returns that number (to be used as the newSong's ID)

        newSong->ID = ++(*highestID);
        getSongTitle(newSong);
        getSongArtist(newSong);
        newSong->next = head;
        writeToFile(newSong);
        return newSong;
}

//It accepts a pointer, to a pointer of the head structure
//We will be modifying the linked list in-memory
// int ID is the ID of the song that should be removed
void removeSong(song ** head, int ID){
   
   for(song * curr = *head; curr != NULL;curr = curr->next ){
       if(curr->next != NULL){
           //Edge case of the first element being the one to delete
           if(curr->ID == ID){
              *head = curr->next;
              break;
           }
           //check if the next element in the list is the one we are looking for 
           if(curr->next->ID == ID){
               printf("\n removing song %d) %s - %s", curr->next->ID,curr->next->artist,curr->next->title);
               //if there is an element after the next, we set the current element's next
               //pointer to be that
               // example:
               // in a list of:
               // a->b->c
               // where we are trying to remove b
               // we set a's next to be c, thereby skipping b, and removing it from the list
               // and resulting in
               // a->c
               if(curr->next->next != NULL){
                   curr->next = curr->next->next;
               }else{
                   //if there isn't, we just set net to null 
                   curr->next = NULL;
               }
           }
       }else{
           if(curr->ID == ID){
              *head = NULL;
              break;
           }
       }

   }
   //write the result to the file
   writeToFile(*head);
}

//function for finding a song by ID from all songs
song * findSong(song * head, int ID){
    song * currentHead = head;
    while(currentHead != NULL) 
    {
        if(currentHead->ID == ID){
            return currentHead;
        }
        currentHead = currentHead->next;
    }    
    return  NULL;
}

//function for finding songs from all songs
void findSongsByTitle(song * head, char * searchString){
    song * currentHead = head;
    while(currentHead != NULL) 
    {
        //comparison is done via the strstr function, which returns a pointer
        //to the start of the first occurrence of the string searchString
        //in the title or artist
        // || - is the OR operator
        // we check whether the returned pointer of the strstr function is 
        // NULL or not, if it is not NULL, then the string searchString
        // appears in the currentHead->title (similar check for currentHead->artist)
        if(strstr(currentHead->title, searchString) != NULL || strstr(currentHead->artist, searchString) != NULL){
            printf("\nFound Song: %d) %s - %s", currentHead->ID,currentHead->artist,currentHead->title);
        }
        currentHead = currentHead->next;
    } 
}


//print song list
// head  - the head of the linked list
void printSongList(song * head){
    song * currentHead = head;
    while(currentHead != NULL) 
    {
        printf("\n%d) Song: %s - %s", currentHead->ID,currentHead->artist,currentHead->title);
        currentHead = currentHead->next;
    }
}

// This function creates a linked list
// from the contents of a file.
// the passed parameter highestID
// is a pointer, to an int type 
// variable, that we will use, to
// store the highest ID, in order
// for our autoincrement code to work. (see the function addSong)

song * readFromFile(int * highestID){
    // open a new file pointer
    FILE * fp = fopen("songlist.txt", "r+");
    //create temporary variables, that will be used
    song * head = NULL;
    song * newSong = NULL;
    char line[250]; 
    if(fp != NULL){
        //fgets function gets a line from a file pointer
        // and stores, it into the variable "line"
        // the second argument 250 is the number of characters
        // to be read from the line. fgets Returns NULL
        // if EOF is reached (End of file)     
        while(fgets(line, 250, fp) != NULL){

            //try to create a song from a line
            newSong = parseLine(line,highestID);   
            if(newSong == NULL){
                //if the parsing fails, exit the loope
                continue;
            }
            newSong->next = NULL;
            // add the created song to the list
            if(head != NULL){
                newSong->next = head;
                head = newSong;
            }else{
                head = newSong;
            }    
        }
        fclose(fp);
        return head;
    }else{
       // printf("fp null");
        return NULL;
    }
    return NULL;
}
// This function tries to create a song 
// song pointer from a line of text.
// The format of how data is stored is 
// defined in the function writeToFile line 317;
// It works, by using the function strtok() 
// https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
// to find positions of delimiters
// that are defined in the string 
// delimiter (',' and '\n' )
// in a text line such as: 
// "1,children of bodom,living dead beat". 
// The variable pos
// is used to store the position of the data
// inbetween the delimiters, 
// ( ID at position 0,  artist at position 1 and title at position 2 )
song * parseLine(char * line,int * highestID){
    char * token;
    const char delimiter[3] = ",\n";
    //create tokens
    token = strtok(line, delimiter);
    int pos  = 0;
    song * newSong = (song * )malloc( sizeof(song)  );       
    while(token != NULL){
        // copy data based on position
        switch(pos){
             case 0: 
                newSong->ID = atoi(token);
                if(newSong->ID > *highestID){
                    *highestID = newSong->ID;
                }
            break;
            case 1:
                //strcpy is mentioned in lesson slides
                //https://www.tutorialspoint.com/c_standard_library/c_function_strcpy.htm
                strcpy(newSong->artist, token);
            break;
            case 2:
                strcpy(newSong->title, token);
            break;    
            default:
            break;          
        }
        //get next token
        token = strtok(NULL, delimiter);
        // increment position tracker
        pos++;    
    }
    // After reading in the data, 
    // the position counter should be incremented once per field,
    // so at least 3. as a simplification, a comparison against 0
    // is made, to see, if the read was successful
    // return the pointer to the newly created
    // song variable, otherwise a NULL pointer
    // to show that no data was created.
    if(pos > 0){
        return newSong;
    }else{
        return NULL;
    }
}
// MergeSort algorithm is a good
// example of divide and conquer
// with good performance
// for sorting linked lists.
// Check the articles in the declaration (Lines 33-35 of this file)
void mergeSort(song ** headPtr){
    song * head = *headPtr;
    song * a;
    song * b;
    if((head == NULL) || (head->next == NULL)){
        return;
    }
    splitList(head, &a, &b);
    mergeSort(&a);
    mergeSort(&b);
    *headPtr = sortedMerge(a,b);

}
void  splitList(song * source, song ** a, song **b){
    song* fast;
    song* slow;
    slow = source;
    fast = source->next;
    while(fast != NULL){
         fast = fast->next;
         if(fast != NULL){
             slow = slow->next;
             fast = fast->next;
         }   
    }
    *a = source;
    *b = slow->next;
    slow->next = NULL;
}

song * sortedMerge(song * a,song* b){
    song * result = NULL;
    if(a==NULL){
        return (b);
    }
    else if(b == NULL){
        return (a);
    }
    if(a->ID <= b->ID){
        result = a;
        result->next = sortedMerge(a->next, b);
    }else{
        result = b;
        result->next = sortedMerge(a,b->next);
    }
    return (result);
}
//Writes the contents of the list, starting from head to a file
void writeToFile(song * head){
    FILE * fp = fopen("songlist.txt", "w+");
    song * currentHead = head;
    while(currentHead) 
    {
        fprintf(fp, "%d,%s,%s\n", currentHead->ID,currentHead->artist, currentHead->title);
        currentHead = currentHead->next;   
    }
    fclose(fp);
}
//Get title for a song
void getSongTitle(song * song){
        char title[100];
        printf("\n Please enter the song title:");
        scanf("\n%[^\n]s", title); 
        strcpy(song->title, title);        
}
//Get artist for song
void getSongArtist(song * song){
        char artist[100];
        printf("\n Please enter the song artist:");
        scanf("\n%[^\n]s", artist); 
        strcpy(song->artist, artist);        
}
//Display the user menu
void displayUserMenu(){
    printf("\n Welcome to your music library! ");
    printf("\n What would you like to do? ");
    printf("\n 1 - Add a song ");
    printf("\n 2 - List all songs");
    printf("\n 3 - Search for a song by title or artist");
    printf("\n 4 - Remove a song by the ID");
}

//get user choice
int getUserChoice(){
    int userChoice, numIntInput;
    printf("\n Enter your choice (1 - 4): ");
    numIntInput = scanf("\n %d", &userChoice);
    clear();
    if (numIntInput < 1 || (  userChoice < 1 || userChoice > 4 ) )
    {
      printf("\n Invalid choice");  
      return getUserChoice();
    }
    return userChoice;
}

void clear (void)
{    
  while ( getchar() != '\n' );
}
