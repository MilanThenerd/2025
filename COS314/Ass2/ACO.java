import java.io.*;
import java.util.*;
import java.nio.file.*;

public class ACO {
  private int n;
  private int m;
  private double tmax;
  private Node[] nodes;
  private Random random;

  private double alpha = 1.0;
  private double beta = 5.0;
  private int maxIterations = 100;
  private int numberOfAnts = 200;
  private double initialPheromone = 0.1;
  private double evaporationRate = 0.1;
  private double q0 = 0.9;

  private double[][] pheromones;
  private double[][] heuristic;
  private double[][] distances;

  private List<List<Integer>> bestSolution;
  private double bestScore = 0;
  private long runtimeMillis;
  private long bestSeed;

  public static void main(String[] args) throws IOException {
    Scanner scanner = new Scanner(System.in);
    File dataDir = new File("./Data_TOP");
    File[] files = dataDir.listFiles((dir, name) -> name.endsWith(".txt"));

    if (files == null || files.length == 0) {
      System.out.println("No .txt files found in Data_TOP directory!");
      return;
    }

    System.out.println("Available files:");
    for (int i = 0; i < files.length; i++) {
      System.out.printf("%d. %s\n", i + 1, files[i].getName());
    }

    System.out.println("\n=== ACO Parameter Configuration ===");
    double alpha = getParameter(scanner, "Alpha (pheromone importance)", 1.0);
    double beta = getParameter(scanner, "Beta (heuristic importance)", 5.0);
    int maxIterations = (int) getParameter(scanner, "Max iterations", 100);
    int numberOfAnts = (int) getParameter(scanner, "Number of ants", 200);
    double q0 = getParameter(scanner, "Exploitation probability (q0)", 0.9);
    double evaporationRate = getParameter(scanner, "Evaporation rate", 0.1);

    System.out.println("\nChoose an option:");
    System.out.println("1. Run all files with these parameters");
    System.out.println("2. Select specific file and customize further");
    System.out.print("Your choice: ");
    int choice = scanner.nextInt();

    if (choice == 1) {
      for (File file : files) {
        runACOForFile(file.getPath(), -1, alpha, beta, maxIterations,
            numberOfAnts, q0, evaporationRate);
      }
    } else if (choice == 2) {
      System.out.print("Enter file number (1-" + files.length + "): ");
      int fileNum = scanner.nextInt();
      if (fileNum < 1 || fileNum > files.length) {
        System.out.println("Invalid file number!");
        return;
      }

      System.out.print("Enter seed (or -1 for random): ");
      long seed = scanner.nextLong();

      System.out.println("\nCurrent parameters for " + files[fileNum - 1].getName() + ":");
      System.out.println("1. Alpha: " + alpha);
      System.out.println("2. Beta: " + beta);
      System.out.println("3. Max iterations: " + maxIterations);
      System.out.println("4. Number of ants: " + numberOfAnts);
      System.out.println("5. q0: " + q0);
      System.out.println("6. Evaporation rate: " + evaporationRate);
      System.out.println("7. Keep all parameters as-is");
      System.out.print("Enter parameter number to change (1-6) or 7 to run: ");

      int paramChoice = scanner.nextInt();
      if (paramChoice >= 1 && paramChoice <= 6) {
        String[] paramNames = { "Alpha", "Beta", "Max iterations",
            "Number of ants", "q0", "Evaporation rate" };
        System.out.print("Enter new value for " + paramNames[paramChoice - 1] + ": ");
        double newValue = scanner.nextDouble();

        switch (paramChoice) {
          case 1:
            alpha = newValue;
            break;
          case 2:
            beta = newValue;
            break;
          case 3:
            maxIterations = (int) newValue;
            break;
          case 4:
            numberOfAnts = (int) newValue;
            break;
          case 5:
            q0 = newValue;
            break;
          case 6:
            evaporationRate = newValue;
            break;
        }
      }

      runACOForFile(files[fileNum - 1].getPath(), seed, alpha, beta, maxIterations,
          numberOfAnts, q0, evaporationRate);
    }

    scanner.close();
  }

  private static double getParameter(Scanner scanner, String paramName, double defaultValue) {
    System.out.print(paramName + " [" + defaultValue + "]: ");
    try {
      return scanner.nextDouble();
    } catch (InputMismatchException e) {
      scanner.next();
      return defaultValue;
    }
  }

  private static void runACOForFile(String filePath, long seed,
      double alpha, double beta, int maxIterations,
      int numberOfAnts, double q0, double evaporationRate)
      throws IOException {
    System.out.println("\nProcessing file: " + filePath);
    System.out.println("Parameters: α=" + alpha + " β=" + beta +
        " iter=" + maxIterations + " ants=" + numberOfAnts +
        " q0=" + q0 + " evap=" + evaporationRate);

    ACO bestAco = new ACO();
    bestAco.setParameters(alpha, beta, maxIterations, numberOfAnts, q0, evaporationRate);

    int runs = (seed == -1) ? 10 : 1;

    for (int i = 0; i < runs; i++) {
      long currentSeed = (seed == -1) ? System.currentTimeMillis() : seed;
      ACO aco = new ACO(currentSeed);
      aco.setParameters(alpha, beta, maxIterations, numberOfAnts, q0, evaporationRate);
      aco.loadProblemFromFile(filePath);

      long startTime = System.currentTimeMillis();
      aco.solve();
      long endTime = System.currentTimeMillis();
      aco.runtimeMillis = endTime - startTime;

      if (aco.bestScore > bestAco.bestScore) {
        bestAco = aco;
        bestAco.bestSeed = currentSeed;
      }
    }
    System.out.println("Results for " + new File(filePath).getName());
    System.out.println("Best seed: " + bestAco.bestSeed);
    for (int k = 0; k < bestAco.bestSolution.size(); k++) {
      List<Integer> route = bestAco.bestSolution.get(k);
      System.out.printf("Vehicle %d: Score=%.2f Time=%.2f Route=%s\n",
          k + 1,
          bestAco.calculateRouteScore(route),
          bestAco.calculateRouteTime(route),
          formatRoute(route));
    }
    System.out.printf("Total score: %.2f Runtime: %dms\n\n",
        bestAco.bestScore, bestAco.runtimeMillis);
  }

  public void setParameters(double alpha, double beta, int maxIterations,
      int numberOfAnts, double q0, double evaporationRate) {
    this.alpha = alpha;
    this.beta = beta;
    this.maxIterations = maxIterations;
    this.numberOfAnts = numberOfAnts;
    this.q0 = q0;
    this.evaporationRate = evaporationRate;
  }

  private static String formatRoute(List<Integer> route) {
    if (route.isEmpty()) return "Depot -> Depot";
    StringBuilder sb = new StringBuilder("Depot");
    for (int node : route) {
      sb.append(" -> Node ").append(node);
    }
    sb.append(" -> Depot");
    return sb.toString();
  }

  public ACO(long seed) {
    this.random = new Random(seed);
  }

  public ACO() {
    this(System.currentTimeMillis());
  }

  public void loadProblemFromFile(String filename) throws IOException {
    try (BufferedReader reader = new BufferedReader(new FileReader(filename))) {
      String line = reader.readLine();
      while (line != null && line.trim().isEmpty())
        line = reader.readLine();

      n = Integer.parseInt(line.trim().split("\\s+")[1]);
      m = Integer.parseInt(reader.readLine().trim().split("\\s+")[1]);
      tmax = Double.parseDouble(reader.readLine().trim().split("\\s+")[1]);

      nodes = new Node[n];
      for (int i = 0; i < n; i++) {
        line = reader.readLine();
        while (line != null && line.trim().isEmpty())
          line = reader.readLine();
        if (line == null)
          break;

        String[] parts = line.trim().split("\\s+");
        double x = Double.parseDouble(parts[0]);
        double y = Double.parseDouble(parts[1]);
        double score = Double.parseDouble(parts[2]);
        nodes[i] = new Node(i, x, y, score);
      }

      nodes[0].score = 0;

      distances = new double[n][n];
      heuristic = new double[n][n];
      pheromones = new double[n][n];

      for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
          distances[i][j] = distance(i, j);
          if (i != j) {
            heuristic[i][j] = nodes[j].score / (distances[i][j] + 0.001);
          }
          pheromones[i][j] = initialPheromone;
        }
      }
    }
  }

  private double distance(int i, int j) {
    if (i == j)
      return 0;
    if (distances[i][j] == 0) {
      Node a = nodes[i];
      Node b = nodes[j];
      distances[i][j] = Math.hypot(a.x - b.x, a.y - b.y);
    }
    return distances[i][j];
  }

  private double calculateRouteScore(List<Integer> route) {
    return route.stream().mapToDouble(node -> nodes[node].score).sum();
  }

  private double calculateTotalScore(List<List<Integer>> routes) {
    double totalScore = 0.0;
    for (List<Integer> route : routes) {
      totalScore += calculateRouteScore(route);
    }
    return totalScore;
  }

  private double calculateRouteTime(List<Integer> route) {
    if (route.isEmpty())
      return 0;
    double time = distances[0][route.get(0)];
    for (int i = 1; i < route.size(); i++) {
      time += distances[route.get(i - 1)][route.get(i)];
    }
    time += distances[route.get(route.size() - 1)][0];
    return time;
  }

  public void solve() {
    initializeWithGreedySolution();

    for (int iter = 0; iter < maxIterations; iter++) {
      List<List<Integer>> iterationBestSolution = null;
      double iterationBestScore = 0;

      for (int ant = 0; ant < numberOfAnts; ant++) {
        List<List<Integer>> routes = new ArrayList<>();
        List<Double> times = new ArrayList<>();
        boolean[] visited = new boolean[n];
        visited[0] = true;

        for (int k = 0; k < m; k++) {
          routes.add(new ArrayList<>());
          times.add(0.0);
        }

        while (true) {
          int bestVehicle = -1;
          int bestNext = -1;
          double bestValue = -1;
          List<Candidate> candidates = new ArrayList<>();

          for (int k = 0; k < m; k++) {
            int last = routes.get(k).isEmpty() ? 0 : routes.get(k).get(routes.get(k).size() - 1);

            for (int j = 1; j < n; j++) {
              if (!visited[j]) {
                double newTime = times.get(k);
                if (routes.get(k).isEmpty()) {
                  newTime += distances[0][j] + distances[j][0];
                } else {
                  newTime += distances[last][j] + distances[j][0] - distances[last][0];
                }

                if (newTime <= tmax) {
                  double value = Math.pow(pheromones[last][j], alpha) * Math.pow(heuristic[last][j], beta);
                  candidates.add(new Candidate(k, j, value));
                  if (value > bestValue) {
                    bestValue = value;
                    bestVehicle = k;
                    bestNext = j;
                  }
                }
              }
            }
          }

          if (candidates.isEmpty())
            break;

          int selectedVehicle, selectedNext;
          if (random.nextDouble() < q0) {
            selectedVehicle = bestVehicle;
            selectedNext = bestNext;
          } else {
            selectedVehicle = -1;
            selectedNext = -1;
            double total = candidates.stream().mapToDouble(c -> c.value).sum();
            double r = random.nextDouble() * total;
            double cumulative = 0.0;

            for (Candidate c : candidates) {
              cumulative += c.value;
              if (cumulative >= r) {
                selectedVehicle = c.vehicle;
                selectedNext = c.node;
                break;
              }
            }

            if (selectedVehicle == -1) {
              selectedVehicle = bestVehicle;
              selectedNext = bestNext;
            }
          }

          int last = routes.get(selectedVehicle).isEmpty() ? 0
              : routes.get(selectedVehicle).get(routes.get(selectedVehicle).size() - 1);
          double addedTime = distances[last][selectedNext];
          if (routes.get(selectedVehicle).isEmpty()) {
            addedTime += distances[selectedNext][0];
          } else {
            addedTime += distances[selectedNext][0] - distances[last][0];
          }
          times.set(selectedVehicle, times.get(selectedVehicle) + addedTime);

          routes.get(selectedVehicle).add(selectedNext);
          visited[selectedNext] = true;

          pheromones[last][selectedNext] = (1 - evaporationRate) * pheromones[last][selectedNext]
              + evaporationRate * initialPheromone;
        }

        for (int k = 0; k < m; k++) {
          List<Integer> optimizedRoute = constrainedTwoOpt(routes.get(k), tmax);
          routes.set(k, optimizedRoute);
        }

        double totalScore = calculateTotalScore(routes);
        if (totalScore > iterationBestScore && isSolutionValid(routes)) {
          iterationBestScore = totalScore;
          iterationBestSolution = deepCopySolution(routes);
        }
      }
      if (iterationBestSolution != null) {
        if (iterationBestScore > bestScore) {
          bestScore = iterationBestScore;
          bestSolution = deepCopySolution(iterationBestSolution);
          for (List<Integer> route : bestSolution) {
            int last = 0;
            double routeTime = calculateRouteTime(route);
            for (int node : route) {
              pheromones[last][node] = (1 - evaporationRate) * pheromones[last][node]
                  + evaporationRate * (1.0 / (1.0 + tmax - routeTime));
              last = node;
            }
            pheromones[last][0] = (1 - evaporationRate) * pheromones[last][0]
                + evaporationRate * (1.0 / (1.0 + tmax - routeTime));
          }
        }
      }
    }
  }

  private List<Integer> constrainedTwoOpt(List<Integer> route, double maxTime) {
    if (route.size() < 2)
      return route;

    List<Integer> bestRoute = new ArrayList<>(route);
    double bestTime = calculateRouteTime(route);
    boolean improved = true;

    while (improved) {
      improved = false;
      for (int i = 0; i < route.size() - 1; i++) {
        for (int j = i + 1; j < route.size(); j++) {
          List<Integer> newRoute = new ArrayList<>(route);
          Collections.reverse(newRoute.subList(i, j + 1));
          double newTime = calculateRouteTime(newRoute);

          if (newTime < bestTime && newTime <= maxTime) {
            bestRoute = newRoute;
            bestTime = newTime;
            improved = true;
          }
        }
      }
    }
    return bestRoute;
  }

  private boolean isSolutionValid(List<List<Integer>> solution) {
    for (List<Integer> route : solution) {
      if (calculateRouteTime(route) > tmax) {
        return false;
      }
    }
    return true;
  }

  private void initializeWithGreedySolution() {
    List<List<Integer>> greedySolution = new ArrayList<>();
    boolean[] visited = new boolean[n];
    visited[0] = true;

    for (int k = 0; k < m; k++) {
      greedySolution.add(new ArrayList<>());
    }

    List<Node> sortedNodes = new ArrayList<>();
    for (int i = 1; i < n; i++) {
      sortedNodes.add(nodes[i]);
    }
    sortedNodes.sort((a, b) -> Double.compare(b.score, a.score));

    for (Node node : sortedNodes) {
      for (int k = 0; k < m; k++) {
        List<Integer> route = greedySolution.get(k);
        int last = route.isEmpty() ? 0 : route.get(route.size() - 1);
        double newTime = calculateRouteTime(route) + distances[last][node.id] +
            (route.isEmpty() ? distances[node.id][0] : distances[node.id][0] - distances[last][0]);

        if (newTime <= tmax && !visited[node.id]) {
          route.add(node.id);
          visited[node.id] = true;
          break;
        }
      }
    }

    for (List<Integer> route : greedySolution) {
      int last = 0;
      for (int node : route) {
        pheromones[last][node] += nodes[node].score * 0.5;
        last = node;
      }
      pheromones[last][0] += nodes[0].score * 0.5;
    }
  }

  private List<List<Integer>> deepCopySolution(List<List<Integer>> solution) {
    List<List<Integer>> copy = new ArrayList<>();
    for (List<Integer> route : solution) {
      copy.add(new ArrayList<>(route));
    }
    return copy;
  }

  static class Node {
    int id;
    double x, y;
    double score;

    Node(int id, double x, double y, double score) {
      this.id = id;
      this.x = x;
      this.y = y;
      this.score = score;
    }
  }

  static class Candidate {
    int vehicle;
    int node;
    double value;

    Candidate(int vehicle, int node, double value) {
      this.vehicle = vehicle;
      this.node = node;
      this.value = value;
    }
  }
}