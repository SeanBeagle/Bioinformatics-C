/******************************************************************************
  FILE: fa.c
 ABOUT: Returns JSON of FastA summary statistics.
AUTHOR: Sean Beagle
 EMAIL: SeanBeagle@gmail.com
   URL: http://www.SeanBeagle.com
        http://www.StrongLab.org
******************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>

// FORWARD DECLARATIONS: 
void newRecord();
void printRecordStats();
void printFastaStats();
void readFasta();
void clearRecordResidues();
bool isHeader(char *line);

// GLOBAL VARIABLES:
FILE *file;
char filename[255];
unsigned int num_records = 0;
unsigned long total_residues[256], total_size = 0; 
unsigned long record_residues[256], record_size = 0;

// MAIN:
int main(int argc, char *argv[]) {
  if (argc == 2 && (file = fopen(argv[1], "r"))) {
    strcpy(filename, argv[1]);
    readFasta();
    fclose(file);
  } else {
    return EXIT_FAILURE;
  }
}

void readFasta() {
  clearRecordResidues();

  printf("{\"records\": [");

  char *line;
  size_t len = 0;

  while ((getline(&line, &len, file)) != -1) {
    if (isHeader(line)) {
      if (num_records > 0) { 
        printRecordStats();
        printf(", ");
      }
      // remove last char if newline
      char last_char = line[strlen(line)-1];
      line[strlen(line)-1] = (last_char == '\n') ? '\0' : last_char;
      // begin printing record
      printf("{\"id\": \"%s\"", line+1);
      newRecord();
    } else {
      total_size += strlen(line) - 1;
      record_size += strlen(line) - 1;
      int residue, i = 0;
      while ((residue = toupper(line[i++])) != '\0' && residue != '\n') {
        ++record_residues[residue];
        ++total_residues[residue];
      }
    }
  }
  printRecordStats();
  printf("], ");
  printFastaStats();
}

/* Clear previous records data */
void newRecord() {
  num_records++;
  record_size = 0;
  clearRecordResidues();
}

bool isHeader(char *line) {
  return line[0] == '>';
}

void clearRecordResidues() {
  for (int i = 0; i < 256; i++)
    record_residues[i] = 0;
}

void printRecordStats() {
  for (int i = 0; i < 256; i++) {
    if (record_residues[i] > 0) printf(", \"%c\": %lu", i, record_residues[i]);
  }
    printf(", \"basepairs\": %d}", record_size);
}

void printFastaStats() {
  printf("\"fasta\": {\"%s\"", filename);
  for (int i = 0; i < 256; i++) {
    if (total_residues[i] > 0) printf(", \"%c\": %lu", i, total_residues[i]);
  }
  printf(", \"basepairs\": %d}\n", total_size);
}
