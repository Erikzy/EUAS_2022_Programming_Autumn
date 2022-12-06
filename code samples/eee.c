#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void runGame();
int executeTurn();

void handleUpdates();
void outputState();
int handleEncounters();
void handlePlayerChoice();
void playerPicksUpWeaponIfNotOwned();
char * getRoomText();
char * getAvailableDirections();
int handleMonsterEncounter();
int getPlayerChoice();
void addPlayerLives();
int handleCombatChoice();
int checkIfMonsterAttacks();
void escapeAttempt(int *combatEnded);
void attackAttempt(int *combatEnded);

//Room coordinates are x,y,
const int playerX = 1;
const int playerY = 0;
const int playerLives = 100;
static int weaponPickedUp = 0;
struct Player {
    int XLocation;
    int YLocation;
    int lives;
};

static struct Player player;

static int healingRegen = 0;
static int turns = 0;
static int monsterLives = 200;
static int secondMonsterLives = 100;

static char directionNames[4][10]  = {
   "(u)p",
   "(r)ight",
   "(d)own",
   "(l)eft"
};
                            // x  y  direction
static int availableDirections[3][3][4]  = {
 //Definition for a room's available directions: Up, Right, Down, Left. 0 - blocked, 1 - available
  {  //x=0
   { 1, 1, 0, 0}, //y=0
   { 1 ,1, 1, 0}, //y=1
   { 0, 1, 1, 0}, //y-2
  },{ //x=1
   { 1, 1, 0, 1}, //y=0
   { 1, 1, 1, 1}, //y=1
   { 0, 1, 1, 1}, //y-2
  },{ //x=2
   { 1, 0, 0, 1}, //y=0
   { 1, 0, 1, 1}, //y=1
   { 0, 0, 1, 1}, //y-2
  }
};

static char roomTexts[3][3][200] = {
    {
        {"You see a gaping canyon before you blocking your path to the left.\nYou can go:\n"},
        {"You Can go:\n"},
        {"You Can go:\n"},
    },
    {
        {"You enter the forest of a thousand truths. You Can go:\n"},
        {"You Can go:\n"},
        {"You Can go:\n"},
    },{
        {"You Can go:\n"},
        {"You Can go:\n"},
        {"You enter a holy place. You Can go:\n "},
    }
};
//Main Method
int main()
{
    //Initialize player base stats
    player.XLocation = playerX;
    player.YLocation = playerY;
    player.lives = playerLives;
    //enter Game loop
    runGame();
    return 0;
}


void runGame(){
  int isGameOver = 0;
  //While the game is not over, execute loop
  while(!isGameOver){
      isGameOver = executeTurn();
  }
}

int executeTurn(){
    int gameOver;
    //incement turn counter
    turns++;
    //handle any uptades, that have to happen each turn
    handleUpdates();
    //output details about player's status: lives, location, etc.
    outputState();

    //handle a random encounter, and check whether it caused the game to end
    gameOver = handleEncounters();
    if(gameOver > 0){
        return 1;
    }
    //give player a choice for further exploration
    handlePlayerChoice();
    return 0;
}

void handleUpdates(){
    //decrement the amount of turns left until healing is available
    if(healingRegen > 0){
        healingRegen--;
    }
}

void outputState(){
    char* roomText;
    //get room description text
    roomText = getRoomText();
    printf("\n\n\n\n");
    printf("\nPlayer location: X: %d Y: %d",player.XLocation,player.YLocation);
    printf("\nPlayer lives: %d",player.lives);
    printf("\nCurrent turn: %d \n",turns);
    printf("\n%s",roomText);
    printf("\n\n");
    //iterate x
    //iterate y;
    for(int j = 2; j >=0;j--){
      printf("\n y: %d ",j);
      for(int i = 0;i<= 2;i++){
         switch(j){
         case 2:
             switch(i){

                case 2:
                       printf("| h |");
                break;
                default:
                    printf("|   |");

            }
            break;
                default:
                    printf("|   |");
         }
      }
      printf ("\n");
    }



}
void playerPicksUpWeaponIfNotOwned(){
    if(weaponPickedUp == 0){
        printf("\nIts dangerous to go alone. Take this\n");
        weaponPickedUp = 1;
    }

}

//Decide any encounters that should happen in the given room.
int handleEncounters(){
    switch(player.XLocation){
        case 0:
            switch(player.YLocation){
                case 0:
                    playerPicksUpWeaponIfNotOwned();

                break;
                case 1:
                break;
                case 2:
                break;
            }
        break;
        case 1:
            switch(player.YLocation){
                case 0:
                break;
                case 1:
                break;
                case 2:
                break;
            }

        break;
        case 2:
            switch(player.YLocation){
                case 0:
                break;
                case 1:

                break;
                case 2:
                    //Healing station in X:2 Y:2
                    addPlayerLives();
                break;
            }
        break;
    }

    if(checkIfMonsterAttacks()){
        return handleMonsterEncounter();
    }
    return 0;
}

void handlePlayerChoice(){
    int choice = getPlayerChoice();
    switch(choice){
        //u - up
        case 117:
           if(availableDirections[player.XLocation][player.YLocation][0]){
              printf("\nYou chose to go up.\n\n");
              player.YLocation++;
           } else{
             printf("\nYou cannot go there");
           }

        break;
        //r - right
        case 114:
           if(availableDirections[player.XLocation][player.YLocation][1]){
              printf("\nYou chose to go right.\n\n");
              player.XLocation++;
           } else{
             printf("\nYou cannot go there");
           }
        break;
        //d - down
        case 100:
           if(availableDirections[player.XLocation][player.YLocation][2]){
              printf("\nYou chose to go up.\n\n");
              player.YLocation--;
           } else{
             printf("\nYou cannot go there");
           }
        break;
        //l - left
        case 108:
           if(availableDirections[player.XLocation][player.YLocation][3]){
              printf("\nYou chose to go up.\n\n");
              player.XLocation--;
           } else{
             printf("\nYou cannot go there");
           }
        break;

        default:
            printf("Unknown choice %d", choice);
    }
    getchar();

}

int getPlayerChoice(){
    char selected;
    char* availableDirections;
    availableDirections  = getAvailableDirections();
    printf("\n%s\nWhere will you go? Choose wisely:  ",availableDirections);
    scanf(" %c", &selected);
    return (int)selected;
}

char * getRoomText(){
    static char fullString[240];
    strcpy(fullString,"");
    strcat(fullString, roomTexts[player.XLocation][player.YLocation]);
    return fullString;
}

char * getAvailableDirections(){
    static char availableDirectionString[40];
    strcpy(availableDirectionString,"");
    //Iterate through all directions, and if the value is 1, add to the player output
    for(int i = 0; i < 4; i++){
        if(availableDirections[player.XLocation][player.YLocation][i]){
            strcat(availableDirectionString, directionNames[i]);
            strcat(availableDirectionString,"\n");
        }
    }
    return availableDirectionString;
}


int checkIfMonsterAttacks(){
    int randomResult;
    time_t t;
    srand((unsigned) time(&t));

    randomResult = rand() % 10;
    //simplest implementation of random in c
    //values 5-9 will produce a monster attack
    if(randomResult > 4){
        return 1;
    } else {
        return 0;
    }
}

int handleMonsterEncounter(){
    printf("A horribly disformed figure appears in front of you.\n");
    printf("He has %d lives left\n", monsterLives);
    printf("If you choose to run, you have a 2-out-of-6 chance 10 health. \n");
    printf("If you choose to attack, you have a 4-out-of-6 chance to do 20 damage. \n");
    return handleCombatChoice();
}


void addPlayerLives(){
    if(healingRegen == 0){
        printf("You feel your life essence regenerated. +10 lives.\n");
        player.lives += 10;
        healingRegen += 5;
    } else {
        printf("The well of life is dry. Wait for %d turns \n",healingRegen);
    }
}

int handleCombatChoice(){
    char selected;
    int combatEnded;
    combatEnded = 0;
    printf("\n\nDo you: (r)un or (a)ttack\n");
    //Pass the MEMORY ADDRESS of the selected value to the scanf function
    scanf(" %c", &selected);
    switch(selected){
        case 'r':
           //Pass the reference to the Combat ended variable
           escapeAttempt(&combatEnded);
        break;
        case 'a':
           attackAttempt(&combatEnded);
        break;
        default:
            printf("Invalid option.\n");
            combatEnded = 0;
    }

    if(player.lives <= 0){
        printf("You died!\nGame over\n");
        return 1;
    }
    if(monsterLives <= 0){
        printf("You killed the monster!\nGame over\n");
        return 1;
    }

    if(!combatEnded){
       return handleCombatChoice();
    }
    return 0;
}

//accept a pointer to a variable
void escapeAttempt(int *combatEnded){
    int randomResult;
    time_t t;
    srand((unsigned) time(&t));
    randomResult = rand() % 6;
    if(randomResult > 1){
        printf("You have succeeded in escaping the monster without harm!\n");
        //assign a value to the variable.
        *combatEnded = 1;
    }else{
        printf("You try to run, but the monster manages to hurt you.\nYou lose 10 lives\n");
        player.lives -= 10;
        *combatEnded = 1;
    }
}

void attackAttempt(int *combatEnded){
    int randomResult;
    time_t t;
    srand((unsigned) time(&t));
    randomResult = rand() % 6;
    if(randomResult > 1){
        printf("You have succeeded in hitting the monster without harm!\n");
       
        if(weaponPickedUp){
            monsterLives -= 40;
        }else{
             monsterLives -= 20;
        }
       
        *combatEnded = 0;
    }else{
        printf("You try to hit, but fail miserably.\nYou lose 20 lives\n");
        player.lives -= 20;
        *combatEnded = 0;
    }
    printf("Monster lives %d\n", monsterLives);
    printf("Player lives %d\n\n\n",player.lives);
}
