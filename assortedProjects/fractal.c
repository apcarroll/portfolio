#include <stdio.h>
#include <math.h>
#include <string.h>
#include "bmpfile.h"
// Make the rules global
// strdup the lhs and rhs
// rules[lhs]=rhs
//so rules['a']and such
//don't save and destroy until you are done
char comparison;
void draw(char  axiom[], int depth);
void drizzle(int numFractals);
void drawLine(int boolean);
double currentAngle;
void move();
double max(double herp, double derp);
int drib=0;
//a.out or a  < file.txt
char *rules[256];
char lhs;
char rhs[9001];
int top=0;

double minX,maxX,minY,maxY;
double lineLength=1;
double angle;
double heading;
int numRules;
int newbool=0;
typedef struct{
    double xOne;
    double yOne;
 
    double position;
    double turning;
}status;

status stack[256];
status curstat;

bmpfile_t *bmp;
rgb_pixel_t pixelW = {255, 255, 255, 0};
rgb_pixel_t pixelB = {0, 0, 0, 0};





int main(){
 
    inputs();
   
 
}
int inputs(){
    char c;
    int j=0;
    int numFractals;
    double startingAngle;
    int depth;
    char masterString[100][9001];
    //array of char pointers like argv
    char axiom[9001];
   // char rules [9001];// pass as a char**
    scanf("%d",&numFractals);
    drizzle(numFractals);
    int proxy=numFractals;
    int x=0;
    for (x=0;x<numFractals;x++){
        scanf("%d",&depth);
        scanf("%lf",&angle);//curstat.turning);
	
        scanf("%lf",&heading);
        getchar();// get rid of null
        scanf("%s",&axiom);
        scanf("%d",&numRules);
        curstat.turning=(angle*(M_PI/180));
	
	curstat.position=(heading*(M_PI/180));
	startingAngle=curstat.position;
	curstat.xOne=0;
        curstat.yOne=0;
	lineLength=1;
	
	
        int y;
	
        for (y=0;y<numRules;y++){
            scanf("%s -> %s\n",&lhs,rhs);
        //   printf("lhs is %c\n",lhs);
        //printf ("rhs is %s\n",rhs);
        rules[lhs]=strdup(rhs);
      //  printf("Old rule is %s\n",rules[lhs]);
       }
        maxX=curstat.xOne;
    	maxY=curstat.yOne;
    	minX=curstat.xOne;
    	minY=curstat.yOne;
	/*
	printf("The max x is %lf\n",maxX);
	printf("The max y is %lf\n",maxY);
	printf("The min x is %lf\n",minX);
	printf("The min y is %lf\n",minY);
	*/
	top=0;
	drib=0;
        draw(axiom,depth);
	double deltaX=(maxX-minX);
	double deltaY=(maxY-minY);
	double maximum = max(deltaX,deltaY);
	printf("The maximum is %lf\n",maximum);
	curstat.position=heading;
	top=0;
	lineLength=(300/maximum);
	
/*
	printf("Line length is %lf\n",lineLength);
	printf("The max x is %lf\n",maxX);
	printf("The max y is %lf\n",maxY);
	printf("The min x is %lf\n",minX);
	printf("The min y is %lf\n",minY);
*/
	curstat.xOne=fabs(minX*lineLength)+(300*x);
	curstat.yOne=fabs(minY*lineLength);
	printf("x is %lf\n",curstat.xOne);
	printf("y is %lf\n",curstat.yOne);
	drib=1;
	curstat.position=startingAngle;
	draw(axiom,depth);
	//numFractals--;
    }


 
 bmp_save(bmp, "OUTPUT.BMP");
    // free bmp object
    bmp_destroy(bmp);
}

void move(){
	double dx= (lineLength*cos(curstat.position));
        double dy= (lineLength*sin(curstat.position));
	dx=-dx;
	dy=-dy;

}

void draw(char  axiom[], int depth)
{
    
    int x;
    int boolean=0;
    int length=strlen(axiom);
    

    
    for (x=0;x<length;x++){
        comparison=axiom[x];
    
        if(rules[axiom[x]]!=NULL &&depth!=0){
     
            draw(rules[axiom[x]],depth-1);
       

        }
     else {
     
    
       switch(comparison){
            case 'F':
             boolean=1;
                 drawLine(boolean);
            
	 if(newbool==0){
            curstat.xOne+=(lineLength*cos(curstat.position));
             curstat.yOne+=(lineLength*sin(curstat.position));
            }
           
                break;
            case 'f':
              
            move();
                break;
            case '+':
                curstat.position-=curstat.turning;
       
          
                break;
            case '-':
          
                curstat.position+=curstat.turning;
                break;
            case '[':
          
                stack[top]=curstat;
            top++;
                break;
           case ']':
          
                top--;
            curstat=stack[top];
                break;
        }
	if (curstat.xOne>maxX){
		maxX=curstat.xOne;
	}
	if (curstat.yOne>maxY){
		maxY=curstat.yOne;
	}
	if (curstat.xOne<minX){
		minX=curstat.xOne;
	}
	if (curstat.yOne<minY){
		minY=curstat.yOne;
	}
    }

    }

}

void drawLine(int boolean){
    newbool=0;
    int q;
    double dx= (lineLength*cos(curstat.position));
    double dy= (lineLength*sin(curstat.position));
    //printf("Got To DrawLine\n");
    //printf("The boolean is %d\n",boolean);
   // printf("The dx is %lf\n",dx);
   // printf("The dy is %lf\n",dy);
//if(fabs(dx)>fabs(dy)){
    if(dx<0){
        newbool=1;
        curstat.xOne+=dx;
        curstat.yOne+=dy;
        dx=-dx;
        dy=-dy;
            if(boolean==1){

            //printf("here 1\n");
            for (q = 0; q<dx; q++){
		if (drib==1){
                      bmp_set_pixel(bmp,curstat.xOne+q,curstat.yOne+q*dy/dx, pixelW);
		}
            }
        boolean=0;
        }
        }
    else if(dx>0){
   
        if(boolean==1){
           // printf("here 2\n");
            for (q = 0; q<dx; q++){
                  //bmp_set_pixel(bmp,137 ,100 , pixelW);
			if (drib==1){
                      bmp_set_pixel(bmp,curstat.xOne+q,curstat.yOne+q*dy/dx, pixelW);
        	}    
	}
        boolean=0;
        }
        }
//}
    else if(dy<0){
        curstat.xOne+=dx;
        curstat.yOne+=dy;
        dx=-dx;
        dy=-dy;
        if(boolean==1){
           // printf("here 3\n");
            for (q = 0; q<dy; q++){
			if (drib==1){
                      bmp_set_pixel(bmp,curstat.xOne+q*dx/dy,curstat.yOne+q, pixelW);
        		}    
	}
        boolean=0;
        }
    }
    else{
   
        if(boolean==1){
           // printf("here 4\n");
            for (q = 0; q<dy; q++){
			if (drib==1){
                      bmp_set_pixel(bmp,curstat.xOne+q*dx/dy,curstat.yOne+q, pixelW);
        		}    
	}
        boolean=0;
        }
        }

}

void drizzle(int numFractals){
   // curstat.xOne=100;
  //  curstat.yOne=100;
    //numFractals=3;

    //printf("The number of fractals is %d\n",numFractals);

    int CX, CY;

    // create the bmp object having desired width, height and color depth
    bmp = bmp_create(300*numFractals, 300, 32);
    // clear the bmp to all black
    for (CX = 0; CX < 300*numFractals; CX++)
        for (CY = 0; CY < 300; CY++)
         bmp_set_pixel(bmp, CX, CY, pixelB);
    // draw a white line from top left to bottom right
    //for (CX = 0; CX < 300; CX++)
    //    bmp_set_pixel(bmp, CX, CX, pixelW);
    // save bitmap to demo.BMP
  /*
    bmp_set_pixel(bmp,137 ,100 , pixelW);
    bmp_set_pixel(bmp,125 , 125, pixelW);
    bmp_set_pixel(bmp, 150, 125, pixelW);
   */
}

double max(double herp, double derp){

	if (herp>derp){
	return herp;

	}
	else{
	return derp;

	}

}
