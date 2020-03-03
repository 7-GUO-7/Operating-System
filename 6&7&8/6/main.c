#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4

/* the available amount of each resource */
int available[NUMBER_OF_RESOURCES];
/*the maximum demand of each customer */
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
/* the amount currently allocated to each customer */
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
/* the remaining need of each customer */
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];




void copy_vector(int *new_vec, int *old_vec){
    int i;
    for(i=0; i<NUMBER_OF_RESOURCES; i++)
        new_vec[i] = old_vec[i];
}


void add_or_sub(int *old_vec, int *new_vec, int sign){
    //sign is +1 then add, sign is -1 then sub
    int i;
    for(i=0; i<NUMBER_OF_RESOURCES; i++)
        old_vec[i] += new_vec[i] * sign;
}

// compare vector
int less_or_equal(int *left, int *right){
    int i;
    int flag = 0;
    for(i=0; i<NUMBER_OF_RESOURCES; i++)
        flag += (left[i] <= right[i]);
    return (flag == NUMBER_OF_RESOURCES);
}


// 1 if safe, 0 if unsafe
int whether_safe() {
    int work[NUMBER_OF_RESOURCES];
    int finish[NUMBER_OF_CUSTOMERS] = {0};
    int flag = 0;
    int finished = 0;
    int i, j;
    int judge;
    copy_vector(work, available);
    int in = 1;
    while (1 && in) {
	in = 0;
        for (i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
            if (finish[i] == 0 && less_or_equal(need[i], work)) {
                flag = 1;
		in = 1;
                add_or_sub(work, allocation[i], 1);
                finish[i] = 1;
                finished++;
            }
        }
        if (!flag && !in) {
            judge = 1;
            for (j = 0; j < NUMBER_OF_CUSTOMERS; j++) {
                if (finish[j] == 0)
                    judge = 0;

            }
            if (!judge && !in)
                return 0;
            else
                return 1;
        }
        flag = 0;

    
}
}



// update resources
void update(int customer_num, int release[], int sign){
    add_or_sub(available, release, sign);
    add_or_sub(allocation[customer_num], release, -sign);
    add_or_sub(need[customer_num], release, sign);
}

// request resources
int request_resources(int customer_num, int request[]){
    int flag;
    if(!less_or_equal(request, need[customer_num]))
    {
        printf("Denied, request exceeds need\n");
        return -1;
    }
    if(!less_or_equal(request, available))
    {
        printf("Denied, request larger than available\n");
        return -1;
    }
    update(customer_num, request, -1);
    if(whether_safe()) return 0;
    update(customer_num, request, 1);
    printf("Denied, it will cause deadlock\n");
    return -1;
}

// release resources
void release_resources(int customer_num, int release[]){
    update(customer_num, release, 1);
}

// print a vector
void print_vec(int v[NUMBER_OF_RESOURCES])
{
    int i;
    for(i=0; i<NUMBER_OF_RESOURCES; i++)
        printf("%d ", v[i]);
    printf("\n");
}

// print a matrix
void print_matrix(int m[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES])
{
    int i, j;
    for(i=0; i<NUMBER_OF_CUSTOMERS; i++)
    {
        for(j=0; j<NUMBER_OF_RESOURCES; j++)
            printf("%d ", m[i][j]);
        printf("\n");
    }
}

int main(int argc, char **args){
    FILE *file = fopen("max_table.txt", "r");

    int i, j;
    int request[NUMBER_OF_RESOURCES];
    char s[3];
    for(i=0; i<NUMBER_OF_CUSTOMERS; i++){
        for(j=0; j<NUMBER_OF_RESOURCES; j++){
            fscanf(file, ((j==NUMBER_OF_RESOURCES-1)?"%d\n":"%d,"), &maximum[i][j]);

            allocation[i][j] = 0;
            need[i][j] = maximum[i][j];

        }
    }
    fclose(file);
    for(j=0; j<NUMBER_OF_RESOURCES; j++)
        available[j] = ((j<argc-1)?atoi(args[j+1]):0);
    while(1){
        scanf("%s", s);
        if(s[0] == '*'){
            printf("Available:\n");
            print_vec(available);
            printf("Maximum matrix:\n");
            print_matrix(maximum);
            printf("Allocation matrix:\n");
            print_matrix(allocation);
            printf("Need matrix:\n");
            print_matrix(need);
        }
        else{
            scanf("%d", &j);
            for(i=0; i<NUMBER_OF_RESOURCES; i++)
                scanf("%d", &request[i]);
            if(s[1] == 'Q' || s[1] == 'q'){
                if(request_resources(j, request) == 0){
                    printf("Request satisfied\n");
                }
            }
            else if(s[1] == 'L' || s[1] == 'l'){
                if(less_or_equal(request, allocation[j])){
                    release_resources(j, request);
                    printf("Resource released\n");
                }
                else{
                    printf("Denied, release exceeds allocation\n");
                }
            }
//            else{
//                printf("Error\n");
//            }
        }
    }
    return 0;
}
