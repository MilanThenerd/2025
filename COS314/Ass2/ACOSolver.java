import java.util.*;
import java.util.stream.IntStream;
import java.util.stream.Collectors;

public class ACOSolver {
    private static final double ALPHA = 1.5;
    private static final double BETA = 3.0;
    private static final double RHO = 0.2;
    private static final double Q = 100;
    private static final double TAU0 = 0.1;
    private static final int MAX_ITERATIONS = 1000;
    private static final int ANTS_COUNT = 100;

    private final TOPInstance instance;
    private double[][] pheromones;
    private double[][] heuristics;
    private Random random = new Random();

    public ACOSolver(TOPInstance instance) {
        this.instance = instance;
    }

    public Solution solve() {
        initializePheromones();
        initializeHeuristics();

        Solution bestSolution = null;
        double bestScore = -1;

        for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
            List<Solution> antSolutions = new ArrayList<>();

            for (int ant = 0; ant < ANTS_COUNT; ant++) {
                Solution solution = constructSolution();
                applyLocalSearch(solution);
                antSolutions.add(solution);

                if (solution.totalScore > bestScore) {
                    bestScore = solution.totalScore;
                    bestSolution = solution.copy();
                }
            }

            updatePheromones(antSolutions, bestSolution);
        }

        return bestSolution;
    }

    private void initializePheromones() {
        pheromones = new double[instance.n][instance.n];
        for (int i = 0; i < instance.n; i++) {
            for (int j = 0; j < instance.n; j++) {
                pheromones[i][j] = (i != j) ? TAU0 : 0;
            }
        }
    }

    private void initializeHeuristics() {
        heuristics = new double[instance.n][instance.n];
        for (int i = 0; i < instance.n; i++) {
            for (int j = 0; j < instance.n; j++) {
                if (i != j) {
                    heuristics[i][j] = instance.nodes[j].score / instance.distances[i][j];
                }
            }
        }
    }

    private Solution constructSolution() {
        Solution solution = new Solution();
        List<Integer> unvisited = IntStream.range(1, instance.n).boxed().collect(Collectors.toList());
        
        for (int vehicle = 0; vehicle < instance.m; vehicle++) {
            List<Integer> route = new ArrayList<>();
            route.add(0); // Start at depot
            double currentTime = 0;
            int currentNode = 0;
            
            while (true) {
                int nextNode = selectNextNode(currentNode, unvisited, currentTime);
                if (nextNode == -1) break;
                
                double addedTime = instance.distances[currentNode][nextNode];
                double returnTime = instance.distances[nextNode][0];
                
                if (currentTime + addedTime + returnTime > instance.tmax) {
                    break;
                }
                
                route.add(nextNode);
                currentTime += addedTime;
                currentNode = nextNode;
                unvisited.remove((Integer)nextNode);
                solution.visitedNodes.add(nextNode);
                solution.totalScore += instance.nodes[nextNode].score;
            }
            
            // Return to depot
            route.add(0);
            currentTime += instance.distances[currentNode][0];
            solution.routes.add(route);
            solution.routesTime.add(currentTime);
        }
        
        // Try to insert remaining nodes
        handleUnvisitedNodes(solution, unvisited);
        
        return solution;
    }

    private int selectNextNode(int currentNode, List<Integer> candidates, double currentTime) {
        if (candidates.isEmpty()) return -1;
        
        double[] probabilities = new double[candidates.size()];
        double sum = 0;
        
        // Calculate probabilities
        for (int i = 0; i < candidates.size(); i++) {
            int j = candidates.get(i);
            probabilities[i] = Math.pow(pheromones[currentNode][j], ALPHA) * 
                             Math.pow(heuristics[currentNode][j], BETA);
            sum += probabilities[i];
        }
        
        // Roulette wheel selection
        double rand = random.nextDouble() * sum;
        double cumulative = 0;
        for (int i = 0; i < probabilities.length; i++) {
            cumulative += probabilities[i];
            if (rand <= cumulative) {
                return candidates.get(i);
            }
        }
        
        return candidates.get(candidates.size() - 1);
    }

    private void handleUnvisitedNodes(Solution solution, List<Integer> unvisited) {
        unvisited.sort(Comparator.comparingDouble(node -> -instance.nodes[node].score));
        
        for (int node : unvisited) {
            boolean assigned = false;
            double bestIncrease = Double.MAX_VALUE;
            int bestRoute = -1;
            int bestPosition = -1;
            
            for (int i = 0; i < solution.routes.size(); i++) {
                List<Integer> route = solution.routes.get(i);
                double currentRouteTime = solution.routesTime.get(i);
                
                for (int j = 1; j < route.size(); j++) {
                    int prev = route.get(j - 1);
                    int next = route.get(j);
                    
                    double increase = instance.distances[prev][node] + 
                                    instance.distances[node][next] - 
                                    instance.distances[prev][next];
                    double newTime = currentRouteTime + increase;
                    
                    if (newTime <= instance.tmax && increase < bestIncrease) {
                        bestIncrease = increase;
                        bestRoute = i;
                        bestPosition = j;
                        assigned = true;
                    }
                }
            }
            
            if (assigned) {
                List<Integer> route = solution.routes.get(bestRoute);
                route.add(bestPosition, node);
                solution.routesTime.set(bestRoute, solution.routesTime.get(bestRoute) + bestIncrease);
                solution.totalScore += instance.nodes[node].score;
                solution.visitedNodes.add(node);
            }
        }
    }

    private void applyLocalSearch(Solution solution) {
        for (int i = 0; i < solution.routes.size(); i++) {
            List<Integer> route = solution.routes.get(i);
            if (route.size() <= 3) continue;
            
            boolean improved;
            do {
                improved = false;
                for (int j = 1; j < route.size() - 2; j++) {
                    for (int k = j + 1; k < route.size() - 1; k++) {
                        double current = instance.distances[route.get(j-1)][route.get(j)] + 
                                       instance.distances[route.get(k)][route.get(k+1)];
                        double newTime = instance.distances[route.get(j-1)][route.get(k)] + 
                                       instance.distances[route.get(j)][route.get(k+1)];
                        
                        if (newTime < current) {
                            Collections.reverse(route.subList(j, k+1));
                            solution.routesTime.set(i, solution.routesTime.get(i) - current + newTime);
                            improved = true;
                        }
                    }
                }
            } while (improved);
        }
    }

    private void updatePheromones(List<Solution> antSolutions, Solution bestSolution) {
        // Evaporation
        for (int i = 0; i < instance.n; i++) {
            for (int j = 0; j < instance.n; j++) {
                pheromones[i][j] *= (1 - RHO);
            }
        }
        
        // Deposit pheromones (elitism - only best solution)
        double deposit = Q * bestSolution.totalScore;
        for (List<Integer> route : bestSolution.routes) {
            for (int i = 0; i < route.size() - 1; i++) {
                int from = route.get(i);
                int to = route.get(i + 1);
                pheromones[from][to] += deposit;
                pheromones[to][from] += deposit;
            }
        }
    }
}