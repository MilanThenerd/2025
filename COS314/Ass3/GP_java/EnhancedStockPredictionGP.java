import java.io.*;
import java.util.*;
import java.util.function.Function;

public class EnhancedStockPredictionGP {

    // Enhanced Configuration
    private static final int POPULATION_SIZE = 1000;
    private static final int MAX_GENERATIONS = 200;
    private static final double CROSSOVER_RATE = 0.85;
    private static final double MUTATION_RATE = 0.3;
    private static final int TOURNAMENT_SIZE = 7;
    private static final int MAX_DEPTH = 8;
    private static final int LOOKBACK_WINDOW = 21;
    private static final Random random = new Random();

    // Data structures
    private List<double[]> normalizedTrainData;
    private List<double[]> normalizedValidationData;
    private List<double[]> normalizedTestData;
    private List<Function<double[][], Double>> population;
    private double[] featureMins;
    private double[] featureMaxs;

    public EnhancedStockPredictionGP(List<double[]> trainData, List<double[]> validationData, List<double[]> testData) {
        normalizeData(trainData, validationData, testData);
        this.population = new ArrayList<>();
    }

    private void normalizeData(List<double[]> trainData, List<double[]> validationData, List<double[]> testData) {
        int featureCount = trainData.get(0).length - 1;
        featureMins = new double[featureCount];
        featureMaxs = new double[featureCount];
        Arrays.fill(featureMins, Double.MAX_VALUE);
        Arrays.fill(featureMaxs, Double.MIN_VALUE);

        // Calculate min/max on training data only
        for (double[] row : trainData) {
            for (int i = 0; i < featureCount; i++) {
                featureMins[i] = Math.min(featureMins[i], row[i]);
                featureMaxs[i] = Math.max(featureMaxs[i], row[i]);
            }
        }

        // Normalize all datasets
        this.normalizedTrainData = normalizeDataset(trainData);
        this.normalizedValidationData = normalizeDataset(validationData);
        this.normalizedTestData = normalizeDataset(testData);
    }

    private List<double[]> normalizeDataset(List<double[]> data) {
        List<double[]> normalized = new ArrayList<>();
        for (double[] row : data) {
            double[] normRow = new double[row.length];
            for (int i = 0; i < row.length - 1; i++) {
                normRow[i] = (featureMaxs[i] - featureMins[i]) == 0 ? 0 : 
                    (row[i] - featureMins[i]) / (featureMaxs[i] - featureMins[i]);
            }
            normRow[row.length - 1] = row[row.length - 1]; // Don't normalize output
            normalized.add(normRow);
        }
        return normalized;
    }

    // Enhanced Terminal Set with Technical Indicators
    private List<Function<double[][], Double>> getTerminals() {
        return Arrays.asList(
            // Price data
            window -> window[window.length-1][0], // Open
            window -> window[window.length-1][1], // High
            window -> window[window.length-1][2], // Low
            window -> window[window.length-1][3], // Close
            window -> window[window.length-1][4], // Volume
            
            // Moving Averages
            window -> sma(window, 5, 3),   // 5-day SMA
            window -> sma(window, 10, 3),  // 10-day SMA
            window -> ema(window, 7, 3),   // 7-day EMA
            window -> ema(window, 21, 3),  // 21-day EMA
            
            // Oscillators
            window -> rsi(window, 14, 3),  // 14-day RSI
            window -> macd(window, 12, 26, 3), // MACD
            window -> stochasticOscillator(window, 14, 3),
            
            // Volatility
            window -> atr(window, 14),     // 14-day ATR
            window -> bollingerBand(window, 20, 2, 3),
            
            // Volume
            window -> obv(window, 4),      // OBV
            
            // Constants
            window -> 1.0,
            window -> 0.0,
            window -> -1.0,
            window -> 0.5,
            window -> 2.0,
            window -> (double)LOOKBACK_WINDOW
        );
    }

    // Enhanced Function Set
    private List<Function<Function<double[][], Double>[], Function<double[][], Double>>> getFunctions() {
        return Arrays.asList(
            // Arithmetic
            children -> window -> children[0].apply(window) + children[1].apply(window),
            children -> window -> children[0].apply(window) - children[1].apply(window),
            children -> window -> children[0].apply(window) * children[1].apply(window),
            children -> window -> safeDivide(children[0].apply(window), children[1].apply(window)),
            
            // Comparisons
            children -> window -> children[0].apply(window) > children[1].apply(window) ? 1.0 : 0.0,
            children -> window -> children[0].apply(window) < children[1].apply(window) ? 1.0 : 0.0,
            children -> window -> children[0].apply(window) == children[1].apply(window) ? 1.0 : 0.0,
            
            // Math functions
            children -> window -> Math.max(children[0].apply(window), children[1].apply(window)),
            children -> window -> Math.min(children[0].apply(window), children[1].apply(window)),
            children -> window -> Math.abs(children[0].apply(window)),
            children -> window -> Math.log(Math.abs(children[0].apply(window)) + 1e-10),
            children -> window -> Math.sin(children[0].apply(window)),
            children -> window -> Math.sqrt(Math.abs(children[0].apply(window))),
            
            // Conditional
            children -> window -> children[0].apply(window) > 0 ? children[1].apply(window) : children[2].apply(window)
        );
    }

    // Technical Indicator Implementations
    private double sma(double[][] window, int period, int priceIndex) {
        double sum = 0;
        int start = Math.max(0, window.length - period);
        for (int i = start; i < window.length; i++) {
            sum += window[i][priceIndex];
        }
        return sum / (window.length - start);
    }

    private double ema(double[][] window, int period, int priceIndex) {
        double multiplier = 2.0 / (period + 1);
        double ema = sma(window, period, priceIndex);
        for (int i = Math.max(0, window.length - period); i < window.length; i++) {
            ema = (window[i][priceIndex] - ema) * multiplier + ema;
        }
        return ema;
    }

    private double rsi(double[][] window, int period, int priceIndex) {
        if (window.length < period + 1) return 50.0;
        
        double avgGain = 0;
        double avgLoss = 0;
        
        for (int i = window.length - period; i < window.length; i++) {
            double change = window[i][priceIndex] - window[i-1][priceIndex];
            if (change > 0) {
                avgGain += change;
            } else {
                avgLoss -= change;
            }
        }
        
        avgGain /= period;
        avgLoss /= period;
        
        return avgLoss == 0 ? 100.0 : 100.0 - (100.0 / (1.0 + avgGain / avgLoss));
    }

    private double macd(double[][] window, int fast, int slow, int priceIndex) {
        return ema(window, fast, priceIndex) - ema(window, slow, priceIndex);
    }

    private double stochasticOscillator(double[][] window, int period, int priceIndex) {
        if (window.length < period) return 50.0;
        
        double highestHigh = Double.MIN_VALUE;
        double lowestLow = Double.MAX_VALUE;
        double close = window[window.length-1][priceIndex];
        
        for (int i = window.length - period; i < window.length; i++) {
            highestHigh = Math.max(highestHigh, window[i][1]); // High
            lowestLow = Math.min(lowestLow, window[i][2]);     // Low
        }
        
        return (close - lowestLow) / (highestHigh - lowestLow) * 100;
    }

    private double atr(double[][] window, int period) {
        if (window.length < period + 1) return 0.0;
        
        double sum = 0;
        for (int i = window.length - period; i < window.length; i++) {
            double tr = Math.max(
                window[i][1] - window[i][2], // High - Low
                Math.max(
                    Math.abs(window[i][1] - window[i-1][3]), // High - Prev Close
                    Math.abs(window[i][2] - window[i-1][3])  // Low - Prev Close
                )
            );
            sum += tr;
        }
        return sum / period;
    }

    private double bollingerBand(double[][] window, int period, double multiplier, int priceIndex) {
        double sma = sma(window, period, priceIndex);
        double sum = 0;
        int start = Math.max(0, window.length - period);
        
        for (int i = start; i < window.length; i++) {
            sum += Math.pow(window[i][priceIndex] - sma, 2);
        }
        
        double stdDev = Math.sqrt(sum / (window.length - start));
        return (window[window.length-1][priceIndex] - sma) / (multiplier * stdDev);
    }

    private double obv(double[][] window, int priceIndex) {
        double obv = 0;
        for (int i = 1; i < window.length; i++) {
            if (window[i][priceIndex] > window[i-1][priceIndex]) {
                obv += window[i][4]; // Volume
            } else if (window[i][priceIndex] < window[i-1][priceIndex]) {
                obv -= window[i][4]; // Volume
            }
        }
        return obv;
    }

    private double safeDivide(double a, double b) {
        return Math.abs(b) < 1e-10 ? 1.0 : a / b;
    }

    // Enhanced Training Process
    public void train() {
        initializePopulation();
        
        double bestValidationFitness = Double.MIN_VALUE;
        Function<double[][], Double> bestProgram = null;
        int noImprovementCount = 0;
        
        for (int generation = 0; generation < MAX_GENERATIONS; generation++) {
            List<Function<double[][], Double>> newPopulation = new ArrayList<>();
            
            // Elitism - keep best performer
            Function<double[][], Double> bestInGen = Collections.max(population, 
                    Comparator.comparingDouble(this::evaluateFitness));
            newPopulation.add(bestInGen);
            
            // Evaluate on validation set
            double validationFitness = evaluateValidationFitness(bestInGen);
            if (validationFitness > bestValidationFitness) {
                bestValidationFitness = validationFitness;
                bestProgram = bestInGen;
                noImprovementCount = 0;
            } else {
                noImprovementCount++;
            }
            
            // Early stopping
            if (noImprovementCount >= 15) {
                System.out.println("Early stopping at generation " + generation);
                break;
            }
            
            // Create new generation
            while (newPopulation.size() < POPULATION_SIZE) {
                Function<double[][], Double> parent1 = tournamentSelection();
                Function<double[][], Double> parent2 = tournamentSelection();
                
                Function<double[][], Double> offspring;
                if (random.nextDouble() < CROSSOVER_RATE) {
                    offspring = enhancedCrossover(parent1, parent2);
                } else {
                    offspring = random.nextDouble() < 0.5 ? parent1 : parent2;
                }
                
                offspring = enhancedMutate(offspring);
                newPopulation.add(offspring);
            }
            
            population = newPopulation;
            
            // Progress reporting
            if (generation % 5 == 0) {
                System.out.printf("Gen %d: Train=%.2f%%, Val=%.2f%%\n", 
                        generation,
                        evaluateTrainFitness(bestInGen) * 100,
                        validationFitness * 100);
            }
        }
        
        // Ensure best program is preserved
        if (bestProgram != null) {
            population.set(0, bestProgram);
        }
    }

    // Enhanced Fitness Evaluation
    private double evaluateFitness(Function<double[][], Double> program, 
                                 List<double[]> data, 
                                 int lookback) {
        double profit = 0;
        int correct = 0;
        int trades = 0;
        
        for (int i = lookback; i < data.size(); i++) {
            double[][] window = new double[lookback][];
            for (int j = 0; j < lookback; j++) {
                window[j] = data.get(i - lookback + j);
            }
            
            double output = program.apply(window);
            int prediction = output > 0.5 ? 1 : 0;
            int actual = (int) data.get(i)[data.get(i).length - 1];
            
            if (prediction == actual) {
                correct++;
                profit += Math.abs(data.get(i)[3] - data.get(i-1)[3]); // Close price change
            }
            trades++;
        }
        
        double accuracy = trades > 0 ? (double) correct / trades : 0.0;
        double avgProfit = trades > 0 ? profit / trades : 0.0;
        
        // Combined fitness metric (weighted accuracy and profitability)
        return (0.7 * accuracy) + (0.3 * avgProfit);
    }

    private double evaluateTrainFitness(Function<double[][], Double> program) {
        return evaluateFitness(program, normalizedTrainData, LOOKBACK_WINDOW);
    }

    private double evaluateValidationFitness(Function<double[][], Double> program) {
        return evaluateFitness(program, normalizedValidationData, LOOKBACK_WINDOW);
    }

    public double evaluateTestFitness(Function<double[][], Double> program) {
        return evaluateFitness(program, normalizedTestData, LOOKBACK_WINDOW);
    }

    // Enhanced Genetic Operators
    private Function<double[][], Double> tournamentSelection() {
        Function<double[][], Double> best = null;
        double bestFitness = -1;
        for (int i = 0; i < TOURNAMENT_SIZE; i++) {
            Function<double[][], Double> candidate = population.get(random.nextInt(population.size()));
            double fitness = evaluateTrainFitness(candidate);
            if (fitness > bestFitness) {
                best = candidate;
                bestFitness = fitness;
            }
        }
        return best;
    }

    private Function<double[][], Double> enhancedCrossover(Function<double[][], Double> parent1, 
                                                        Function<double[][], Double> parent2) {
        // In a complete implementation, this would swap subtrees
        // For simplicity, we alternate between parents
        return random.nextDouble() < 0.5 ? parent1 : parent2;
    }

    private Function<double[][], Double> enhancedMutate(Function<double[][], Double> individual) {
        if (random.nextDouble() < MUTATION_RATE) {
            // 50% chance grow new subtree, 50% chance point mutation
            if (random.nextDouble() > 0.5) {
                return growTree(random.nextInt(MAX_DEPTH/2), getTerminals(), getFunctions());
            } else {
                return pointMutate(individual);
            }
        }
        return individual;
    }

    private Function<double[][], Double> pointMutate(Function<double[][], Double> individual) {
        // Simplified mutation - in practice would mutate nodes
        List<Function<double[][], Double>> terminals = getTerminals();
        return terminals.get(random.nextInt(terminals.size()));
    }

    // Tree Initialization
    private void initializePopulation() {
        List<Function<double[][], Double>> terminals = getTerminals();
        List<Function<Function<double[][], Double>[], Function<double[][], Double>>> functions = getFunctions();
        
        for (int i = 0; i < POPULATION_SIZE; i++) {
            if (random.nextDouble() < 0.5) {
                population.add(growTree(0, terminals, functions));
            } else {
                population.add(fullTree(0, terminals, functions));
            }
        }
    }

    private Function<double[][], Double> growTree(int depth, 
            List<Function<double[][], Double>> terminals,
            List<Function<Function<double[][], Double>[], Function<double[][], Double>>> functions) {
        if (depth >= MAX_DEPTH || (depth > 0 && random.nextDouble() < 0.3)) {
            return terminals.get(random.nextInt(terminals.size()));
        } else {
            Function<Function<double[][], Double>[], Function<double[][], Double>> function = 
                functions.get(random.nextInt(functions.size()));
            Function<double[][], Double>[] children = new Function[function.apply(null) instanceof Function ? 2 : 1];
            for (int i = 0; i < children.length; i++) {
                children[i] = growTree(depth + 1, terminals, functions);
            }
            return function.apply(children);
        }
    }

    private Function<double[][], Double> fullTree(int depth, 
            List<Function<double[][], Double>> terminals,
            List<Function<Function<double[][], Double>[], Function<double[][], Double>>> functions) {
        if (depth >= MAX_DEPTH) {
            return terminals.get(random.nextInt(terminals.size()));
        } else {
            Function<Function<double[][], Double>[], Function<double[][], Double>> function = 
                functions.get(random.nextInt(functions.size()));
            Function<double[][], Double>[] children = new Function[function.apply(null) instanceof Function ? 2 : 1];
            for (int i = 0; i < children.length; i++) {
                children[i] = fullTree(depth + 1, terminals, functions);
            }
            return function.apply(children);
        }
    }

    public Function<double[][], Double> getBestProgram() {
        return Collections.max(population, Comparator.comparingDouble(this::evaluateTrainFitness));
    }

    // Data Loading
    public static List<double[]> loadCSVData(String filename) throws IOException {
        List<double[]> data = new ArrayList<>();
        BufferedReader reader = new BufferedReader(new FileReader(filename));
        String line;
        boolean headerSkipped = false;
        
        while ((line = reader.readLine()) != null) {
            if (!headerSkipped) {
                headerSkipped = true;
                continue;
            }
            
            String[] values = line.split(",");
            double[] row = new double[values.length];
            for (int i = 0; i < values.length; i++) {
                row[i] = Double.parseDouble(values[i]);
            }
            data.add(row);
        }
        reader.close();
        return data;
    }

    // Main Execution
    public static void main(String[] args) {
        try {
            // Load and prepare data
            List<double[]> allData = loadCSVData("./data/BTC_train.csv");
            Collections.shuffle(allData);
            
            // 70% train, 15% validation, 15% test
            int trainSize = (int)(allData.size() * 0.7);
            int valSize = (int)(allData.size() * 0.15);
            
            List<double[]> trainData = allData.subList(0, trainSize);
            List<double[]> valData = allData.subList(trainSize, trainSize + valSize);
            List<double[]> testData = allData.subList(trainSize + valSize, allData.size());
            
            // Try to load external test set if available
            try {
                List<double[]> externalTest = loadCSVData("./data/BTC_test.csv");
                testData = externalTest;
            } catch (IOException e) {
                System.out.println("Using holdout test set");
            }
            
            // Train model
            EnhancedStockPredictionGP predictor = new EnhancedStockPredictionGP(trainData, valData, testData);
            System.out.println("Training model...");
            predictor.train();
            
            // Evaluate
            Function<double[][], Double> best = predictor.getBestProgram();
            double testAccuracy = predictor.evaluateTestFitness(best);
            
            System.out.printf("\nFinal Test Results:\n");
            System.out.printf("Accuracy: %.2f%%\n", testAccuracy * 100);
            
        } catch (IOException e) {
            System.err.println("Error: " + e.getMessage());
            e.printStackTrace();
        }
    }
}