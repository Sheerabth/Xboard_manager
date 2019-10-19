#include<stdio.h>
#include<gtk/gtk.h>
void main()
{
    char ch;
    int i=0,count=0;
    char target[25][40];
    system("xclip -o -t TARGETS > target.txt");
    FILE *fp;
    sleep(0.5);
    fp=fopen("target.txt","r");
    while(!eof(fp))
    {
        ch=fgetc(fp);
        fscanf(fp,"%[^\n]s",target[i]);
    }
    fclose(fp);
}