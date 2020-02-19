#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Params {
    bool no_headers;
};

struct Params params;

struct Fasta {
    FILE *file;
    char *filename;
    unsigned long contigs, basepairs, A, T, C, G;
};
struct Fasta fa;

void read_fasta();
float gc_content();

int main(int argc, char *argv[]) {
    if (argc == 1) { /* Check for filename argument */
        printf("Usage: %s [FILE]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (fa.file = fopen(argv[1], "r")) { /* This is a file */
        fa.filename = argv[1];
        read_fasta();
    } else { /* This is not a file */
        printf("ERROR: could not read %s\n", argv[1]);
        return EXIT_FAILURE;
    }
}

void read_fasta() {
    char *line = NULL;
    size_t len = 0;
    while((getline(&line, &len, fa.file)) != -1) {
        if (line[0] == '>') {
            ++fa.contigs;
        } else {
            fa.basepairs += strlen(line)-1;
            int c, i = 0;
            while((c = line[i++]) != '\0') {
                switch(c) {
                    case 'A': ++fa.A;
                    break;
                    case 'T': ++fa.T;
                    break;
                    case 'C': ++fa.C;
                    break;
                    case 'G': ++fa.G;
                    break;
                    default: break;
                }
            }
        }
    }
    fclose(fa.file);

    printf("contigs=%d,", fa.contigs);
    printf("basepairs=%d,", fa.basepairs);
    printf("A=%d,T=%d,C=%d,G=%d,", fa.A, fa.T, fa.C, fa.G);
    printf("GC=%.2f\n", gc_content());
}

float gc_content() {
    float atcg = fa.A + fa.T + fa.C + fa.G;
    float gc = fa.G + fa.C;
    if (atcg == 0 || gc == 0)
        return 0;
    return gc / atcg;
}
