#include "Genotype.h"
#include "Population.h"
#include <fstream>
#include <map>
#include <vector>

void print_semi_magic_square(std::vector<int> values, int dimension);
void create_input_data();

int main() {

    // Uncomment to create input files
    // create_input_data();

    // Create a new Population with genes consisting of three `double` variables
    Population<int> population("inputs\\normal_dimension_3.txt", 10000, 5000, 0.8, 0.35);

    // Find dimension of magic square based on the input file
    int variableCount = population.GetVariableCount();
    std::cout << variableCount;
    int dimension = sqrt(variableCount);
    // Check if input file makes sense for a square
    if (dimension * dimension != variableCount) {
        // TODO: Make this sentence make sense
        std::cerr << "ERROR: Input file doesnt contain a cleanly squared amount of values."
                  << std::endl;
        return -1;
    }

    // Evolve the population to maximize the given equation
    // In this case we try to find a semi magic square
    population.evolve([&](std::vector<int> genes) {
        // checking that no duplicates are present in the genes, because that would make the magic
        // square trivial
        // from https://stackoverflow.com/a/32238115
        std::map<int, int> geneDuplicates;
        for_each(genes.begin(), genes.end(), [&geneDuplicates](int val) { geneDuplicates[val]++; });
        int fitness = genes.size();
        // counting max duplicates
        int geneMax = 0;
        for (auto duplicate : geneDuplicates) {
            int count = duplicate.second;
            geneMax = geneMax < count ? count : geneMax;
        }
        // if there are duplicates we return them as negative value (= really bad fitness)
        if (geneMax > 1) {
            //+gene.size() because we want to always be >=0
            return genes.size() - geneMax;
        }

        std::vector<int> sums;
        // check rows
        for (int row = 0; row < genes.size(); row += dimension) {
            int sum = 0.0;
            // sum all values in the current row
            for (int col = 0; col < dimension; ++col) {
                sum += genes[row + col];
            }
            sums.push_back(sum);
        }
        // check cols
        for (int col = 0; col < dimension; ++col) {
            int sum = 0.0;
            for (int row = 0; row < genes.size(); row += dimension) {
                sum += genes[row + col];
            }
            sums.push_back(sum);
        }

        // finding biggest amount of duplicate values, because we want to maximize this
        // from https://stackoverflow.com/a/32238115
        std::map<int, int> sumDuplicates;

        for_each(sums.begin(), sums.end(), [&sumDuplicates](int val) { sumDuplicates[val]++; });

        int sumMax = 0;
        for (auto duplicate : sumDuplicates) {
            int count = duplicate.second;
            sumMax = sumMax < count ? count : sumMax;
        }
        //+gene.size() to be conform with earlier return
        return sumMax + genes.size();
    });

    // Print the result
    population.print_result();
    Genotype<int> bestMember = population.GetBestGenotype();

    print_semi_magic_square(bestMember.genes, dimension);
}

void print_semi_magic_square(std::vector<int> values, int dimension) {
    std::cout << std::endl << "Semi Magic Square:" << std::endl << std::endl;

    for (int row = 0; row < values.size(); row += dimension) {
        int sum = 0;
        for (int col = 0; col < dimension; ++col) {
            int curValue = values[row + col];
            if (curValue < 100) {
                std::cout << "0";
            }
            if (curValue < 10) {
                std::cout << "0";
            }
            std::cout << curValue << " ";
            sum += curValue;
        }

        std::cout << "- ";
        if (sum < 100) {
            std::cout << "0";
        }
        if (sum < 10) {
            std::cout << "0";
        }
        std::cout << sum << std::endl;
    }
    std::cout << " ";
    for (int i = 0; i < dimension; i++) {
        std::cout << "|   ";
    }
    std::cout << std::endl;
    for (int col = 0; col < dimension; ++col) {
        int sum = 0;
        for (int row = 0; row < values.size(); row += dimension) {
            int curValue = values[row + col];
            sum += curValue;
        }
        if (sum < 100) {
            std::cout << "0";
        }
        if (sum < 10) {
            std::cout << "0";
        }
        std::cout << sum << " ";
    }
    std::cout << std::endl;
}

void create_input_data() {
    // File writing from
    // https://www.sqlnethub.com/blog/how-to-write-to-text-file-from-c-plus-plus-program/

    // create data for each necessary dimension
    for (int dim = 3; dim <= 9; ++dim) {
        // maxvalue for a normal dataset is dimension sqaured
        int maxValue = dim * dim;
        // creating string for lower and upper bounds in necessary format
        std::string data = "1 " + std::to_string(maxValue);
        // open file for writing
        std::ofstream fw(("inputs\\normal_dimension_" + std::to_string(dim) + ".txt"),
                         std::ofstream::out);
        if (fw.is_open()) {
            fw << std::to_string(maxValue) << std::endl;
            // writing bounds as a line for every gene that is later neede
            for (int line = 0; line < maxValue; ++line) {
                fw << data << std::endl;
            }
            fw.close();
        } else
            std::cout << "Problem with opening file";
        for (int factor = 2; factor < 5; ++factor) {
            int nonNormalMaxvalue = maxValue * factor;
            std::string nonNormalData = "1 " + std::to_string(nonNormalMaxvalue);
            // open file for writing
            std::ofstream fw(("inputs\\notnormal_factor_" + std::to_string(factor) + "_dimension_" +
                              std::to_string(dim) + ".txt"),
                             std::ofstream::out);
            if (fw.is_open()) {
                fw << std::to_string(maxValue) << std::endl;
                for (int j = 0; j < maxValue; j++) {
                    fw << nonNormalData << "\n";
                }
                fw.close();
            } else
                std::cout << "Problem with opening file";
        }
    }
}