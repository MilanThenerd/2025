import java.util.*;
import java.io.*;

public class Main {
    public static void main(String[] args) {
        // Get seed value from user input or system time
        long seed = System.currentTimeMillis();  // Default seed using current time in milliseconds
        System.out.println("Seed value: " + seed);

        // Files containing TSP instances
        String[] filenames = {"./Data/8.txt", "./Data/12.txt", "./Data/15.txt", "./Data/20.txt", "./Data/25.txt"};

        // Loop through each TSP file to run both algorithms
        for (String filename : filenames) {
            try {
                // Read the TSP instance from file
                TSP tsp = TSP.fromFile(filename);
                System.out.println("Running for file: " + filename);

                // Initialize Simulated Annealing with the seed
                SimulatedAnnealing sa = new SimulatedAnnealing(tsp, 1000, 0.995, 10000, seed);
                List<Integer> bestSASolution = sa.run();
                double bestSACost = tsp.calculateTourLength(bestSASolution);
                
                // Initialize Tabu Search with the seed
                TabuSearch ts = new TabuSearch(tsp, 10000, 10, seed);
                List<Integer> bestTSSolution = ts.run();
                double bestTSCost = tsp.calculateTourLength(bestTSSolution);
                
                // Output the results for the current TSP instance
                System.out.println("Simulated Annealing Best Tour: " + bestSASolution);
                System.out.println("Simulated Annealing Best Cost: " + bestSACost);
                System.out.println("Tabu Search Best Tour: " + bestTSSolution);
                System.out.println("Tabu Search Best Cost: " + bestTSCost);
                System.out.println();

            } catch (IOException e) {
                System.err.println("Error reading file " + filename + ": " + e.getMessage());
            }
        }
    }
}
