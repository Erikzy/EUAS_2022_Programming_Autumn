#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *fp;
    char fileName[20] = "test.csv";
    char buffer[256];


    fp = fopen(fileName, "w+");

    for(int i = 0; i < 5; i++){

        fputs("THIS LINE is the line to write\n", fp);
    }

    fclose(fp);

    fp = fopen(fileName, "r");

    int count = 0;

    while (  fgets(buffer, sizeof(buffer), fp)  ) {
      printf("%d: %s",count, buffer);
      count++;

    }
    fclose(fp);


    return 0;
}
