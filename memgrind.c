#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mymalloc.h"

double testCaseA() {
    
    int x, y;
    double time_total = 0;
<<<<<<< HEAD
    time_t start,end;
=======
    struct timeval start;
    struct timeval end;    
>>>>>>> ce26c9a9e8b5d348bfc8b3315be2bab65a97fe02
    char *pointer;
    
    // take the average of 100 trials
    for (x = 0; x < 100; x++) {
        
        // start the timer
<<<<<<< HEAD
        start = time(NULL);
=======
        gettimeofday(&start, 0);
>>>>>>> ce26c9a9e8b5d348bfc8b3315be2bab65a97fe02
        
        // malloc/free 150 times consecutively
        for (y = 0; y < 150; y++) {
            
            // allocate 1 byte and immediately free it
            pointer = malloc(1);
            free(pointer);
            
        }        
<<<<<<< HEAD
        // end the timer
        end = time(NULL);        
=======
       //end the timer
        gettimeofday(&end, 0);
        
>>>>>>> ce26c9a9e8b5d348bfc8b3315be2bab65a97fe02
        // calculate the duration of a single malloc/free call and add to total
        time_total +=((end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec));      
    }    
    // return the average time elapsed
    return time_total/100;
    
}


double testCaseB() {
    int x, y, z, j, s=0, e=50;
    double time_total = 0;
<<<<<<< HEAD
    time_t start, end;
=======
    struct timeval start;
    struct timeval end;
>>>>>>> ce26c9a9e8b5d348bfc8b3315be2bab65a97fe02
    char *pointers[50]; //  
    // take the average of 100 trials
    for(x=0; x<100; x++){
        s = 0;
        e = 50;
        //start the timer
<<<<<<< HEAD
        start = time(NULL);
=======
        gettimeofday(&start, 0);
>>>>>>> ce26c9a9e8b5d348bfc8b3315be2bab65a97fe02
        
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
        }
<<<<<<< HEAD
        end = time(NULL);
=======
        //end the timer
        gettimeofday(&end, 0);
        
>>>>>>> ce26c9a9e8b5d348bfc8b3315be2bab65a97fe02
        // calculate the duration of a single malloc/free call and add to total
        time_total +=((end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec));
    }
    
    return time_total/100;
}



double testCaseC(){
    int x, r, y, h, l;
<<<<<<< HEAD
    time_t start, end;
=======
    struct timeval start;
    struct timeval end;
>>>>>>> ce26c9a9e8b5d348bfc8b3315be2bab65a97fe02
    char* pointers[50];
    double time_total = 0;
    for(x=0; x<100; x++){
        y=0;
        l=0;
<<<<<<< HEAD
        start = time(NULL);
=======
        gettimeofday(&start, 0);
>>>>>>> ce26c9a9e8b5d348bfc8b3315be2bab65a97fe02
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
<<<<<<< HEAD
        end = time(NULL);
=======
       //end the timer
        gettimeofday(&end, 0);
>>>>>>> ce26c9a9e8b5d348bfc8b3315be2bab65a97fe02
        
        // calculate the duration of a single malloc/free call and add to total
        time_total +=((end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec));

    }
    
    // return the average time elapsed
    return time_total/100;
}


double testCaseD(){
    int x, r, y, h, capacity, l;
<<<<<<< HEAD
    time_t start, end;
=======
    struct timeval start;
    struct timeval end;
>>>>>>> ce26c9a9e8b5d348bfc8b3315be2bab65a97fe02
    char* pointers[50];
    double time_total = 0;
    
    // take the average of 100 trials
    for(x=0; x<100; x++){
        y=0;
        l=0;
        
        //start the timer
<<<<<<< HEAD
        start = time(NULL);
=======
        gettimeofday(&start, 0);
>>>>>>> ce26c9a9e8b5d348bfc8b3315be2bab65a97fe02
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
        
<<<<<<< HEAD
        //end the timer
        end = time(NULL);
=======
       //end the timer
        gettimeofday(&end, 0);
>>>>>>> ce26c9a9e8b5d348bfc8b3315be2bab65a97fe02
        
        // calculate the duration of a single malloc/free call and add to total
        time_total +=((end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec));
    }
    
    // return the average time elapsed
    return time_total/100;
    
}


double testCaseE(){
    int x, y, w, k, z;
<<<<<<< HEAD
    time_t start, end;
=======
    struct timeval start;
    struct timeval end;
>>>>>>> ce26c9a9e8b5d348bfc8b3315be2bab65a97fe02
    char* pointers[49];
    double time_total = 0;
    
    for(x=0; x<100; x++){
        
        //start the timer
<<<<<<< HEAD
        start = time(NULL);
=======
        gettimeofday(&start, 0);
>>>>>>> ce26c9a9e8b5d348bfc8b3315be2bab65a97fe02
        
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
        
<<<<<<< HEAD
        //end the timer
        end = time(NULL);
=======
       //end the timer
        gettimeofday(&end, 0);
>>>>>>> ce26c9a9e8b5d348bfc8b3315be2bab65a97fe02
        
        // calculate the duration of a single malloc/free call and add to total
        time_total +=((end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec));
    }
    
    // return the average time elapsed
    return time_total/100;
}


<<<<<<< HEAD
double testcaseF(){
    int x, y, w, k, z;
    time_t start, end;
=======
double testCaseF(){
    int x, y, w, k, z;
    struct timeval start;
    struct timeval end;
>>>>>>> ce26c9a9e8b5d348bfc8b3315be2bab65a97fe02
    char* pointers[49];
    double time_total = 0;
    
    for(x=0; x<100; x++){
        
        //start the timer
<<<<<<< HEAD
        start = time(NULL);
=======
        gettimeofday(&start, 0);
>>>>>>> ce26c9a9e8b5d348bfc8b3315be2bab65a97fe02
        
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
        
<<<<<<< HEAD
        //end the timer
        end = time(NULL);
=======
       //end the timer
        gettimeofday(&end, 0);
>>>>>>> ce26c9a9e8b5d348bfc8b3315be2bab65a97fe02
        
        // calculate the duration of a single malloc/free call and add to total
        time_total +=((end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec));
    }
    
    // return the average time elapsed
    return time_total/100;
}


int main(int argc, char* argv[]) {
    printf("Testcase A took on average %.2f microseconds to complete.\n", testCaseA());
    printf("Testcase B took on average %.2f microseconds to complete.\n", testCaseB());
    printf("Testcase C took on average %.2f microseconds to complete.\n", testCaseC());
    printf("Testcase D took on average %.2f microseconds to complete.\n", testCaseD());
    printf("Testcase E took on average %.2f microseconds to complete.\n", testCaseE());
    printf("Testcase F took on average %.2f microseconds to complete.\n", testCaseF());
    return 0;
}

