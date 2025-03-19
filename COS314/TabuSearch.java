import java.util.*;

public class TabuSearch {
    private TSP tsp;
    private int maxIterations;
    private int tabuTenure;
    private Random rand;  // Random object to be seeded

    public TabuSearch(TSP tsp, int maxIterations, int tabuTenure, long seed) {
        this.tsp = tsp;
        this.maxIterations = maxIterations;
        this.tabuTenure = tabuTenure;
        this.rand = new Random(seed);  // Use the seed here
    }

    public List<Integer> run() {
        List<Integer> currentSolution = tsp.generateRandomTour();
        List<Integer> bestSolution = new ArrayList<>(currentSolution);
        double bestCost = tsp.calculateTourLength(bestSolution);

        // Initialize the Tabu list
        Set<Integer> tabuList = new HashSet<>();
        int iteration = 0;

        while (iteration < maxIterations) {
            List<Integer> newSolution = generateNeighbor(currentSolution, tabuList);
            double newCost = tsp.calculateTourLength(newSolution);
            if (newCost < bestCost) {
                bestSolution = new ArrayList<>(newSolution);
                bestCost = newCost;
            }

            // Update the Tabu list and the current solution
            tabuList.addAll(newSolution);
            if (tabuList.size() > tabuTenure) {
                tabuList.clear();  // Reset the tabu list if it grows too large
            }

            currentSolution = new ArrayList<>(newSolution);
            iteration++;
        }

        return bestSolution;
    }

    private List<Integer> generateNeighbor(List<Integer> currentSolution, Set<Integer> tabuList) {
        List<Integer> neighbor = new ArrayList<>(currentSolution);
        int maxAttempts = 100;  // Maximum number of attempts to find valid i and j
        int attempts = 0;

        int i = rand.nextInt(neighbor.size());
        int j = rand.nextInt(neighbor.size());

        while ((tabuList.contains(i) || tabuList.contains(j)) && attempts < maxAttempts) {
            i = rand.nextInt(neighbor.size());
            j = rand.nextInt(neighbor.size());
            attempts++;
        }

        if (attempts >= maxAttempts) 
        {
            return currentSolution;
        }

        Collections.swap(neighbor, i, j);
        return neighbor;
    }
}