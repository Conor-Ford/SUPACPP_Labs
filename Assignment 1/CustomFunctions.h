#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <utility>

// void printAllFileContents(const std::string& filename);
void printFileContents(const std::vector<std::pair<double, double>>& data);
std::vector<std::pair<double, double>> readFile(const std::string& filename);
std::vector<double> calculateMagnitudes(const std::vector<std::pair<double, double>>& data);
std::pair<std::string, double> leastSquaresFit(const std::vector<std::pair<double, double>>& data, 
                            const std::vector<std::pair<double, double>>& errors);
double chiSquaredTest(const std::vector<std::pair<double, double>>& errors, const std::vector<std::pair<double, double>>& data, double p, double q);
void printStuff(const std::vector<std::pair<double, double>>& data);
void printStuff(const std::vector<double>& magnitudes);
void printStuff(std::string equation, double chi_squared_value);
double x_power_y(double x, double y);
void saveToFile(const std::vector<double>& values, const std::string& filename);
void saveToFile(std::string equation, double chi_squared_value, const std::string& filename = "least_squares_fit.txt");