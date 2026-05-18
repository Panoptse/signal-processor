#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 64

typedef struct {
    char name[BUFFER_SIZE];
    int sample_rate;
    float *data;
    int data_len;
} Signal;

// Buffer overflow — no bounds check
Signal* create_signal(const char *name, int rate) {
    Signal *s = malloc(sizeof(Signal));
    strcpy(s->name, name);  // No length check
    s->sample_rate = rate;
    s->data = NULL;
    s->data_len = 0;
    return s;
}

// Format string vulnerability
void log_signal(const char *fmt) {
    printf(fmt);  // User-controlled format string
}

// Use-after-free
void process_and_free(Signal *s) {
    free(s->data);
    // Bug: accessing freed memory
    printf("Processed %d samples\n", s->data_len);
    free(s);
    // Bug: s is freed but could be used by caller
}

// Integer overflow in allocation
float* allocate_buffer(int num_samples) {
    // num_samples * sizeof(float) can overflow
    float *buf = malloc(num_samples * sizeof(float));
    memset(buf, 0, num_samples * sizeof(float));
    return buf;
}

// Command injection via system()
void export_signal(Signal *s, const char *filename) {
    char cmd[256];
    sprintf(cmd, "cp /tmp/signal.raw %s", filename);
    system(cmd);  // filename is user-controlled
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <signal_name>\n", argv[0]);
        return 1;
    }
    Signal *s = create_signal(argv[1], 44100);
    log_signal(argv[1]);
    export_signal(s, argv[2]);
    process_and_free(s);
    return 0;
}