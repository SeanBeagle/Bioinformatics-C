#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>

// GLOBALS
std::string PROGRAM;     // filename of this executable (used in help function)
int MIN_DEPTH = 4;       // minimum depth for a call
float THRESHOLD = 0.75;  // minimum ratio of call depth to total depth
char AMBIGUOUS = '-';    // used in place for ambiguous calls (eg. A,T,C) -N in args for "N"

// String Functions
std::string changeExtension(std::string &fileName, const std::string &ext) {
    return fileName.substr(0, fileName.find_last_of(".")).append(".").append(ext);
}

bool startsWith(std::string &str, const std::string &target) {
    return (str.size() >= target.size()) && (0 == str.compare(0, target.size(), target));    
}

bool endsWith(std::string &str, const std::string &target) {
    return (str.size() >= target.size()) && (0 == str.compare(str.size()-target.size(), target.size(), target)); 
}

bool contains(std::string &str, const std::string &target) {
    return str.find(target) != std::string::npos;
}

/*split string on char and return vector of sub-strings*/
std::vector<std::string> split(std::string &str, char delimiter) {
    std::vector<std::string> tokens;
    int index = 0;
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == delimiter || i == str.size() - 1) {
            int size = (i == str.size() - 1) ? str.size()-index : i - index;
            if (index != i) {
                tokens.emplace_back(str.substr(index, size));
            }
            index = i + 1;
        }
    }   
    return tokens;
}

/*Prints program information/usage and exits*/
void help() {
    std::cout << "AUTHOR:\n\tSean Beagle\n\thttp://www.SeanBeagle.com\n\thttp://www.StrongLab.org\n";
    std::cout << "ABOUT:\n" << "\tTranslates VCF to FastA file format using filtering criteria\n"; 
    std::cout << "USAGE:\n";
    std::cout << "\t" << PROGRAM << " <input.vcf>\n";
    std::cout << "\t" << PROGRAM << " <input.vcf> <output.fasta>\n"; 
    std::cout << "\t" << PROGRAM << " --vcf <input.vcf> --fasta <output.fasta>\n";
    exit(1);
}

std::vector<int> getDP4(std::string &str) {
    // DP=7;MQ0F=0;AF1=0;AC1=0;DP4=2,0,0,0;MQ=23;FQ=-32.9934
    int start = str.find("DP4=") + 4;
    int size = str.find(";", start) - start + 1;
    std::string subStr = str.substr(start, size);
    std::vector<std::string> splitStr = split(subStr, ',');

    std::vector<int> dp4;
    dp4.emplace_back(std::stoi(splitStr.at(0)) + std::stoi(splitStr.at(1)));
    dp4.emplace_back(std::stoi(splitStr.at(2)) + std::stoi(splitStr.at(3)));
    return dp4;
}

void toFasta(std::string &fileName) {
    long size;
    char* sequence;
    int start;
    int stop;
    
    std::fstream file(fileName);
    std::string line;
    int i = -1;
    while (getline(file, line)) {
        // get sequence length and create template
        if (startsWith(line, "##contig")) {
            start = line.find("length=") + 7;
            size = std::stoi(line.substr(start, line.size()-start-1));
            std::cout << size << "\n";
            sequence = new char[size] {'-'};
            for (int i=0; i<size; i++) {
                sequence[i] = '-';
            }
        // extract pos, alt, ref, and dp4 from line 
        } else if (!startsWith(line, "#") && !contains(line, "INDEL")) {
            i++;
            std::vector<std::string> sLine = split(line, '\t');
            int pos = std::stoi(sLine.at(1)) - 1;
            std::string ref = sLine.at(3);
            std::string alt = sLine.at(4);
            std::string info = sLine.at(7);
            std::vector<int> dp4 = getDP4(info);
            int refDepth = dp4.at(0);
            int altDepth = dp4.at(1);
            int totalDepth = dp4.at(0) + dp4.at(1);
            if (refDepth >= MIN_DEPTH && refDepth >= totalDepth * THRESHOLD) {
                sequence[pos] = ref.size() == 1 ? ref[0] : AMBIGUOUS;
            } else if (altDepth >= MIN_DEPTH && altDepth >= totalDepth * THRESHOLD) {
                sequence[pos] = alt.size() == 1 ? alt[0] : AMBIGUOUS;
            } else {
                sequence[pos] = '-';
            }
            // std::cout << pos << " = " <<sequence[pos] << "\n";           
        }
    }
    std::cout << "DONE CREATING SEQUENCE";
}



int main(int argc, char* argv[]) { 
    std::string vcf;
    std::string fasta;
    PROGRAM = std::string(argv[0]);
    PROGRAM = PROGRAM.substr(PROGRAM.find_last_of("/") + 1);

    if (argc == 1) {
        help();
    } else if (argc == 2 && argv[1][0] != '-') {
        vcf = std::string(argv[1]);
    } else if (argc == 2 && argv[1][0] == '-') {
        help();
    } else if (argc == 3 && argv[1][0] != '-' && argv[2][0] != '-') {
            vcf = std::string(argv[1]);
            fasta = std::string(argv[2]);
    } else {
        for (int i=1; i < argc; i++) {
            if (std::string(argv[i]) == "--vcf" && i+1 <= argc) {
                vcf = std::string(argv[++i]);
            } else if (std::string(argv[i]) == "--fasta" && i+1 <= argc) {
                fasta = std::string(argv[++i]);
            }
        }
    }
    if (vcf.length() == 0) { help(); }
    if (fasta.length() == 0) {
        fasta = changeExtension(vcf, "fasta");
    }
    std::printf("  VCF: %s\n", vcf.c_str());
    std::printf("FASTA: %s\n", fasta.c_str());
    toFasta(vcf);
    return 0;
}
