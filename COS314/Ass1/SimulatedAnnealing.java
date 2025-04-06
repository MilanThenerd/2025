import java.util.*;

public class SimulatedAnnealing 
{
    private List<City> cities;
    private double[][] distanceMatrix;
    long seed;

    public SimulatedAnnealing(List<City> cities, long seed) 
    {
        this.cities = cities;
        this.seed = seed;
        initializeDistanceMatrix();
    }

    private void initializeDistanceMatrix() 
    {
        int n = cities.size();
        distanceMatrix = new double[n][n];
        for (int i = 0; i < n; i++) 
        {
            for (int j = 0; j < n; j++) 
            {
                distanceMatrix[i][j] = calculateDistance(cities.get(i), cities.get(j));
            }
        }
    }

    private double calculateDistance(City city1, City city2) 
    {
        double dx = city1.x - city2.x;
        double dy = city1.y - city2.y;
        return Math.sqrt(dx * dx + dy * dy);
    }

    private List<Integer> generateRandomList() 
    {
        List<Integer> list = new ArrayList<>();
        list.add(0);

        List<Integer> remaining = new ArrayList<>();
        for (int i = 1; i < cities.size(); i++) 
        {
            remaining.add(i);
        }
        Random rand = new Random(seed);
        Collections.shuffle(remaining, rand);
        list.addAll(remaining);
        list.add(0);

        return list;
    }

    public double calculateListLength(List<Integer> list)
    {
        double length = 0;
        for (int i = 0; i < list.size(); i++) 
        {
            int city1 = list.get(i);
            int city2 = list.get((i + 1) % list.size());
            length += distanceMatrix[city1][city2];
        }
        return length;
    }

    private List<Integer> generateNeighbor(List<Integer> list) 
    {
        List<Integer> neighbor = new ArrayList<>(list);
        Random rand = new Random(seed);
        int i = rand.nextInt(neighbor.size() - 2) + 1;
        int j = rand.nextInt(neighbor.size() - 2) + 1;
        Collections.swap(neighbor, i, j);
        return neighbor;
    }

    public List<Integer> run(double initialTemperature, double coolingRate, int maxIterations) 
    {
        List<Integer> currentList = generateRandomList();
        List<Integer> bestList = new ArrayList<>(currentList);
        double bestCost = calculateListLength(bestList);
        double temperature = initialTemperature;

        for (int iteration = 0; iteration < maxIterations; iteration++) 
        {
            List<Integer> newList = generateNeighbor(currentList);
            double newCost = calculateListLength(newList);

            if (acceptanceCriterion(bestCost, newCost, temperature)) 
            {
                currentList = new ArrayList<>(newList);
                if (newCost < bestCost) 
                {
                    bestList = new ArrayList<>(newList);
                    bestCost = newCost;
                }
            }
            temperature *= coolingRate;
        }

        return bestList;
    }

    private boolean acceptanceCriterion(double currentCost, double newCost, double temperature) 
    {
        if (newCost < currentCost) 
        {
            return true;
        }
        double probability = Math.min(1, Math.exp((currentCost - newCost) / temperature));
        Random rand = new Random(seed);
        return rand.nextDouble() < probability;
    }
}