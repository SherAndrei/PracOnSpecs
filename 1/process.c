#include <stdio.h>
#include <unistd.h>
#include "find_incr.h"

static int from_root[2];
static int to_root[2];

struct Data {
    const char* filename;
    int result;
};

static void process_function(const char* filename, int m, int p) {
    FILE* inp;
    char byte;
    struct Data data;
    data.filename = filename;

    inp = fopen(filename, "r");
    if (!inp) {
        data.result = -1;
    } else {
        data.result = find_incr(inp);
    }

    if (read(from_root[0], &byte, 1) != 1) {
        fprintf(stderr, "Error reading in process %d out of %d, pid = %d\n",
                m, p, getpid());
        fclose(inp);
        return;
    }
    if (write (to_root[1], &data, sizeof (struct Data)) != sizeof (struct Data)) {
        fprintf(stderr, "Error writing in process %d out of %d, pid = %d\n",
                m, p, getpid());
        fclose(inp);
        return;
    }

    fclose(inp);
}

int main(int argc, const char* const argv[]) {
    int m, pid, total = 0;
    char byte;
    struct Data data;
    const int p = argc - 1;

    if (argc == 1) {
        printf("Usage: %s <files>\n", argv[0]);
        return -1;
    }

    if (pipe(from_root) == -1 || pipe(to_root) == -1) {
      fprintf(stderr, "Cannot pipe!\n");
      return -2;
    }

    for (m = 1; m <= p; m++) {
        pid = fork();
        if (pid == -1) {
            fprintf(stderr, "Cannot fork!\n");
            return -3 - m;
        } else if (pid == 0) {
            close(from_root[1]);
            close(to_root[0]);

            process_function(argv[m], m, p);

            close(from_root[0]);
            close(to_root[1]);
            return 0;
        }
    }

    close(from_root[0]);
    close(to_root[1]);

    pid = 0;
    for (m = 1; m <= p; m++) {
        byte = (char) m;
        if (write(from_root[1], &byte, 1) != 1) {
            fprintf(stderr, "Error writing in root process\n");
            return -100;
        }
        if (read (to_root[0], &data, sizeof (struct Data)) != sizeof (struct Data)) {
            fprintf(stderr, "Error reading in root process\n");
            return -101;
        }
        switch (data.result) {
        case -1:
            printf("Error opening file %s\n", data.filename);
            pid++;
            break;
        case -2:
            printf("Incorrect data in %s\n", data.filename);
            pid++;
            break;
        default:
            total += data.result;
            break;
        }
    }
    if (pid == 0)
        printf("Result: %d\n", total);
    else
        return -200;
    return 0;
}
