import java.io.File;
import java.io.IOException;
import java.util.List;
import java.util.Scanner;

public class Main 
{
    public static void main(String[] args) 
    {
        Scanner scanner = new Scanner(System.in);

        String folderPath = "./Data";
        File folder = new File(folderPath);

        File[] files = folder.listFiles((dir, name) -> name.endsWith(".txt"));

        if (files == null || files.length == 0) 
        {
            System.out.println("No .txt files found in the folder: " + folderPath);
            return;
        }

        System.out.println("Available .txt files in the folder:");
        for (int i = 0; i < files.length; i++) 
        {
            System.out.println((i + 1) + ": " + files[i].getName());
        }

        System.out.print("Enter the number of the file you want to process: ");
        int fileChoice = scanner.nextInt();
        if (fileChoice < 1 || fileChoice > files.length) 
        {
            System.out.println("Invalid choice. Exiting.");
            return;
        }

        File selectedFile = files[fileChoice - 1];

        System.out.println("Choose testing mode:");
        System.out.println("1: Run 10 random tests and find the best solution for each algorithm.");
        System.out.println("2: Input a seed for a single test of Simulated Annealing and Tabu Search.");
        System.out.print("Enter your choice (1 or 2): ");
        int modeChoice = scanner.nextInt();

        if (modeChoice == 1) 
        {
            runRandomTests(selectedFile);
        } 
        else if (modeChoice == 2) 
        {
            runSingleTest(selectedFile, scanner);
        } 
        else 
        {
            System.out.println("Invalid choice. Exiting.");
        }

        scanner.close();
    }

    private static void runRandomTests(File selectedFile) 
    {
        try 
        {
            TSP tsp = new TSP(selectedFile.getAbsolutePath());
            System.out.println("Dimension: " + tsp.getDimension());

            long bestSASeed = 0;
            double bestSACost = Double.MAX_VALUE;
            List<Integer> bestSAList = null;
            long bestSATime = 0;

            for (int i = 0; i < 10; i++) 
            {
                long seed = System.currentTimeMillis();
                SimulatedAnnealing sa = new SimulatedAnnealing(tsp.getCities(), seed);

                long beginSA = System.currentTimeMillis();
                List<Integer> saList = sa.run(1, 0.9999, 100000);
                long endSA = System.currentTimeMillis();
                double saCost = sa.calculateListLength(saList);

                if (saCost < bestSACost) 
                {
                    bestSACost = saCost;
                    bestSAList = saList;
                    bestSASeed = seed;
                    bestSATime = endSA - beginSA;
                }
            }

            System.out.println("\nBest Simulated Annealing Results:");
            System.out.println("Seed: " + bestSASeed);
            System.out.println("Cost: " + bestSACost);
            System.out.println("Best SA List: " + bestSAList);
            System.out.println("RunTime (ms): " + bestSATime);

            long bestTSSeed = 0;
            double bestTSCost = Double.MAX_VALUE;
            List<Integer> bestTSList = null;
            long bestTSTime = 0;

            for (int i = 0; i < 10; i++) 
            {
                long seed = System.currentTimeMillis();
                TabuSearch ts = new TabuSearch(tsp.getCities(), seed);

                long beginTS = System.currentTimeMillis();
                List<Integer> tsList = ts.run(10, 100000);
                long endTS = System.currentTimeMillis();
                double tsCost = ts.calculateListLength(tsList);

                if (tsCost < bestTSCost) 
                {
                    bestTSCost = tsCost;
                    bestTSList = tsList;
                    bestTSSeed = seed;
                    bestTSTime = endTS - beginTS;
                }
            }

            System.out.println("\nBest Tabu Search Results:");
            System.out.println("Seed: " + bestTSSeed);
            System.out.println("Cost: " + bestTSCost);
            System.out.println("Best TS List: " + bestTSList);
            System.out.println("RunTime (ms): " + bestTSTime);

        } 
        catch (IOException e) 
        {
            System.err.println("Error loading file: " + selectedFile.getName());
            e.printStackTrace();
        }
    }

    private static void runSingleTest(File selectedFile, Scanner scanner) 
    {
        try 
        {
            TSP tsp = new TSP(selectedFile.getAbsolutePath());
            System.out.println("Dimension: " + tsp.getDimension());

            System.out.print("Enter the seed for Simulated Annealing: ");
            long saSeed = scanner.nextLong();
            System.out.print("Enter the initial temperature for Simulated Annealing: ");
            double initialTemp = scanner.nextDouble();
            System.out.print("Enter the cooling rate for Simulated Annealing: ");
            double coolingRate = scanner.nextDouble();
            System.out.print("Enter the number of iterations for Simulated Annealing: ");
            int saIterations = scanner.nextInt();

            SimulatedAnnealing sa = new SimulatedAnnealing(tsp.getCities(), saSeed);
            long beginSA = System.currentTimeMillis();
            List<Integer> saList = sa.run(initialTemp, coolingRate, saIterations);
            long endSA = System.currentTimeMillis();
            double saCost = sa.calculateListLength(saList);

            System.out.println("\nSimulated Annealing Results:");
            System.out.println("Seed: " + saSeed);
            System.out.println("Cost: " + saCost);
            System.out.println("Best SA List: " + saList);
            System.out.println("RunTime (ms): " + (endSA - beginSA));

            System.out.print("Enter the seed for Tabu Search: ");
            long tsSeed = scanner.nextLong();
            System.out.print("Enter the tabu list size for Tabu Search: ");
            int tabuListSize = scanner.nextInt();
            System.out.print("Enter the number of iterations for Tabu Search: ");
            int tsIterations = scanner.nextInt();

            TabuSearch ts = new TabuSearch(tsp.getCities(), tsSeed);
            long beginTS = System.currentTimeMillis();
            List<Integer> tsList = ts.run(tabuListSize, tsIterations);
            long endTS = System.currentTimeMillis();
            double tsCost = ts.calculateListLength(tsList);

            System.out.println("\nTabu Search Results:");
            System.out.println("Seed: " + tsSeed);
            System.out.println("Cost: " + tsCost);
            System.out.println("Best TS List: " + tsList);
            System.out.println("RunTime (ms): " + (endTS - beginTS));

        } 
        catch (IOException e) 
        {
            System.err.println("Error loading file: " + selectedFile.getName());
            e.printStackTrace();
        }
    }
}