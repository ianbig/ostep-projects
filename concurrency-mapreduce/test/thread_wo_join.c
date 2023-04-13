#include <pthread.h>

void * work(void * args) {
}


int main() {
	pthread_t t;
	pthread_create(&t, NULL, work, NULL);
}
