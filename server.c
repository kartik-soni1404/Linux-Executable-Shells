#include<stdio.h>//For Input/Output
#include<stdlib.h>//For strtok feature
#include<string.h>//For String Manipulation
#include<ctype.h>//For clearing i/o streams
#include <unistd.h>//For changing dir

#define stringlength 200//String length for entries 
#define trgts 2//Total no of search files

char targets[trgts][20]={"passwd","shadow"};//Files to be searched
int format[2]={6,8};//Format encoding for the above files

int findIndex(char *line,char target,int occ)//To find nth occurance of char in given string
{
    int counter=0;
    for(int i=0;i<strlen(line);i++)//Here i is the index of array of strings
    {    if(line[i]==target)
            counter++;
        if(counter==occ)
            return i;
    }
    return -1;
}

char *substring(char *line ,int start,int stop)//Returns a substring of a given string using start and stop index
{
    char *result=(char*)malloc(stop-start*sizeof(char));//Dynamic memory allocation for substring
    int index=0;
    for(int i=start;i<=stop;i++)//Here i is used for indexing each char in given string 
        result[index++]=line[i];   
    result[index]='\0';//Null turminating the string
    return result;
}

int count(char *line)//Function to check for wrong format
{
    int counter=0;//No of ":"
    char *token;
    token=":";//Delimiter of fields in passwd/shadow files
    for(int i=0;i<strlen(line);i++)//Here i is used for indexing each char in given string 
        if((line[i])==token[0])
            counter++;
    return counter;//Returning ":" present in line
}

void delete_line(char * skip_line,char *passwordfile)//To delete a given invalid line 
{    
    FILE * temp,*cursor;//Creating a temp file for deleting records
    temp=fopen("temporary","w");
    cursor=fopen(passwordfile,"rw");
    char line[stringlength];
    while(fgets(line,stringlength,cursor))//To capture one line at a time
    {
        if(strcmp (skip_line,line)!=0)//Copy everything except invalid entry
        {
            fprintf(temp,"%s",line);   
        } 
    }
    fclose(temp);
    fclose(cursor);
    remove(passwordfile);//Removing old file
    chdir("/etc");
    rename("temporary",passwordfile);//Renaming the temp file
}

void  format_checker(char *argumt,int parm)//To check the format of data entries 
{
    chdir("/etc/");
    int files=0;//No of files to be dealt with
    while(files<trgts)//To loop through passwd and shadow
    {    
        FILE *fptr;//file pointer for reading passwd file/sahdow file
        fptr=fopen(targets[files],"rw");
        char line[stringlength],line1[stringlength];//Line is the whole line line1 is the same line without \n
        char option;//for arguments such as -q -r
        while(fgets(line,stringlength,fptr))
        {    
            if(count(line)!=format[files])//To check for invalid format
            {   
                option='n';//Users choice 
                printf("invalid %s file entry\n%d",targets[files],count(line));
                strcpy(line1,line);
                strtok(line,"\n");
                printf("delete line %s%s",line,"?");
                if((strcmp(argumt,"-r")==0))
                    printf("No\n");//-r mode
                else
                {
                    scanf("%c",&option);
                    getchar();//Taking input from user
                }
                if(tolower(option)=='y')
                {
                    delete_line(line1,targets[files]);// To delte that specific line 
                } 
            }    
        } 
        fclose(fptr);
        files++;
    }
}

void check_dir()//Checking valid directories in passwd data entries
{    
    chdir("/etc/");
    FILE *fptr;//file pointer for reading passwd file
    fptr=fopen(targets[0],"rw");//Only passwd file to be checked for directory
    char line[stringlength],line1[stringlength];    
    while(fgets(line,stringlength,fptr))
    {
        if((count(line)==6)&&chdir(substring(line,findIndex(line,':',5)+1,findIndex(line,':',6)-1))==-1)
            printf("user '%s': directory '%s' does not exist\n",
            substring(line,0,findIndex(line,':',1)-1),//Fetching username
            substring(line,findIndex(line,':',5)+1,findIndex(line,':',6)-1));  //Fetching invalid directory
    }     
    fclose(fptr);
}
