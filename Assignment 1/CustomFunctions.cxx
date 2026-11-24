#include "CustomFunctions.h"
#include <iostream>
#include <fstream>
#include <cmath>

/*void printAllFileContents(const std::string& filename) {
    std::ifstream data_file(filename);

    if (!data_file.is_open()) {
        std::cerr << "Could not open file: " << filename;
        return;
    }

    std::string line;
    while (std::getline(data_file, line)) {
        std::cout << line << std::endl;
    }

    data_file.close();
}*/

//Initial function to read the file

std::vector<std::pair<double, double>> readFile(const std::string& filename) {
    std::ifstream data_file(filename);
    std::vector<std::pair<double, double>> data;

    if (!data_file.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return data; // Return empty vector
    }

    // Need to skip first line, which are just labelled column names x, y
    std::string line;
    std::getline(data_file, line); // Skip the first line

    // Rest of file is data in form "x, y", e.g. 1.3456, 2.3456:
    while (std::getline(data_file, line, ',')) {
        double x = std::stod(line); // Convert the first part to double
        if (std::getline(data_file, line)) { // Read the second part
            double y = std::stod(line); // Convert the second part to double
            data.emplace_back(x, y); // Store the pair in the vector
        }
    }
    std::cout << "\nSuccessfully read " << data.size() << " rows from the file.\n" << std::endl;

    data_file.close();
    return data;
}

// Option 2: Calculate magnitudes of each data point (x, y) as sqrt(x^2 + y^2)
std::vector<double> calculateMagnitudes(const std::vector<std::pair<double, double>>& data) {
    std::vector<double> magnitudes;
    for (const auto& point : data) {
        double magnitude = std::sqrt(point.first * point.first + point.second * point.second);
        magnitudes.push_back(magnitude);
    }
    return magnitudes;
}

// Option 3: Perform least squares fit to the data and return the equation and chi-squared value
std::pair<std::string, double> leastSquaresFit(const std::vector<std::pair<double, double>>& data, 
                            const std::vector<std::pair<double, double>>& errors) {

    // Least squares fit:
    // y = px + q
    // p = (N * Σ(xy) - Σx * Σy) / (N * Σ(x^2) - (Σx)^2)
    // q = (Σx^2 * Σy - Σx * Σ(xy)) / (N * Σ(x^2) - (Σx)^2)

    double sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_x2 = 0.0;
    int N = data.size();

    for (const auto& point : data) {
        sum_x += point.first;
        sum_y += point.second;
        sum_xy += point.first * point.second;
        sum_x2 += point.first * point.first;
    }

    double p = (N * sum_xy - sum_x * sum_y) / (N * sum_x2 - sum_x * sum_x);
    double q = (sum_x2 * sum_y - sum_x * sum_xy) / (N * sum_x2 - sum_x * sum_x);

    std::string equation = "y = " + std::to_string(p) + "x + " + std::to_string(q);

    //Chi-squared test:
    auto chi_squared_value = chiSquaredTest(errors, data, p, q);

    return std::make_pair(equation, chi_squared_value); // Return the equation and chi-squared values
}

double chiSquaredTest(const std::vector<std::pair<double, double>>& errors, const std::vector<std::pair<double, double>>& data, double p, double q) {
   double chi_squared_value = 0.0, expected, observed, error;

    // chi-squared = Σ((observed - expected)^2 / expected_error^2)
    // Want a chi-squared / degrees of freedom test (X^2 / NDOF)
    // errors has the form x_error, y_error. Data has the form x, y. Want to calculate chi-squared for each data point:
    chi_squared_value = 0.0;
    for (size_t i = 0; i < data.size(); ++i) {
        expected = p * data[i].first + q; // y = px + q
        observed = data[i].second; // y value
        auto error_squared = (errors[i].second * errors[i].second) + (p*p * errors[i].first * errors[i].first); // y_error^2 + (p^2 * x_error^2)

        chi_squared_value += (observed - expected) * (observed - expected) / error_squared;
    }

    double reduced_chi_squared = chi_squared_value / (data.size() - 2); // NDOF = N - 2 for linear fit

    return reduced_chi_squared; // Return the reduced chi-squared value
}

// Option 4: Calculate x^y for each data point (with y rounded to nearest integer)
double x_power_y(double x, double y) {
    // Want to calculate x^y for each data point, y rounded to nearest integer.
    // Going to do this using recursion (i.e. without pow function, a loop, or logarithms).
    int exp = static_cast<int>(std::round(y)); // Round y to nearest integer
    if (exp == 0) {
        return 1.0; // Base case: x^0 = 1
    } else if (exp < 0) { //Actually redundant since no negative values in input2D_float.txt, but just in case.
        return 1.0 / (x_power_y(x, -exp));
    } else {
        return x * x_power_y(x, exp - 1); // Recursive case: x^y = x * x^(y-1)
    }

}

// Print functions. Want to overload printStuff() to accept different types of input, e.g., string, vector, etc.

//Option 1
void printStuff(const std::vector<std::pair<double, double>>& data) {
    int n;
    std::cout << "Please enter the number of lines to print: ";
    std::cin >> n;
    std::cout << std::endl;
    if ((n <= 0) || (n > data.size())) {
        std::cout << "\nYou have selected a number beyond the available range, or else some other invalid input. Printing default 5 lines.\n";
        n = 5; // Default to 5 if invalid input
    }
    else {
        std::cout << "\n Printing " << n << " lines of the file:\n";
    }
    for (int i = 0; i < n && i < data.size(); ++i) {
        std::cout << data[i].first << ", " << data[i].second << std::endl;
    }
}

//Option 2 & 4
void printStuff(const std::vector<double>& values) {
    for (const auto& value : values) {
        std::cout << value << std::endl;
    }
}

//Option 3
void printStuff(std::string equation, double chi_squared_value) {
    std::cout << "Least squares fit equation: " << std::endl;
    std::cout << equation << std::endl;
    std::cout << "\nChi-squared value: " << std::endl;
    std::cout << chi_squared_value << std::endl;
    }

//Functions to save to files

//Options 2 & 4: (i.e., 2nd and 4th from the list)
void saveToFile(const std::vector<double>& values, const std::string& filename) {
    std::ofstream output_file(filename);
    if (output_file.is_open()) {
        for (const auto& value : values) {
            output_file << value << "\n";
        }
        output_file.close();
        std::cout << "\nValues saved to " << filename << std::endl;
    } else {
        std::cerr << "Could not open file to save values." << std::endl;
    }
}

//Option 3:
void saveToFile(std::string equation, double chi_squared_value, const std::string& filename) {
    std::ofstream output_file(filename);
    if (output_file.is_open()) {
        output_file << equation << "\n";
        output_file << "Reduced chi-squared (X^2 / NDOF): " << chi_squared_value << "\n";
        output_file.close();
        std::cout << "\nLeast squares fit equation & chi-squared error saved to " << filename << std::endl;
    } else {
        std::cerr << "Could not open file to save least squares fit data." << std::endl;
    }
}