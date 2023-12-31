#include "p3200211-p3200025-p3200103.h"


int charge_customer(int i, unsigned int seedp, float P_fail);
int income=0;
int plain=0;
int special=0;
int successful=0;
int unsuccessful=0;
int no_plain_pizzas = 0;
int no_special_pizzas = 0;
int total = 0;


void *pizzeria(void *order_id){
    int *oid;
    oid = (int *)order_id;
    unsigned int seedp = seed^(*oid); // initialize a seed unique for each thread
    int rc;
    struct timespec order_start; // when the order is given
    struct timespec order_prep; // when the order starts being prepared
    struct timespec order_bake; // when the order is inserted in the oven
    struct timespec order_baked; // when the order is out of the oven
    struct timespec order_pack_start; // when the packing process starts
    struct timespec order_packed; //when the packing process finish
    struct timespec order_delivery_start; //when the delivery starts
    struct timespec order_delivered; //when the order is delivered;
    


        
    // Charge the customer
    int flag = charge_customer(*oid , seedp , Pfail);
    if (flag==1){
        successful ++;
    }else{
        unsuccessful ++;
    }
    
    if (flag==1) {
    
        //get the time the order was given
        clock_gettime(CLOCK_REALTIME, &order_start);
        
        printf("Taking order %d.\n", *oid);
        //WAITING TIME: find a cook to take the order
        rc = pthread_mutex_lock(&mutex_no_available_cooks);
        
        if (rc != 0) {
            printf("Mutex: Error in locking with code %d\n", rc);
                pthread_exit(&rc);
            }
        while (no_available_cooks <= 0) {
                rc = pthread_cond_wait(&cond_no_available_cooks, &mutex_no_available_cooks);
                
                if (rc != 0) {
                    printf("Mutex: Error in condition wait with code %d\n", rc);
                    pthread_exit(&rc);
                }
        }
        printf("Cook was found for order %d.\n", *oid);
        --no_available_cooks; // available cook was found and taken for the order
        rc = pthread_mutex_unlock(&mutex_no_available_cooks);
        
        if (rc != 0) {
                printf("Mutex: Error in unlocking with code %d\n", rc);
                pthread_exit(&rc);
        }
        //get the time the order was started to be prepared
        clock_gettime(CLOCK_REALTIME, &order_prep);
        //PREPARATION TIME: how many pizzas need to be prepared
        int no_pizzas = rand_r(&seedp) % N_order_high + N_order_low;
        for (int i = 0; i < no_pizzas; i++) {
            double random_value = (double)rand() / (double)RAND_MAX; // Generate a random value between 0 and 1
    
            if (random_value < Pplain) {
                no_plain_pizzas++;
            }
        }
        no_special_pizzas = no_pizzas - no_plain_pizzas;  // Remaining pizzas are special
        plain += no_plain_pizzas;
        special += no_special_pizzas;
        total += no_pizzas;


    
        //CHARGE PROCESS : Sleep for a random duration between Tpaymentlow and Tpaymenthigh SO THAT THE CHARGING PROCESS ENDS
        int sleep_duration = rand_r(&seedp)% (T_payment_high + T_payment_low);
        sleep(sleep_duration);
        income += C_plain*no_plain_pizzas + C_special*no_special_pizzas;
        
        no_plain_pizzas = 0;
        no_special_pizzas = 0;

        //prepare each pizza for T_prep duration
        sleep(no_pizzas * T_prep);
        printf("%d pizzas prepared for order %d.\n", no_pizzas, *oid);
        //BAKE TIME: firstly wait for an oven to be empty for the pizzas
        rc = pthread_mutex_lock(&mutex_no_available_ovens);
        
        if (rc != 0) {
            printf("Mutex: Error in locking with code %d\n", rc);
            pthread_exit(&rc);
        }
        while (no_available_ovens <= 0) {
            rc = pthread_cond_wait(&cond_no_available_ovens, &mutex_no_available_ovens);
            
            if (rc != 0) {
                printf("Mutex: Error in condition wait with code %d\n", rc);
                pthread_exit(&rc);
            }
        }
        printf("Oven was found for order %d, starting to bake...\n", *oid);
        --no_available_ovens; // empty oven was found and pizzas were inserted for the baking
        rc = pthread_mutex_unlock(&mutex_no_available_ovens);
        
        if (rc != 0) {
            printf("Mutex: Error in unlocking with code %d\n", rc);
            pthread_exit(&rc);
        }
        //get the time the pizzas were inserted in the oven
        clock_gettime(CLOCK_REALTIME, &order_bake);
        //BAKE TIME: baking is simulated with the use of sleep for a standard duration and the baker is busy until the pizzas are done and out of the oven
        sleep(T_bake);
        printf("Pizzas for order %d are done, ready for take-away!\n", *oid);

        //Oven is released because the pizzas are done
        rc = pthread_mutex_lock(&mutex_no_available_ovens);
        
        if (rc != 0) {
            printf("Mutex: Error in locking with code %d\n", rc);
            pthread_exit(&rc);
        }
        ++no_available_ovens;
        pthread_cond_signal(&cond_no_available_ovens);
        rc = pthread_mutex_unlock(&mutex_no_available_ovens);
        
        if (rc != 0) {
            printf("Mutex: Error in unlocking with code %d\n", rc);
            pthread_exit(&rc);
        }
        //get the time the pizzas were out of the oven
        clock_gettime(CLOCK_REALTIME, &order_baked);

        //The cook is also released because he took the pizzas out of the oven and passed them to the counter
        rc = pthread_mutex_lock(&mutex_no_available_cooks);
        
        if (rc != 0) {
            printf("Mutex: Error in locking with code %d\n", rc);
            pthread_exit(&rc);
        }
        ++no_available_cooks;
        pthread_cond_signal(&cond_no_available_cooks);
        rc = pthread_mutex_unlock(&mutex_no_available_cooks);
        
        if (rc != 0) {
            printf("Mutex: Error in unlocking with code %d\n", rc);
            pthread_exit(&rc);
        }

        //PACKING TIME : firstly wait for a packer to be available for the pizzas
        // Bind a packer for packing
        rc = pthread_mutex_lock(&mutex_no_available_packers);
        
        if (rc != 0) {
            printf("Mutex: Error in locking with code %d\n", rc);
            pthread_exit(&rc);
        }
        while (no_available_packers <= 0) {
            rc = pthread_cond_wait(&cond_no_available_packers, &mutex_no_available_packers);
            if (rc != 0) {
                printf("Mutex: Error in condition wait with code %d\n", rc);
                pthread_exit(&rc);
            }
        }
        printf("Packer is assigned for order %d, starting to pack...\n", *oid);
        --no_available_packers; // Packager assigned for packing
        
        rc = pthread_mutex_unlock(&mutex_no_available_packers);
        
        if (rc != 0) {
            printf("Mutex: Error in unlocking with code %d\n", rc);
            pthread_exit(&rc);
        }
        // Packing Time
        clock_gettime(CLOCK_REALTIME, &order_pack_start);
        int packing_time = no_pizzas * T_pack;
        sleep(packing_time); // Simulate packing time
        clock_gettime(CLOCK_REALTIME, &order_packed); // Update order_packed after packing

                
                //update ready order time
        rc = pthread_mutex_lock(&mutex_ready_order_time);
        
        if (rc != 0) {
            printf("Mutex: Error in locking with code %d\n", rc);
            pthread_exit(&rc);
        }
        rc = pthread_mutex_lock(&mutex_lock_screen);
        
        if (rc != 0) {
            printf("Mutex: Error in locking with code %d\n", rc);
            pthread_exit(&rc);
        }
        order_time = (order_packed.tv_sec - order_prep.tv_sec)+(order_prep.tv_sec - order_start.tv_sec);


        //print the order ready message
        print_order_ready(*oid, order_time);
        
        rc = pthread_mutex_unlock(&mutex_lock_screen);
        
        if (rc != 0) {
            printf("Mutex: Error in unlocking with code %d\n", rc);
            pthread_exit(&rc);
        }
        
        rc = pthread_mutex_unlock(&mutex_ready_order_time);
        if (rc != 0) {
            printf("Mutex: Error in unlocking with code %d\n", rc);
            pthread_exit(&rc);
        }
        
        
        
        //Packer is released because the packing is done
        rc = pthread_mutex_lock(&mutex_no_available_packers);
        if (rc != 0) {
            printf("Mutex: Error in locking with code %d\n", rc);
            pthread_exit(&rc);
        }
        ++no_available_packers;
        pthread_cond_signal(&cond_no_available_packers);
        
        rc = pthread_mutex_unlock(&mutex_no_available_packers);
        
        if (rc != 0) {
            printf("Mutex: Error in unlocking with code %d\n", rc);
            pthread_exit(&rc);
        }
        

        
        //DELIVERY TIME : firstly wait for a deliverer to be available for the pizzas
        // Bind a deliverer for delivery
        rc = pthread_mutex_lock(&mutex_no_available_deliverers);
        
        if (rc != 0) {
            printf("Mutex: Error in locking with code %d\n", rc);
            pthread_exit(&rc);
        }
        while (no_available_deliverers <= 0) {
            rc = pthread_cond_wait(&cond_no_available_deliverers, &mutex_no_available_deliverers);
            if (rc != 0) {
                printf("Mutex: Error in condition wait with code %d\n", rc);
                pthread_exit(&rc);
            }
        }
        printf("Deliverer is assigned for order %d, starting delivery...\n", *oid);
        --no_available_deliverers; // Deliverer assigned for delivery
        rc = pthread_mutex_unlock(&mutex_no_available_deliverers);
        if (rc != 0) {
            printf("Mutex: Error in unlocking with code %d\n", rc);
            pthread_exit(&rc);
        }
        // Generate random delivery time
        int deliverytime = rand_r(&seedp) % T_delhigh + T_dellow;
        // Delivery Time
        clock_gettime(CLOCK_REALTIME, &order_delivery_start);
        sleep(deliverytime);
        clock_gettime(CLOCK_REALTIME, &order_delivered); 
        
        
        
        rc = pthread_mutex_lock(&mutex_lock_screen);
        if (rc != 0) {
            printf("Mutex: Error in locking with code %d\n", rc);
            pthread_exit(&rc);
        }
        
        delivery_time = (order_time + (order_delivered.tv_sec - order_delivery_start.tv_sec));
        total_delivery_time += delivery_time;
        
        //print the order delivery message
        print_order_delivery(*oid, delivery_time);
        rc = pthread_mutex_unlock(&mutex_lock_screen);
        if (rc != 0) {
            printf("Mutex: Error in unlocking with code %d\n", rc);
            pthread_exit(&rc);
        }        


        //sleep until the deliverer is back
        sleep(deliverytime);
        
        //Deliverer is released because the delivery is done and deliverer is back
        rc = pthread_mutex_lock(&mutex_no_available_deliverers);
        if (rc != 0) {
            printf("Mutex: Error in locking with code %d\n", rc);
            pthread_exit(&rc);
        }
        ++no_available_deliverers;
        pthread_cond_signal(&cond_no_available_deliverers);
        rc = pthread_mutex_unlock(&mutex_no_available_deliverers);
        if (rc != 0) {
            printf("Mutex: Error in unlocking with code %d\n", rc);
            pthread_exit(&rc);
        }
        
                
        

        //update total order time
        
        rc = pthread_mutex_lock(&mutex_total_order_time);
        if (rc != 0) {
            printf("Mutex: Error in locking with code %d\n", rc);
            pthread_exit(&rc);
        }
        
        
        
                //update max waiting time
        rc = pthread_mutex_lock(&mutex_max_order_time);
        if (rc != 0) {
            printf("Mutex: Error in locking with code %d\n", rc);
            pthread_exit(&rc);
        }
        if (delivery_time > max_order_time) {
            max_order_time = delivery_time;
        }
        rc = pthread_mutex_unlock(&mutex_max_order_time);
        if (rc != 0) {
            printf("Mutex: Error in unlocking with code %d\n", rc);
            pthread_exit(&rc);
        }
        
                
        //update time from the end of the baking until delivery
        rc = pthread_mutex_lock(&mutex_secondpart_time);
        if (rc != 0) {
            printf("Mutex: Error in locking with code %d\n", rc);
            pthread_exit(&rc);
        }
 
        second_time = (order_delivered.tv_sec - order_pack_start.tv_sec);
        total_second_time += second_time;
        

        rc = pthread_mutex_unlock(&mutex_secondpart_time);
        if (rc != 0) {
            printf("Mutex: Error in unlocking with code %d\n", rc);
            pthread_exit(&rc);
        }
        
        //update second max time
        rc = pthread_mutex_lock(&mutex_max2_order_time);
        if (rc != 0) {
            printf("Mutex: Error in locking with code %d\n", rc);
            pthread_exit(&rc);
        }
        if (second_time > max2_order_time) {
            max2_order_time = second_time;
        }
        rc = pthread_mutex_unlock(&mutex_max2_order_time);
        if (rc != 0) {
            printf("Mutex: Error in unlocking with code %d\n", rc);
            pthread_exit(&rc);
        }
        
        
        rc = pthread_mutex_unlock(&mutex_total_order_time);
        if (rc != 0) {
            printf("Mutex: Error in locking with code %d\n", rc);
            pthread_exit(&rc);
        }
        
        
        
        pthread_exit(oid);
    }else {
        printf("Payment failed for order %d. The order cannot proceed further.\n",*oid);
        pthread_exit(NULL);
    }
}

int charge_customer(int i, unsigned int seedp, float P_fail) {
    int flag=1;
    // Check for failure
    if (rand_r(&seedp) % 100 < P_fail) {
       
        flag=0; // End the customer's request
    }else{
        printf("Payment was registered for order %d. \n",i);
    }
    return flag ;
}




int main(int argc, char *argv[]) {

    if(argc != 3) {
        printf("Parameter number error.\n\n");
        exit(-1);
    }

    int N_cust = atoi(argv[1]);
    if(N_cust <0) {
        printf("Customer number error: Negative number.\n\n");
        exit(-1);
    }

    seed = atoi(argv[2]);

    printf("\nWelcome to our Pizzeria!\nNumber of customers is : %d and the seed is : %d\n\n", N_cust, seed);

    //initialize variables to be used
    order_time = 0;
    delivery_time=0;
    second_time=0;
    total_order_time = 0;
    total_delivery_time=0;
    total_second_time=0;
    max_order_time = 0;
    max2_order_time=0;
    no_available_cooks = N_cook;
    no_available_ovens = N_oven;
    no_available_packers = N_packer;
    no_available_deliverers = N_deliverer;

    //initialize order id's
    int order_id[N_cust];
    for(int i=0;i<N_cust;++i)
    {
        order_id[i] = i+1;
    }

    //initialize order threads

    pthread_t *threads;
    threads = malloc(N_cust*sizeof(pthread_t));
    if(threads==NULL){
        printf("Error allocating memory.\n");
        exit(-1);
    }

    int rc;
    for(int i=0; i<N_cust; ++i)
    {
        rc = pthread_create(&threads[i], NULL, &pizzeria, &order_id[i]);
	    //simulate the waiting time for the next order to come
        int wait = rand_r(&seed) % (T_order_high + 1 - T_order_low) + T_order_low;
        sleep(wait);
        if (rc != 0) {
            printf("Error: pthread_create() returned %d\n", rc);
            exit(-1);
        }
    }

    //join threads
    void *status;
    for(int i=0; i<N_cust; i++)
    {
        rc = pthread_join(threads[i], &status);
        if (rc != 0) {
            printf("Error: pthread_join() returned %d on thread %d\n", rc, order_id[i]);
            exit(-1);
        }
    }

    //Print average order preparation time
    printf("\n\nAverage order time from the beginning of the order until delivery is %f", total_delivery_time/N_cust);
    //Print max order preparation time
    printf("\nMax order time is from the beginning of the order until delivery %f\n\n", max_order_time);

    printf("\n\nAverage order time from the moment the pizzas are baked until delivery is %f", total_second_time/N_cust);
    //Print max order preparation time
    printf("\nMax order time is from the moment the pizzas are baked until delivery %f\n\n", max2_order_time);

    //Print the statistics
    printf("Summary Statistics:\n");
    printf("Successful Orders: %d\n", successful);
    printf("Unsuccessful Orders: %d\n", unsuccessful);
    printf("Total Pizzas: %d\n", total);
    printf("Plain Pizzas: %d\n", plain);
    printf("Special Pizzas: %d\n", special);
    printf("Total Income: $%d\n", income);
    free(threads);
    

    return 1;
}
