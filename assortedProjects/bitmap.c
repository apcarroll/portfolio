#include "bmp_header.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

void create( int height, int width,FILE* fptr);

//check for empty files with fseek ftell


int main(int argc, char **argv){

/*
   add your code here
   */
     int height,i,j;
    int width;
    unsigned char c;
    char * end;
    char command[50];
  
    FILE* fptr;
    FILE* fptr2;
    FILE* output;
    if (argv[1]==NULL){
        printf("Usage : bitmap_tool <command> [argument 1] [argument 2] ...\n");
        exit(0);
    }
    strcpy(command,argv[1]);
      
    if(strcmp(command,"-create")==0){
        if (argc!=5){
            printf("Usage : bitmap_tool -create [height] [width] [output file name]\n");
            exit(0);
        }
    int tempa=atoi(argv[2]);
    int tempb=atoi(argv[3]);
    tempa=isdigit(tempa);   
    tempb=isdigit(tempb);
   
    height=atoi(argv[2]);
    if(height<=0||tempa!=0){
        printf("Invalid height given\n");
        exit(0);
    }
  
    width=atoi(argv[3]);
    if(width<=0||tempb!=0){
        printf("Invalid width given\n");
        exit(0);
    }
  
           fptr=fopen(argv[4],"wb");  
  

    create(height,width,fptr);
    }
    else if(strcmp(command,"-invertcolor")==0){
        if (argc!=4){
            printf("bitmap_tool -invertcolor [input file] [output file name]\n");
            exit(0);
        }
        if (fopen(argv[2],"rb")==NULL){
            printf("Unable to open BMP file %s\n",command);
            exit(0);
        }
        if(strcmp(argv[2],argv[3])==0){
            printf("Invalid file type in header for %s\n",argv[2]);
            exit(0);
        }
        fptr=fopen(argv[2],"rb");
        output=fopen(argv[3],"wb");
       


   
    struct header a;
    struct information b;
  
    fread(&a,14,1,fptr);
    fread(&b,40,1,fptr);
    fwrite(&a,14,1,output);
    fwrite(&b,40,1,output);
    height=lendianReadInt((unsigned char*)&b.height);
    width=lendianReadInt((unsigned char*)&b.width);
    for(i=0;i<width;i++){
            for(j=0;j<height;j++){
                     fread(&c,1,1,fptr);
             c=255-c;
             fwrite(&c,1,1,output);
             fread(&c,1,1,fptr);
             c=255-c;
             fwrite(&c,1,1,output);
             fread(&c,1,1,fptr);
             c=255-c;
             fwrite(&c,1,1,output);
        }
        }
    fclose(fptr);
    fclose(output);












    }
    else if(strcmp(command,"-stackvertically")==0){
       if(argc!=5){
        printf("Usage : bitmap_tool -stackvertically [input file 1] [input file 2] [output file name]\n");
        exit(0);
    }
    if (fopen(argv[2],"rb")==NULL){
            printf("Unable to open BMP file %s\n",argv[2]);
            exit(0);
        }
    if (fopen(argv[3],"rb")==NULL){
            printf("Unable to open BMP file %s\n",argv[3]);
            exit(0);
        }

    fptr=fopen(argv[2],"rb");
    fptr2=fopen(argv[3],"rb");
    output=fopen(argv[4],"wb");
    struct header a;
    struct information b;
    struct header c;
    struct information d;
    int heightb,widthb;
    fread(&a,14,1,fptr);
        fread(&b,40,1,fptr);
    fread(&c,14,1,fptr2);
        fread(&d,40,1,fptr2);
    unsigned int stuff;
    height=lendianReadInt((unsigned char*)&b.height);
    width=lendianReadInt((unsigned char*)&b.width);
    heightb=lendianReadInt((unsigned char*)&d.height);
    widthb=lendianReadInt((unsigned char*)&d.width);
    //printf("%d %d %d %d\n",height,width,heightb,widthb);
         if (height!=heightb){
        printf("Files %s %s do not have same dimensions, cannot continue\n",argv[2],argv[3]);
        exit(0);
    }
    if (width!=widthb){
        printf("Files %s %s do not have same dimensions, cannot continue\n",argv[2],argv[3]);
        exit(0);
    }
    heightb=height*2;
    //widthb=width*2;
    //printf("heightb %d widthb %d",heightb,widthb);
    stuff =54+(3*heightb*widthb);
    b.imagesize=lendianWriteInt((unsigned char*)&stuff);
        a.size=lendianWriteInt((unsigned char*)&stuff);
    b.height=lendianWriteInt((unsigned char*)&heightb);
    b.width=lendianWriteInt((unsigned char*)&widthb);
    fwrite(&a,14,1,output);
        fwrite(&b,40,1,output);
    /*works a little better
    for(i=0;i<(widthb);i++){
            for(j=0;j<(heightb);j++){
          
          if (j<=(heightb)){
       
        fread(&c,1,1,fptr);
                 fwrite(&c,1,1,output);
        fread(&c,1,1,fptr);
                 fwrite(&c,1,1,output);
        fread(&c,1,1,fptr);
                 fwrite(&c,1,1,output);
        }
           
         if (j>(heightb)){
           
        fread(&c,1,1,fptr2);
                 fwrite(&c,1,1,output);
        fread(&c,1,1,fptr2);
                 fwrite(&c,1,1,output);
        fread(&c,1,1,fptr2);
                 fwrite(&c,1,1,output);
        }
   

        }
    }
        */
   
   

    for(i=0;i<widthb;i++){
        for(j=0;j<(heightb/2);j++){
        fread(&c,1,1,fptr2);
                 fwrite(&c,1,1,output);
        fread(&c,1,1,fptr2);
                 fwrite(&c,1,1,output);
        fread(&c,1,1,fptr2);
                 fwrite(&c,1,1,output);

        }

    }
    for(i=0;i<widthb;i++){
        for(j=0;j<heightb;j++){
        fread(&c,1,1,fptr);
                 fwrite(&c,1,1,output);
        fread(&c,1,1,fptr);
                 fwrite(&c,1,1,output);
        fread(&c,1,1,fptr);
                 fwrite(&c,1,1,output);

        }

    }

      
    fclose(fptr);
    fclose(fptr2);
        fclose(output);
    exit(0);
   








    }
   












     else if(strcmp(command,"-stackhorizontally")==0){
           //dont forget to handel the errors
    if(argc!=5){
        printf("Usage : bitmap_tool -stackhorizontally [input file 1] [input file 2] [output file name]\n");
        exit(0);
    }
    if (fopen(argv[2],"rb")==NULL){
            printf("Unable to open BMP file %s\n",argv[2]);
            exit(0);
        }
    if (fopen(argv[3],"rb")==NULL){
            printf("Unable to open BMP file %s\n",argv[3]);
            exit(0);
        }
    fptr=fopen(argv[2],"rb");
    fptr2=fopen(argv[3],"rb");
    output=fopen(argv[4],"wb");
    struct header a;
    struct information b;
    struct header c;
    struct information d;
    int heightb,widthb;
    fread(&a,14,1,fptr);
        fread(&b,40,1,fptr);
    fread(&c,14,1,fptr2);
        fread(&d,40,1,fptr2);
    unsigned int stuff;
    height=lendianReadInt((unsigned char*)&b.height);
    width=lendianReadInt((unsigned char*)&b.width);
    heightb=lendianReadInt((unsigned char*)&d.height);
    widthb=lendianReadInt((unsigned char*)&d.width);
    //printf("%d %d %d %d\n",height,width,heightb,widthb);
         if (height!=heightb){
        printf("Files %s %s do not have same dimensions, cannot continue\n",argv[2],argv[3]);
        exit(0);
    }
    if (width!=widthb){
        printf("Files %s %s do not have same dimensions, cannot continue\n",argv[2],argv[3]);
        exit(0);
    }
    //heightb=height*2;
    widthb=width*2;
    stuff =54+(3*heightb*widthb);
    b.imagesize=lendianWriteInt((unsigned char*)&stuff);
        a.size=lendianWriteInt((unsigned char*)&stuff);
    b.height=lendianWriteInt((unsigned char*)&heightb);
    b.width=lendianWriteInt((unsigned char*)&widthb);
    fwrite(&a,14,1,output);
        fwrite(&b,40,1,output);
   
    for(i=0;i<(heightb);i++){
            for(j=0;j<(widthb/2);j++){
            
        fread(&c,1,1,fptr);
                 fwrite(&c,1,1,output);
        fread(&c,1,1,fptr);
                 fwrite(&c,1,1,output);
        fread(&c,1,1,fptr);
                 fwrite(&c,1,1,output);
        
        
           }
       for(j=0;j<(widthb/2);j++){
            
        fread(&c,1,1,fptr2);
                 fwrite(&c,1,1,output);
        fread(&c,1,1,fptr2);
                 fwrite(&c,1,1,output);
        fread(&c,1,1,fptr2);
                 fwrite(&c,1,1,output);
        
        
           }
        }
    fclose(fptr);
    fclose(fptr2);
        fclose(output);
    exit(0);
    }
 

































  else if(strcmp(command,"-drawborder")==0){
        if(argc!=8){
        printf("Usage : bitmap_tool -drawborder [input file] [thickness] [b] [g] [r] [output file]\n");
        exit(0);
    }
    if (fopen(argv[2],"rb")==NULL){
            printf("Unable to open BMP file %s\n",argv[2]);
            exit(0);
        }
    struct header a;
    struct information b;
    fptr=fopen(argv[2],"rb");
    output=fopen(argv[7],"wb");
    fread(&a,14,1,fptr);
        fread(&b,40,1,fptr);
    int thickness=atoi(argv[3]);
    int bl=atoi(argv[4]);
    int g=atoi(argv[5]);
    int r=atoi(argv[6]);
    int tempc=isdigit(bl);
    int tempd=isdigit(g);
    int tempe=isdigit(r);
    if(g<0||tempc!=0){
        printf("Invalid green component given\n");
        exit(0);
    }
    if(bl<0||tempc!=0){
        printf("Invalid blue component given\n");
        exit(0);
    }
    if(r<0||tempc!=0){
        printf("Invalid red component given\n");
        exit(0);
    }
    height=lendianReadInt((unsigned char*)&b.height);
    width=lendianReadInt((unsigned char*)&b.width);
    //printf("%d  %d %d/n",height,width,thickness);
    if(thickness>height||thickness>width){
        printf("Specified thickness greater than the dimensions of the image, cannot continue\n");
        exit(0);
    }
    if(thickness<0){
        printf("Invalid thickness\n");
        exit(0);
    }
    fwrite(&a,14,1,output);
        fwrite(&b,40,1,output);
    //you may need chars for rgb
   
    unsigned char blue= (unsigned char) bl;
    unsigned char green= (unsigned char) g;
    unsigned char red= (unsigned char) r;
   
   
    int count=0;
   
   
    for(i=0;i<height;i++){
        for (j=0;j<width;j++){
            if((i<thickness)||(i>(height-thickness))||(j<(thickness))||(j>(width-thickness))){
                fread(&c, 1, 1, fptr);
                fread(&c, 1, 1, fptr);
                fread(&c, 1, 1, fptr);
                fwrite(&blue,1,1,output);
                        fwrite(&green,1,1,output);
                        fwrite(&red,1,1,output);
            }
            else{
                     fread(&c,1,1,fptr);
                         fwrite(&c,1,1,output);
                        fread(&c,1,1,fptr);
                        fwrite(&c,1,1,output);
                        fread(&c,1,1,fptr);
                        fwrite(&c,1,1,output);


            }
        }
    }
    fclose(fptr);
    fclose(output);
   
    }











    else{
        printf("Usage : bitmap_tool <command> [argument 1] [argument 2] ...\n");    
    exit(0); 
    }
    return 0;

}

void create( int height, int width,FILE* fptr){

   
     

    //convert everything that is not zero

    struct header a;
    unsigned int arbitrary=40;
    unsigned short int  one=1;
    unsigned short int number=19778;
    int reszero=0;
    unsigned int zero=0;
    int i,j;  
    unsigned short int bits=24;
    unsigned int offset=54;
    a.type=lendianWriteShort((unsigned char*)&number);
    unsigned int size=((width*height*3)+54);
    a.size=lendianWriteInt((unsigned char*)&size);
    a.reserved1=lendianWriteShort((unsigned char*)&zero);
    a.reserved2=lendianWriteShort((unsigned char*)&zero);
    a.offset=lendianWriteInt((unsigned char*)&offset);
    fwrite(&a,14,1,fptr);

    struct information b;
    b.size=lendianWriteInt((unsigned char*)&arbitrary);  
    b.width=lendianWriteInt((unsigned char*)&width);
    b.height=lendianWriteInt((unsigned char*)&height);
    b.planes=lendianWriteShort((unsigned char*)&one);
    b.bits=lendianWriteShort((unsigned char*)&bits);
    b.compression=0;
    b.imagesize=lendianWriteInt((unsigned char*)&size);
    b.xresolution=lendianWriteInt((unsigned char*)&reszero);
    b.yresolution=lendianWriteInt((unsigned char*)&reszero);
    b.ncolors=lendianWriteInt((unsigned char*)&zero);
    b.importantcolors=lendianWriteInt((unsigned char*)&zero);
    fwrite(&b,40,1,fptr);
   
    unsigned char c=255;
    for(i=0;i<height;i++){
        for(j=0;j<width;j++){
            fwrite(&c,1,1,fptr);
         fwrite(&c,1,1,fptr);
         fwrite(&c,1,1,fptr);
       
          
        }
    }  
  
    //for (i = 0; i < (height * width * 3); i++){
    //    fwrite(&c, 1, 1, fptr);
    //}
    //fwrite(c, 1, (height * width * 3), fptr);
fclose(fptr);


}
