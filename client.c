#include<stdio.h>
#include<string.h>
#include"server.h"
void main(int argc,char **args){
    char dp[3];//Default param
    int opq=0;//Option -q 
    strcpy(dp,"--");//Providing some default value to agruments
    if(argc>=2){
        strcpy(dp,args[1]);//Copying arguments
        if((strcmp(args[1],"-q")==0))
            opq=1;
    }
    if(opq!=1)//To check for quite mode
        check_dir();
    format_checker(dp);
}
