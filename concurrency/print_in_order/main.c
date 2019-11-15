
/* 
 * Solve https://leetcode.com/problems/print-in-order
 * using pthread condition variable
*/


typedef struct {
    // User defined data may be declared here.
    pthread_mutex_t mutex;
    pthread_cond_t cond[2];

    int order;
} Foo;

Foo* fooCreate() {
    int i;
    Foo* obj = (Foo*) malloc(sizeof(Foo));

    // Initialize user defined data here.
    obj->order = 1;
    pthread_mutex_init( &obj->mutex, NULL);

    for (i = 0 ; i < 2 ; i++) {
        pthread_cond_init( &obj->cond[i], NULL);
    }

    return obj;
}

void first(Foo* obj) {

    pthread_mutex_lock( &(obj->mutex) );

    // printFirst() outputs "first". Do not change or remove this line.
    printFirst();
    obj->order++;

    pthread_cond_signal (&(obj->cond[0]));
    pthread_mutex_unlock (&obj->mutex);
}

void second(Foo* obj) {

    pthread_mutex_lock( &(obj->mutex) );
    while(obj->order != 2)
    {
        pthread_cond_wait(&(obj->cond[0]), &(obj->mutex));
    }

    // printSecond() outputs "second". Do not change or remove this line.
    printSecond();

    obj->order++;
    pthread_cond_signal (&(obj->cond[1]));
    pthread_mutex_unlock (&obj->mutex);
}

void third(Foo* obj) {

    pthread_mutex_lock( &(obj->mutex) );
    while(obj->order != 3)
    {
        pthread_cond_wait(&(obj->cond[1]), &(obj->mutex));
    }

    // printThird() outputs "third". Do not change or remove this line.
    printThird();
    pthread_mutex_unlock( &(obj->mutex) );

}

void fooFree(Foo* obj) {
    // User defined data may be cleaned up here.

    free(obj);
}
