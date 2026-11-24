#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "CustomFunctions.h"

using namespace std;

int main() {

    //Read file
    std::string filename = "input2D_float.txt";
    auto data = readFile(filename);

    bool running = true;

    //Ask user which function they'd like to use:
    while (running) {
        int choice;
        std::cout << "Please enter your function choice: \n" 
                  << "1. Print N lines of the file; or\n"
                  << "2. Calculate and print the magnitudes of every data point; or\n"
                  << "3. Perform a least squares fit to the data and print the resulting line equation; or\n"
                  << "4. Calculate x^y for each data point (with y rounded to nearest integer), and print the results: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                printStuff(data);
                break;

            case 2: {
                auto magnitudes = calculateMagnitudes(data);
                std::cout << "All magnitudes:\n" << std::endl;
                printStuff(magnitudes);
                saveToFile(magnitudes, "magnitudes.txt");
                break;
            }

            case 3: {
                std::string error_filename = "error2D_float.txt";
                auto errors = readFile(error_filename);
                std::string equation;
                double chi_squared_value;
                std::tie(equation, chi_squared_value) = leastSquaresFit(data, errors);
                printStuff(equation, chi_squared_value);
                saveToFile(equation, chi_squared_value);
                break;
                }

            case 4: {
                std::vector<double> results;
                for (const auto& point : data) {
                    double result = x_power_y(point.first, point.second);
                    results.push_back(result);
                }
                std::cout << "\nResults of x^y for each data point:\n";
                printStuff(results);
                saveToFile(results, "x_power_y_results.txt");
                break;
            }

            default:
                std::cerr << "Invalid function choice.\n";
                break;
        }

        int continue_choice = 0;
        std::cout << "\nWould you like to select another function? (1 for Yes, 0 for No): ";
        std::cin >> continue_choice;
        std::cout << std::endl;
        running = (continue_choice == 1);
    }
    std::cout << "Exiting program.\n";
    return 0;
}