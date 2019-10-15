#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mymalloc.c"
#include "mymalloc.h"

double testcaseA() {
    
    int x, y;
    double time_total = 0;
    clock_t start, end;
    
    // take the average of 100 trials
    for (x = 0; x < 100; x++) {
        
        // start the timer
        start = clock();
        
        // malloc/free 150 times consecutively
        for (y = 0; y < 150; y++) {
            
            // allocate 1 byte and immediately free it
            char* pointer = malloc(1);
            myfree(pointer);
            
        }
        
        // end the timer
        end = clock();
        
        // calculate the duration of a single malloc/free call and add to total
        time_total += (double)(end - start)/CLOCKS_PER_SEC;
        
    }
    
    // return the average time elapsed
    printf("%lf", time_total/100);
    return time_total/100;
    
}


double testcaseB() {
    int x, y, z, j, s=0, e=50;
    double time_total = 0;
    clock_t start, end;
    char* pointers[150];
    
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
                printf("%d", y);
                
                pointers[y] = (char*)malloc(1);
                
                //Null check in case pointer fails
                if (pointers[y] == NULL) {
                    return -1;
                }
            }
            
            //free 50 pointers one by one
            for(z=s; z<e; z++){
                myfree(pointers[z]);
            }
            s += 50;
            e += 50;
            
            //end the timer
            
            
        }
        end = clock();
        
        // calculate the duration of a single malloc/free call and add to total
        time_total += (double)(end - start)/CLOCKS_PER_SEC;
    }
    
    printf("%lf", time_total/100);
    return time_total/100;
}


