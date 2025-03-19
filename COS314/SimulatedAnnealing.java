import java.util.*;

public class SimulatedAnnealing {
    private TSP tsp;
    private double temperature;
    private double coolingRate;
    private int maxIterations;
    private Random rand;  // Random object to be seeded

    public SimulatedAnnealing(TSP tsp, double temperature, double coolingRate, int maxIterations, long seed) {
        this.tsp = tsp;
        this.temperature = temperature;
        this.coolingRate = coolingRate;
        this.maxIterations = maxIterations;
        this.rand = new Random(seed);  // Use the seed here
    }

    public List<Integer> run() {
        List<Integer> currentSolution = tsp.generateRandomTour();
        List<Integer> bestSolution = new ArrayList<>(currentSolution);
        double bestCost = tsp.calculateTourLength(bestSolution);

        for (int i = 0; i < maxIterations; i++) {
            List<Integer> newSolution = generateNeighbor(currentSolution);
            double newCost = tsp.calculateTourLength(newSolution);

            if (acceptanceCriterion(bestCost, newCost)) {
                currentSolution = new ArrayList<>(newSolution);
                if (newCost < bestCost) {
                    bestSolution = new ArrayList<>(currentSolution);
                    bestCost = newCost;
                }
            }

            temperature *= coolingRate;  // Cooling down the temperature
        }

        return bestSolution;
    }

    private List<Integer> generateNeighbor(List<Integer> currentSolution) {
        // Generate a neighbor solution by swapping two cities (or other perturbation methods)
        List<Integer> neighbor = new ArrayList<>(currentSolution);
        int i = rand.nextInt(neighbor.size());
        int j = rand.nextInt(neighbor.size());
        Collections.swap(neighbor, i, j);
        return neighbor;
    }

    private boolean acceptanceCriterion(double currentCost, double newCost) {
        // Accept worse solutions with a probability based on the temperature
        if (newCost < currentCost) {
            return true;
        }
        double probability = Math.exp((currentCost - newCost) / temperature);
        return rand.nextDouble() < probability;
    }
}
