#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void main()
{
    char ch;
    int i=0,count=0;
    char target[25][40];
    system("xclip -o -t TARGETS > target.txt");
    FILE *fp;
    fp=fopen("target.txt","r");
    while(!feof(fp))
    {
        fscanf(fp,"%s",target[count]);
        count++;
    }
    strcpy(target[count],"\0");
    count--;
    fclose(fp);
    for(i=0;i<count;i++)
        printf("%s\n",target[i]);
}