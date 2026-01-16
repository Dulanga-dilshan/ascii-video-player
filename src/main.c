#include<stdio.h>
#include<stdlib.h>
#include "decoder.h"

void help();

int main(int argc,char** argv){
    if(argc<2){
        printf("no input");
        help();
        exit(1);
    }
    ffmpeg_play(argv[1]);
    return 0;
}


void help(){
    const char* message = 
        "Usage :\n"
        "main [filename]\n"
    ;
    printf(message);
}
