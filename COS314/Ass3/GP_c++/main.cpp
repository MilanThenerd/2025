#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>

// Structure to hold the individual model (which is an expression in this case)
struct Individual {
    std::string expression;  // Represent the mathematical expression (e.g., "x1 + x2 - x3")
    double fitness;          // Fitness of the individual based on error
};

// Function to read CSV file into a vector of data points (features and target)
std::vector<std::vector<double>> readCSV(const std::string& filename) {
    std::vector<std::vector<double>> data;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::vector<double> row;
        size_t pos = 0;
        while ((pos = line.find(',')) != std::string::npos) {
            row.push_back(std::stod(line.substr(0, pos)));
            line.erase(0, pos + 1);
        }
        row.push_back(std::stod(line));  // Add the output as the last value
        data.push_back(row);
    }
    return data;
}

// Function to generate a random mathematical expression
std::string generateRandomExpression() {
    // A simple implementation for generating a random expression with input variables x1, x2, x3, etc.
    return "x1 + x2 - x3";  // Modify this to generate more complex expressions
}

// Function to initialize a population of random individuals
std::vector<Individual> initializePopulation(int populationSize) {
    std::vector<Individual> population;
    for (int i = 0; i < populationSize; i++) {
        Individual individual;
        individual.expression = generateRandomExpression();
        individual.fitness = 0.0;
        population.push_back(individual);
    }
    return population;
}



// Function to evaluate the expression (this is simplified for demonstration purposes)
double evaluateExpression(const std::string& expression, const std::vector<double>& row) {
    // Replace variables like x1, x2 with corresponding values from the row
    double x1 = row[0], x2 = row[1], x3 = row[2], x4 = row[3], x5 = row[4];
    // For simplicity, assuming the expression is a sum of variables
    return x1 + x2 - x3;
}

// Function to calculate the fitness of an individual using Mean Squared Error (MSE)
double calculateFitness(const Individual& individual, const std::vector<std::vector<double>>& data) {
    double mse = 0.0;
    for (const auto& row : data) {
        double predicted = evaluateExpression(individual.expression, row);  // Predict output
        double actual = row.back();  // Actual output from the dataset
        mse += std::pow(predicted - actual, 2);
    }
    mse /= data.size();
    return mse;
}


// Function for selection (roulette wheel or tournament selection)
Individual selectIndividual(const std::vector<Individual>& population) {
    // Simplified selection: select the individual with the best fitness
    Individual best = population[0];
    for (const auto& individual : population) {
        if (individual.fitness < best.fitness) {
            best = individual;
        }
    }
    return best;
}

// Function to perform crossover (combine two individuals)
Individual crossover(const Individual& parent1, const Individual& parent2) {
    Individual offspring;
    // Randomly combine parts of parent1 and parent2 expressions
    offspring.expression = parent1.expression.substr(0, parent1.expression.length() / 2) +
                          parent2.expression.substr(parent2.expression.length() / 2);
    return offspring;
}

// Function to perform mutation (randomly modify an individual’s expression)
void mutate(Individual& individual) {
    // Randomly modify the expression, e.g., changing an operator or variable
    individual.expression = "x1 * x2 + x3";  // Just an example, modify as needed
}

// Function to evolve the population using genetic programming
void evolvePopulation(std::vector<Individual>& population, const std::vector<std::vector<double>>& data) {
    // Evaluate fitness for each individual
    for (auto& individual : population) {
        individual.fitness = calculateFitness(individual, data);
    }

    // Create a new population
    std::vector<Individual> newPopulation;
    for (int i = 0; i < population.size(); i++) {
        // Select parents
        Individual parent1 = selectIndividual(population);
        Individual parent2 = selectIndividual(population);

        // Perform crossover
        Individual offspring = crossover(parent1, parent2);

        // Perform mutation
        mutate(offspring);

        newPopulation.push_back(offspring);
    }

    population = newPopulation;
}

// Function to evaluate the model on test data and calculate accuracy
double evaluateAccuracy(const std::vector<Individual>& population, const std::vector<std::vector<double>>& testData) {
    int correctPredictions = 0;
    int total = testData.size();

    // Using the best individual for evaluation
    Individual best = selectIndividual(population);
    for (const auto& row : testData) {
        double predicted = evaluateExpression(best.expression, row);
        double actual = row.back();
        if ((predicted >= 0.5 && actual == 1) || (predicted < 0.5 && actual == 0)) {
            correctPredictions++;
        }
    }

    return (correctPredictions / static_cast<double>(total)) * 100;
}

int main() {
    srand(time(0));  // Seed for random number generation

    // Load training data and test data
    std::vector<std::vector<double>> trainData = readCSV("BTC_train.csv");
    std::vector<std::vector<double>> testData = readCSV("BTC_test.csv");

    // Initialize the population of models
    int populationSize = 50;
    std::vector<Individual> population = initializePopulation(populationSize);

    // Evolve the population for a number of generations
    int generations = 100;
    for (int i = 0; i < generations; i++) {
        evolvePopulation(population, trainData);
        std::cout << "Generation " << i + 1 << " evolved!" << std::endl;
    }

    // Evaluate the accuracy on test data
    double accuracy = evaluateAccuracy(population, testData);
    std::cout << "Test Data Accuracy: " << accuracy << "%" << std::endl;

    return 0;
}
