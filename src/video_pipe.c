#include "decoder.h"

int intlen(int num);
void tostr(char* buff,int num);
int power(int num,int p);
void strappend(char* destination,char* source);

void prosess_vid_with_ffmpeg(const char* file_name){
    char* cmnd=malloc(150);
    cmnd[0]='\0';
    strcat(cmnd,"ffmpeg -i src/mp4/\0");
    strcat(cmnd,file_name);
    strcat(cmnd," -t 120 -vf fps=30,scale=1300:700,format=bgr24 src/mp4/frames/frame_%d.bmp");
    printf("%s\n",cmnd);
    system(cmnd);
    free(cmnd);
}

void remove_frames(){
    system("make clean_frames");
}

void play_frames(){
    system("clear");
    char* file="src/mp4/frames/frame_\0";
    char f_num[10];
    char file_name[50];

    bmp* img;


    for(int i=1;i<=3600;i++){
        f_num[0]='\0';
        file_name[0]='\0';
        tostr(f_num,i);
        strappend(file_name,file);
        strappend(file_name,f_num);
        strappend(file_name,".bmp\0");
        img=read_bmp(file_name);
        genarete_BW_img(img);
        convert_to_one_channel(img);
        scale_down(img);
        print_img(img);
        
        free_bmp(img);
        f_sleep(FPS);
    }

    free_bmp(img);
}


void strappend(char* destination,char* source){
    int start=strlen(destination);
    int size=strlen(source);
    memcpy((char*)destination+start,source,size);
    destination[start+size]='\0';
}

int power(int num,int p){
    int tot=1;
    for(int i=1;i<=p;i++){
        tot*=num;
    }
    return tot;
}

void tostr(char* buff,int num){
    int len=intlen(num);
    int pow=power(10,len-1);
    int ans=0;
    int i=0;

    for(;i<len-1;i++){
        ans=num/pow;
        buff[i]=(char)(ans+'0');
        ans*=pow;
        num-=ans;
        pow/=10;
    }
    ans=num/pow;
    buff[i]=(char)(ans+'0');
    buff[i+1]='\0';
}

int intlen(int num){
    int lenth=0;
    while(num>0){
        num /= 10;
        lenth++;
    }
    return lenth;
}

/*sleep for fps*/
void f_sleep(double fps){
    double frame_time_sec = 1.0 / fps;
    long seconds = (long)frame_time_sec;
    long nanoseconds = (long)((frame_time_sec - seconds) * 1e9);

    struct timespec ts = { seconds, nanoseconds };
    nanosleep(&ts, NULL);
}


void ffmpeg_play(char* filename){
    FILE* file=fopen(filename,"rb");
    if(!file){
        perror(filename);
        exit(EXIT_FAILURE);
    }
    
    fclose(file);

    bmp frame;
    frame.bits_per_pixcel=24;
    frame.offset=54;
    size_t height=0;
    size_t width=0;
    size_t read_size=0;
    
    frame.height=FRAME_HEIGHT;
    frame.width=FRAME_WIDTH;
    size_t frame_size=FRAME_HEIGHT*FRAME_WIDTH;
    

    frame.blackandwhite=malloc(frame_size);
    if(frame.blackandwhite==NULL){
        perror("malloc failed");
        exit(3);
    }

    block_size(&frame,&width,&height);
    frame.terminal_out=malloc(frame.out_heigth*frame.out_width);
    if(frame.terminal_out==NULL){
        perror("malloc failed");
        exit(3);
    }

    char* cmd=malloc(150);
    cmd[0]='\0';
    strcat(cmd,"ffmpeg -i \0");
    strcat(cmd,filename);
    strcat(cmd," -vf scale=1300:700,vflip -pix_fmt gray -f rawvideo - 2>/dev/null");


    FILE* pipe=popen(cmd,"r");
    if(!pipe){
        perror("popen failed");
        exit(4);
    }

    width=0;

    while((read_size= fread(frame.blackandwhite ,1,frame_size,pipe)) == frame_size){
        scale_down(&frame);
        print_img(&frame);
        f_sleep(FPS);
    }

    free(frame.blackandwhite);
    free(frame.terminal_out);
    free(cmd);
    pclose(pipe);

    system("clear");

}


