import java.util.*;

public class TabuSearch {
  private List<City> cities;
  private double[][] distanceMatrix;
  private long seed;

  public TabuSearch(List<City> cities, long seed) 
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

  public double calculateListLength(List<Integer> List) 
  {
    double length = 0;
    for (int i = 0; i < List.size(); i++) 
    {
      int city1 = List.get(i);
      int city2 = List.get((i + 1) % List.size());
      length += distanceMatrix[city1][city2];
    }
    return length;
  }

  private List<Integer> generateNeighbor(List<Integer> list, Set<String> tabuList) 
  {
    List<Integer> bestNeighbor = null;
    double bestNeighborCost = Double.POSITIVE_INFINITY;

    for (int i = 1; i < list.size() - 2; i++) 
    {
      for (int j = i + 1; j < list.size() - 1; j++) 
      {
        List<Integer> neighbor = new ArrayList<>(list);
        Collections.swap(neighbor, i, j);

        String moveKey = i + "-" + j;
        if (tabuList.contains(moveKey)) 
        {
          continue;
        }

        double neighborCost = calculateListLength(neighbor);
        if (neighborCost < bestNeighborCost) 
        {
          bestNeighbor = neighbor;
          bestNeighborCost = neighborCost;
        }
      }
    }

    return (bestNeighbor != null) ? bestNeighbor : list;
  }
  
  public List<Integer> run(int tabuTenure, int maxIterations) 
  {
    List<Integer> currentList = generateRandomList();
    List<Integer> bestList = new ArrayList<>(currentList);
    double bestCost = calculateListLength(bestList);

    Set<String> tabuList = new HashSet<>();
    int iteration = 0;

    while (iteration < maxIterations) 
    {
      List<Integer> newList = generateNeighbor(currentList, tabuList);
      double newCost = calculateListLength(newList);

      if (newCost < bestCost) 
      {
        bestList = new ArrayList<>(newList);
        bestCost = newCost;
      }

      currentList = new ArrayList<>(newList);

      if (tabuList.size() > tabuTenure)
      {
        tabuList.clear();
      }

      iteration++;
    }

    return bestList;
  }
}