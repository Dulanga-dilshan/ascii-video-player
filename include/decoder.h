#ifndef DECODER_H
#define DECODER_H

#include<stdio.h>
#include<stddef.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<stdbool.h>

#define MAX_TERMINAL_WIDTH 1800
#define MAX_TERMINAL_HEIGHT 850
#define ASCII_Gradient " .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$"
#define FPS 60

#define FRAME_HEIGHT 700
#define FRAME_WIDTH 1300

typedef struct bmp{
    __uint32_t size;
    __uint32_t offset;
    __uint32_t width;
    __uint32_t height;
    __uint32_t bits_per_pixcel;
    __uint32_t compression;
    size_t out_width;
    size_t out_heigth;

    unsigned char* full_header;
    unsigned char* pixcels;
    unsigned char* blackandwhite;
    unsigned char* terminal_out;
    
}bmp;

bmp* read_bmp(char* file_parth);
void free_bmp(bmp* img);
void genarete_BW_img(bmp *img);
void write_new_bmp(bmp *img);
void scale_down(bmp* img);
unsigned char block_brightness(unsigned char* base,size_t block_height,size_t block_width,size_t img_width);
void print_img(bmp* img);
int round_f(float val);
void convert_to_one_channel(bmp* img);
void block_size(bmp* img,size_t* width,size_t* height);

void prosess_vid_with_ffmpeg(const char* file_name);
void remove_frames();
void play_frames();
void f_sleep(double fps);
void ffmpeg_play(char* filename);

#endif // DECODER_H
