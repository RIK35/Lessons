#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

volatile sig_atomic_t signal_received = 0;

void signal_handler(int sig) {
    signal_received = sig;
    printf("Received signal: %d (%s)\n", sig, strsignal(sig));
}

int main() {
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    // Установка обработчиков для SIGINT и SIGTERM
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    
    printf("Process PID: %d\n", getpid());
    printf("Send signals using: kill -SIGINT %d\n", getpid());
    
    while (!signal_received) {
        printf("Working...\n");
        sleep(1);
    }
    
    printf("Exiting due to signal %d\n", signal_received);
    return 0;
}