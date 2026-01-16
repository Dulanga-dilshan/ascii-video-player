#include "decoder.h"

void read_bytes(FILE* file,size_t base,size_t n,__uint32_t* var){
    unsigned char buffer[10];
    if(base!=0){
        fseek(file,base,0);
    }
    fread(&buffer,1,n,file);
    *var=0;
    for(int i=0;i<(int)n;i++){
        *var |= (__uint32_t)buffer[i] << i*8;
    }
    rewind(file);
}

bmp* read_bmp(char* file_parth){
    FILE* file=fopen(file_parth,"rb");
    if(!file){
        perror(file_parth);
        exit(1);
    }
    unsigned char buff[4];
    fread(&buff,1,2,file);
    if(buff[0] != 'B' || buff[1] != 'M'){
        printf("invalid file format!");
        exit(2);
    }
    bmp* bmp_img=(bmp*)malloc(sizeof(bmp));

    read_bytes(file,2,4,&bmp_img->size);
    read_bytes(file,10,4,&bmp_img->offset);
    read_bytes(file,18,4,&bmp_img->width);
    read_bytes(file,22,4,&bmp_img->height);
    read_bytes(file,28,1,&bmp_img->bits_per_pixcel);
    read_bytes(file,30,4,&bmp_img->compression);

    int pixecel_data_size=bmp_img->size - bmp_img->offset;

    bmp_img->pixcels=(unsigned char*)malloc(sizeof(unsigned char)*pixecel_data_size);
    if(!bmp_img->pixcels){
        perror("malloc faild!");
        exit(3);
    }

    fseek(file,bmp_img->offset,0);
    fread(bmp_img->pixcels,1,pixecel_data_size,file);
    rewind(file);

    bmp_img->full_header=(unsigned char*)malloc(bmp_img->offset);
    if(!bmp_img->full_header){
        perror("malloc faild!");
        exit(3);
    }

    bmp_img->blackandwhite=NULL;
    bmp_img->terminal_out=NULL;

    fread(bmp_img->full_header,1,bmp_img->offset,file);
    fclose(file);
    return bmp_img;
}

void free_bmp(bmp* img){
    if(img->pixcels != NULL){
        free(img->pixcels);
    }
    if(img->blackandwhite != NULL){
        free(img->blackandwhite);
    }
    if(img->full_header != NULL){
        free(img->full_header);
    }
    if(img->terminal_out != NULL){
        free(img->terminal_out);
    }
    free(img);
    img=NULL;
}

void print_pixcel(unsigned char pixcel[]){
    printf("blue: %02x | green: %02x | red: %02x\n",pixcel[0],pixcel[1],pixcel[2]);
}

void genarete_BW_img(bmp *img){
    if(!img){
        perror("null pointer");
        exit(4);
    }
    size_t row_size=img->width*(img->bits_per_pixcel/8);
    size_t row_padding = (4 - (row_size % 4)) % 4;
    if(img->blackandwhite==NULL){
        img->blackandwhite=malloc(img->size-img->offset);
        if(img->blackandwhite==NULL){
            perror("malloc failed");
            exit(3);
        }
    }
    unsigned char pixcel[3]; //BGR
    
    for(size_t i=0;i<img->height;i+=1){
        for(size_t j=0;j<row_size;j+=3){
            size_t index=i*(row_size+row_padding)+j;
            pixcel[0]=img->pixcels[index]; //b
            pixcel[1]=img->pixcels[index+1]; //g
            pixcel[2]=img->pixcels[index+2]; //r
            //gray_scale = 0.299 * R + 0.587 * G + 0.114 * B
            unsigned char gray=(0.299*pixcel[2])+(0.587 *pixcel[1])+(0.114*pixcel[0]);
            img->blackandwhite[index]=img->blackandwhite[index+1]=img->blackandwhite[index+2]=gray;
        }
    }
}

void write_new_bmp(bmp* img){
    FILE* out=fopen("out/out.bmp","wb");
    if(!out){
        perror("out.bmp");
        exit(1);
    }
    unsigned char* data=(unsigned char*)malloc(img->size);
    if(!data){
        perror("malloc faild!");
        exit(3);
    }
    memcpy(data,img->full_header,img->offset);
    memcpy(data+img->offset,img->blackandwhite,img->size-img->offset);
    fwrite(data,1,img->size,out);
    fclose(out);
    free(data);
}


int round_f(float val){
    int base=(int)val;
    val-=(float)base;
    val*=10;
    if(val<5){
        return base;
    }
    return base+1;
}

void convert_to_one_channel(bmp* img){
    size_t img_size=img->width*img->height;
    __uint32_t row_size=img->width*3;
    row_size+=(4-(row_size% 4))%4;

    unsigned char* pixels=malloc(img_size);
    if(!pixels){
        perror("malloc failed");
        exit(3);
    }

    

    size_t index=0;
    for(__uint32_t i=0;i<img->height;i++){
        for(__uint32_t j=0;j<img->width*3;j+=3){
            pixels[index++]=img->blackandwhite[i*row_size+j];
        }
    }
    free(img->blackandwhite);
    img->blackandwhite=pixels;
    
}

void block_size(bmp* img,size_t* width,size_t* height){
    if(img->width>=MAX_TERMINAL_WIDTH && img->height>=MAX_TERMINAL_HEIGHT){
        *width=img->width/MAX_TERMINAL_WIDTH;
        *height=img->height/MAX_TERMINAL_HEIGHT;
        img->out_width=MAX_TERMINAL_WIDTH;
        img->out_heigth=MAX_TERMINAL_HEIGHT;

        if(img->width%MAX_TERMINAL_WIDTH!=0){
            *width+=1;
            img->out_width=(img->width-(img->width% (*width)))/ (*width);
        }
        if(img->height%MAX_TERMINAL_HEIGHT!=0){
            *height+=1;
            img->out_heigth=(img->height-(img->height% (*height)))/ (*height);
        }
    }else if(img->width<MAX_TERMINAL_WIDTH && img->height>MAX_TERMINAL_HEIGHT){
        *width=1;
        img->out_width=img->width;
        *height =img->height/MAX_TERMINAL_HEIGHT;
        img->out_heigth=MAX_TERMINAL_HEIGHT;
        if(img->height%MAX_TERMINAL_HEIGHT!=0){
            *height+=1;
            img->out_heigth=(img->height-(img->height% (*height)))/ (*height);
        }
    }else if(img->width>MAX_TERMINAL_WIDTH && img->height<MAX_TERMINAL_HEIGHT){
        *width=1;
        img->out_heigth=img->height;
        *width=img->width/MAX_TERMINAL_WIDTH;
        img->out_width=MAX_TERMINAL_WIDTH;

        if(img->width%MAX_TERMINAL_WIDTH!=0){
            *width+=1;
            img->out_width=(img->width-(img->width% (*width)))/ (*width);
        }
    }else{
        *width=1;
        *height=2;
        img->out_width=img->width;
        img->out_heigth=img->height/2;
    }
}

void scale_down(bmp* img){
    size_t  block_width=0,block_height=0;
    block_size(img,&block_width,&block_height);

    if(img->terminal_out==NULL){
        img->terminal_out=malloc(img->out_heigth*img->out_width);
        if(!img->terminal_out){
            perror("malloc faild!");
            exit(3);
        }
    }
    
    unsigned char block_avg=0;
    unsigned char* base;
    __uint32_t index=0,out_index=0;

    for(__int32_t i=img->height-block_height; i>0; i-=block_height){
        for(__int32_t j=0; j<(__int32_t)img->width; j+=block_width){
            index=i*img->out_width+j;
            base=img->blackandwhite+index;
            block_avg=block_brightness(base,block_height,block_width,img->width);
            img->terminal_out[out_index++]=block_avg;
        }
    }
}

unsigned char block_brightness(unsigned char* base,size_t block_height,size_t block_width,size_t img_width){
    size_t avg=0;
    size_t index=0;
    size_t count=0;
    for(size_t i=0;i<block_height;i++){
        for(size_t j=0;j<block_width;j++){
            index=(i*(img_width-block_width))+j;
            avg+=base[index];
            count++;
        }
    }
    avg/=count;
    return avg;
}

void print_img(bmp* img){
    size_t index=0;
    unsigned char out_char=0;
    float gardient_index=0;
    size_t ascii_len=strlen(ASCII_Gradient)-1;

    printf("\033[H");
    fflush(stdout);

    for(size_t i=0;i<img->out_heigth;i++){
        for(size_t j=0;j<img->out_width;j++){
            index=i*img->out_width+j;
            out_char=img->terminal_out[index];
            gardient_index=(float)out_char/256;
            gardient_index*=ascii_len;
            printf("%c",ASCII_Gradient[round_f(gardient_index)]);
        }
        printf("\n");
    }
}