#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <tuple>
#include <sstream>




void read (const std::string & name, const std::string & cation, std::vector<int> & cations_count, int cations_per_mol);

std::vector<int> sum (std::vector<int> & first, std::vector<int> & second, std::vector<int> & third);

void data_file_creation (const std::string & name, std::vector<int> & cations);


int main() {

    std::string file_name = "CoMs.pos";

    std::vector<int> Z, H30, H6O2, H9O4, OH, H;

    read(file_name, "H5O2", Z, 1);
    read(file_name, "H3O", H30, 1);
    read(file_name, "H6O2", H6O2, 2);
    read(file_name, "H9O4", H9O4, 1);


    read(file_name, "HO", OH, 1);
    read(file_name, "\tH\t", H, 1);

    H30 = std::move(sum(H30, H6O2, H9O4));

    data_file_creation("Zundels", Z);
    data_file_creation("H3O", H30);

    data_file_creation("HO", OH);
    data_file_creation("free_protons", H);

    return 0;
}


template <typename T>
std::string toString (T val) {
    std::ostringstream oss;
    oss << val;
    return oss.str();
}


void data_file_creation (const std::string & name, std::vector<int> & cations) {
    std::ofstream fout;
    fout.open(name, std::ios::trunc);
    for (int i = 0; i < cations.size()-1; ++i)
        fout << toString(i) << '\t' << toString(cations[i]) << '\n';
    fout.close();
}


unsigned long min (unsigned long a, unsigned long b, unsigned long c) {
    unsigned long tmp = std::min(a, b);
    return (tmp > c) ? c : tmp;
}


std::vector<int> sum (std::vector<int> & first, std::vector<int> & second, std::vector<int> & third) {
    int size = min(first.size(), second.size(), third.size());
    std::vector<int> result (size);
    for (int i = 0; i < size; ++i)
        result[i] = first[i] + second[i] + third[i];
    return result;
}


bool contain (const std::string & word, const std::string & line) {
    return line.find(word) != std::string::npos;
}


void read (const std::string & name, const std::string & cation, std::vector<int> & cations_count, int cations_per_mol) {
    std::string line;
    int i = 0;
    std::ifstream fin(name);
    if (!fin.is_open()) throw std::runtime_error("Error opening file.");
    while (!fin.eof())
        while (getline(fin, line)) {
            bool timestep_line = contain("Timestep", line);
            if (!(timestep_line || contain("ID", line))) {

                if (contain(cation, line)) cations_count[i] += cations_per_mol;

            } else if (timestep_line && !cations_count.empty()) {
                ++i;
            } else cations_count.resize(cations_count.size() + 1);
        }
}