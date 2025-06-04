#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

timer_t timerid;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void handler(int sig, siginfo_t *si, void *uc) {
    write(STDOUT_FILENO, "Tick\n", 5);
}

// Потік для читання нового інтервалу
void* input_thread(void *arg) {
    char line[64];
    while (fgets(line, sizeof(line), stdin)) {
        int new_interval = atoi(line);
        if (new_interval > 0) {
            struct itimerspec its;
            its.it_value.tv_sec = new_interval;
            its.it_value.tv_nsec = 0;
            its.it_interval.tv_sec = new_interval;
            its.it_interval.tv_nsec = 0;

            pthread_mutex_lock(&lock);
            timer_settime(timerid, 0, &its, NULL);
            pthread_mutex_unlock(&lock);

            printf("Таймер оновлено: %d сек\n", new_interval);
        } else {
            printf("Невірне значення. Введіть ціле число > 0\n");
        }
    }
    return NULL;
}

int main() {
    struct sigaction sa = {0};
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigaction(SIGRTMIN, &sa, NULL);

    struct sigevent sev = {0};
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;

    timer_create(CLOCK_REALTIME, &sev, &timerid);

    struct itimerspec its;
    its.it_value.tv_sec = 1;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 1;
    its.it_interval.tv_nsec = 0;

    timer_settime(timerid, 0, &its, NULL);

    // Запускаємо окремий потік для вводу інтервалу
    pthread_t tid;
    pthread_create(&tid, NULL, input_thread, NULL);

    // Основний потік чекає сигнали
    while (1) pause();
}
