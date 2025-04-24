import java.io.File;
import java.io.IOException;

public class Main {
    public static void main(String[] args) {
        String folderPath = "Data_TOP";
        File folder = new File(folderPath);
        
        if (!folder.exists() || !folder.isDirectory()) {
            System.err.println("Invalid folder path");
            return;
        }

        File[] files = folder.listFiles((dir, name) -> name.endsWith(".txt"));
        if (files == null || files.length == 0) {
            System.err.println("No input files found");
            return;
        }

        for (File file : files) {
            System.out.println("\nProcessing: " + file.getName());
            try {
                TOPInstance instance = new TOPInstance();
                instance.readInput(file);
                
                ACOSolver solver = new ACOSolver(instance);
                Solution bestSolution = solver.solve();
                
                printSolution(bestSolution, file.getName());
            } catch (IOException e) {
                System.err.println("Error processing " + file.getName() + ": " + e.getMessage());
            }
        }
    }

    private static void printSolution(Solution solution, String fileName) {
        System.out.println("\nBest solution for " + fileName);
        System.out.println("Total score: " + solution.totalScore);
        
        for (int i = 0; i < solution.routes.size(); i++) {
            System.out.printf("Vehicle %d (time: %.2f): %s%n", 
                i + 1, solution.routesTime.get(i), solution.routes.get(i));
        }
        
        System.out.println("Visited nodes: " + solution.visitedNodes.size() + "/" + (solution.routes.get(0).size() - 2));
    }
}