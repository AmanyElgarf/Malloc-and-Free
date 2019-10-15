#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #include "mymalloc.c"
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
    char* pointers[69];
    double time_total = 0;
    
    for(x=0; x<100; x++){
        y=0;
        
        //start the timer
        start = clock();
        
        pointers[0] = (char*)malloc(60);
        while(pointers[y] != NULL){
            y++;
            pointers[y] = (char*)malloc(60);
            // printf("Allocated %d pointers of size 60, metadata size 21, can't allocate more\n",y);
        }
        //freeing each other pointer
        for(w=0; w<y; w=w+2){
            free(pointers[w]);
        }
        //mallocing the freed pointer with space thats different from the original one
        for(k=0; k<y; k=k+2){
            pointers[k] = (char*)malloc(50);
        }    
        //freeinng all pointers
        for(z=0; z<y; z++){
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


    double testcaseF(){
    int x, y, w, k, z;
    clock_t start, end;
    char* pointers[69];
    double time_total = 0;
    
    for(x=0; x<100; x++){
        y=0;
        
        //start the timer
        start = clock();
        
        pointers[0] = (char*)malloc(60);
        while(pointers[y] != NULL){
            y++;
            pointers[y] = (char*)malloc(60);
        }
        
        //freeing each other pointer
        for(w=0; w<y; w=w+2){
            free(pointers[w]);
        }
        
        //mallocing the freed pointer with space thats different from the original one
        for(k=0; k<y; k=k+2){
            pointers[k] = (char*)malloc(30);
        }
        
        //freeinng all pointers
        for(z=0; z<y; z++){
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
	double aaa = testcaseA();
	double bbb = testcaseB();
	double ccc = testcaseC();
	double ddd = testcaseD();
	double eee = testcaseE();
	double fff = testcaseF();
    printf("Testcase A took on average %.2f microseconds to complete.\n",(double) aaa*1000000);
    printf("Testcase B took on average %.2f microseconds to complete.\n", (double)bbb*1000000);
    printf("Testcase C took on average %.2f microseconds to complete.\n", (double)ccc*1000000);
    printf("Testcase D took on average %.2f microseconds to complete.\n", (double)ddd*1000000);
    printf("Testcase E took on average %.2f microseconds to complete.\n",(double) eee*1000000);
    printf("Testcase F took on average %.2f microseconds to complete.\n", (double)fff*1000000);
		// TESTCASE A
	// int counter;
	// int superCounter;
	// int *a;
	// double times[100];
	// clock_t begin;
	// clock_t end;
	// double time_spent=0;
	// double mean_time;
	// double total_time = 0;
	// for (superCounter = 0; superCounter<100;superCounter++) {
	// 	begin = clock();
	// 	for (counter=0;counter<150;counter++) {
	// 		a = malloc(1);
	// 		free(a);
	// 	}
	// 	end = clock();
	// 	time_spent =(double) (end-begin)/CLOCKS_PER_SEC;
	// 	times[superCounter] = time_spent;
	// 	total_time+=time_spent;
	// }
	// mean_time = (double)total_time/100;
	// printf("Mean time for case A is : %f microseconds\n",mean_time*1000000);

	// //TESTCASE B
	// time_spent=0;
	// total_time = 0;
	// int *pointers[50];
	// int k;
	// for (superCounter = 0; superCounter<100;superCounter++) {
	// 	begin = clock();
	// 	for (k=0;k<3;k++) {
	// 		for (counter=0;counter<50;counter++) {
	// 			pointers[counter] = malloc(1);
	// 		}
	// 		for (counter=0;counter<50;counter++){
	// 			free(pointers[counter]);
	// 		}
	// 	}
	// 	end = clock();
	// 	time_spent = (double)(end-begin)/CLOCKS_PER_SEC;
	// 	times[superCounter]=time_spent;
	// 	total_time+=time_spent;
	// }
	// mean_time = (double)total_time/100;
	// printf("Mean time for case B is : %f microseconds\n",mean_time*1000000);

	// //TESTCASE C
	// total_time = 0;
	// time_t t;
	// int calledMalloc=0;
	// int ind = 0;
	// srand((unsigned) time(&t));
	// int randNum;
	// for (superCounter=0;superCounter<100;superCounter++) {
	// 	time_spent = 0;
	// 	begin = clock();
	// 		while (calledMalloc<50) {
	// 			randNum = rand()%100;
	// 			if  (randNum>49) {
	// 				pointers[ind] = malloc(1);
	// 				ind++;
	// 				calledMalloc++;
	// 			} else  {
	// 				if (ind>1) {
	// 					free(pointers[ind-1]);
	// 					ind--;
	// 				}
	// 			}
	// 		}
	// 		while (ind>0) {
	// 			free(pointers[ind-1]);
	// 			ind--;
	// 		}
	// 		end = clock();
	// 		time_spent = (double)(end-begin)/CLOCKS_PER_SEC;
	// 		// printf("time spent is %f\n",time_spent);
	// 		total_time+= time_spent;
	// 		// printf("total_time is %f\n",total_time);
	// 		calledMalloc = 0;
	// }
	// mean_time = (double)total_time/100;
	// printf("Mean time for case C is : %f microseconds\n",mean_time*1000000);


	// //TESTCASE D
	// total_time = 0;
	// calledMalloc=0;
	// ind = 0;
	// srand((unsigned) time(&t));
	// for (superCounter=0;superCounter<100;superCounter++) {
	// 	time_spent = 0;
	// 	begin = clock();
	// 	while (calledMalloc<50) {
	// 		randNum = rand()%100;
	// 		if (randNum>49) {
	// 			pointers[ind] = malloc((rand()%64)+1);
	// 			ind ++;
	// 			calledMalloc ++;
	// 		} else {
	// 			if (ind >0 ) {
	// 				free(pointers[ind-1]);
	// 				ind --;
	// 			}
	// 		}
	// 	}
	// 	while (ind > 0) {
	// 		free(pointers[ind-1]);
	// 		ind--;
	// 	}
	// 	end = clock();
	// 	time_spent = (double)(end-begin)/CLOCKS_PER_SEC;
	// 	total_time += time_spent;
	// 	calledMalloc = 0;
	// }
	// mean_time = (double) total_time/100;
	// printf("Mean time for case D is : %f microseconds\n",mean_time*1000000);
	return 0;
}



