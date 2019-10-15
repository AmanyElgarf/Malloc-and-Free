#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mymalloc.h"

double testcaseA() {
    
    int x, y;
    double time_total = 0;
    clock_t start, end;
    char *pointer;
    
    // take the average of 100 trials
    for (x = 0; x < 100; x++) {
        
        // start the timer
        start = clock();
        
        // malloc/free 150 times consecutively
        for (y = 0; y < 150; y++) {
            
            // allocate 1 byte and immediately free it
            pointer = malloc(1);
            free(pointer);
            
        }        
        // end the timer
        end = clock();        
        // calculate the duration of a single malloc/free call and add to total
        time_total += (double)(end - start)/CLOCKS_PER_SEC;       
    }    
    // return the average time elapsed
    return time_total/100;
    
}


double testcaseB() {
    int x, y, z, j, s=0, e=50;
    double time_total = 0;
    clock_t start, end;
    char *pointers[50]; //  
    // take the average of 100 trials
    for(x=0; x<100; x++){
        s = 0;
        e = 50;
        //start the timer
        start = clock();
        
        //fill out the array in three chunks
        for(j=0; j<3; j++){
            
            //save 50 pointers in the array
            for(y=s; y<e; y++){
                
                pointers[y] = (char*)malloc(1);
                
                //Null check in case pointer fails
                if (pointers[y] == NULL) {
                    return -1;
                }
            }
            
            //free 50 pointers one by one
            for(z=s; z<e; z++){
                free(pointers[z]);
            }          
            //end the timer                       
        }
        end = clock();
        // calculate the duration of a single malloc/free call and add to total
        time_total += (double)(end - start)/CLOCKS_PER_SEC;
    }
    
    return time_total/100;
}



double testcaseC(){
    int x, r, y, h, l;
    clock_t start, end;
    char* pointers[50];
    double time_total = 0;
    for(x=0; x<100; x++){
        y=0;
        l=0;
        start = clock();
        while(1){
            r = rand() % 2;
            if(r==0){
                
                pointers[y] = (char*)malloc(1);
                if(pointers[y] == NULL){
                    return -1;
                }
                y++;
                l++;
                if(l == 50){break;}
            }
            else{
                if(y != 0){
                    free(pointers[y-1]);
                    y--;
                }
            }
        }
        for(h=0; h<y; h++){
            free(pointers[h]);
        }
        end = clock();
        
        // calculate the duration of a single malloc/free call and add to total
        time_total += (double)(end - start)/CLOCKS_PER_SEC;

    }
    
    // return the average time elapsed
    return time_total/100;
}


double testcaseD(){
    int x, r, y, h, capacity, l;
    clock_t start, end;
    char* pointers[50];
    double time_total = 0;
    
    // take the average of 100 trials
    for(x=0; x<100; x++){
        y=0;
        l=0;
        
        //start the timer
        start = clock();
        while(1){
            
            r = rand() % 2; //randomly choose between malloc and free
            
            if(r==0){
                capacity = rand() % 63 + 1; //choose random number between 1 and 64
                pointers[y] = (char*)malloc(capacity);
                if(pointers[y] == NULL){  //Null check
                    return -1;
                }
                y++;
                l++;
                if(l == 50){break;}

            }
            else{
                if(y != 0){
                    free(pointers[y-1]);
                    y--;
                }
            }
        }
        for(h=0; h<y; h++){
            free(pointers[h]);
        }
        
        //end the timer
        end = clock();
        
        // calculate the duration of a single malloc/free call and add to total
        time_total += (double)(end - start)/CLOCKS_PER_SEC;
    }
    
    // return the average time elapsed
    return time_total/100;
    
}


double testcaseE(){
    int x, y, w, k, z;
    clock_t start, end;
    char* pointers[49];
    double time_total = 0;
    
    for(x=0; x<100; x++){
        
        //start the timer
        start = clock();
        
        for(y=0; y<49; y++){
            pointers[y] = (char*)malloc(60);
        }
        
        //freeing each other pointer
        for(w=0; w<49; w=w+2){
            free(pointers[w]);
        }
        
        //mallocing the freed pointer with space thats different from the original one
        for(k=0; k<49; k=k+2){
            pointers[k] = (char*)malloc(50);
        }
        
        //freeinng all pointers
        for(z=0; z<49; z++){
            free(pointers[z]);
        }
        
        //end the timer
        end = clock();
        
        // calculate the duration of a single malloc/free call and add to total
        time_total += (double)(end - start)/CLOCKS_PER_SEC;
    }
    
    // return the average time elapsed
    return time_total/100;
}


<<<<<<< HEAD

=======
>>>>>>> c109d14459dbc917c1d031e5a59a03e4a5751468
double testcaseF(){
    int x, y, w, k, z;
    clock_t start, end;
    char* pointers[49];
    double time_total = 0;
    
    for(x=0; x<100; x++){
        
        //start the timer
        start = clock();
        
        for(y=0; y<49; y++){
            pointers[y] = (char*)malloc(60);
        }
        
        //freeing each other pointer
        for(w=0; w<49; w=w+2){
            free(pointers[w]);
        }
        
        //mallocing the freed pointer with space thats different from the original one
        for(k=0; k<49; k=k+2){
            pointers[k] = (char*)malloc(30);
        }
        
        //freeinng all pointers
        for(z=0; z<49; z++){
            free(pointers[z]);
        }
        
        //end the timer
        end = clock();
        
        // calculate the duration of a single malloc/free call and add to total
        time_total += (double)(end - start)/CLOCKS_PER_SEC;
    }
    
    // return the average time elapsed
    return time_total/100;
}


int main(int argc, char* argv[]) {
	double a = testcaseA();
	double b = testcaseB();
	double c = testcaseC();
	double d = testcaseD();
	double e = testcaseE();
	double f = testcaseF();
    printf("Testcase A took on average %.2f microseconds to complete.\n",(double) a*1000000);
    printf("Testcase B took on average %.2f microseconds to complete.\n", (double)b*1000000);
    printf("Testcase C took on average %.2f microseconds to complete.\n", (double)c*1000000);
    printf("Testcase D took on average %.2f microseconds to complete.\n", (double)d*1000000);
    printf("Testcase E took on average %.2f microseconds to complete.\n",(double) e*1000000);
    printf("Testcase F took on average %.2f microseconds to complete.\n", (double)f*1000000);
	return 0;
}

