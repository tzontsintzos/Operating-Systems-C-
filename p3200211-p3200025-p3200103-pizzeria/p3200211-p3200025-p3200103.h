#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

const int N_cook = 2; //number of maximum cooks
const int N_oven = 15; //number of maximum ovens
const int N_packer = 2; //number of maximum employees
const int N_deliverer = 10; //number of maximum deliverers

const int T_order_low = 1; // [T_order_low , T_order_high] : time range in which a new order arrives
const int T_order_high = 3;

const int N_order_low = 1; // [N_order_low , N_order_high] : range of possible number of pizzas in each order
const int N_order_high = 3;

const int T_payment_low = 1; //[T_payment_low , T_payment_high] : time range in which a new transaction is completed
const int T_payment_high = 3;

const int C_plain = 10; //price of pizza
const int C_special = 12;


const int T_prep = 1; // preparation time for each pizza
const int T_bake = 10; // baking time for each pizza
const int T_pack = 1; // packing time

const int T_dellow = 5; //[T_dellow , T_delhigh] : time range for delivery
const int T_delhigh =15;

float Pfail = 0.1;  // Example declaration with a 10% failure rate
float Pplain = 0.6; // Example probability of ordering a plain pizza ,60%


//Declaration of variables
int no_available_cooks;
int no_available_ovens;
int no_available_packers;
int no_available_deliverers;
double order_time;
double delivery_time;
double second_time;
double total_order_time;
double total_delivery_time;
double total_second_time;
double max_order_time;
double max2_order_time;
unsigned int seed;


//Declaration of mutexes
pthread_mutex_t mutex_no_available_cooks = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_no_available_ovens = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_no_available_packers = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_no_available_deliverers = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutex_total_order_time = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutex_ready_order_time = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutex_max_order_time = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutex_secondpart_time = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_max2_order_time = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutex_lock_screen = PTHREAD_MUTEX_INITIALIZER;

//Declaration of conditions
pthread_cond_t cond_no_available_cooks = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_no_available_ovens = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_no_available_packers = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_no_available_deliverers = PTHREAD_COND_INITIALIZER;

//The message that is printed when each order is ready
void print_order_ready(int oid, double minutes)
{
    printf("\nOrder with id %d was prepared in %f minutes.\n\n", oid, minutes);
}
//The message that is printed when each order is delivered
void print_order_delivery(int oid , double minutes){
    printf("\nOrder with id %d was delivered in %f minutes.\n\n", oid, minutes);
}
