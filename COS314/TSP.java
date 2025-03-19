import java.io.*;
import java.util.*;

public class TSP {
    private List<int[]> cities;  // List to store city coordinates
    private int numCities;

    // Constructor
    public TSP(List<int[]> cities) {
        this.cities = cities;
        this.numCities = cities.size();
    }

    // Method to read city data from a file
    public static TSP fromFile(String filename) throws IOException {
        List<int[]> cities = new ArrayList<>();
        BufferedReader reader = new BufferedReader(new FileReader(filename));
        String line;
        
        // Skip the header lines
        while ((line = reader.readLine()) != null) {
            if (line.startsWith("NODE_COORD_SECTION")) {
                break;
            }
        }

        // Read city coordinates
        while ((line = reader.readLine()) != null && !line.equals("EOF")) {
            String[] tokens = line.split("\\s+");
            if (tokens.length == 3) {
                int cityIndex = Integer.parseInt(tokens[0]) - 1;  // Convert 1-based index to 0-based
                int x = Integer.parseInt(tokens[1]);
                int y = Integer.parseInt(tokens[2]);
                cities.add(new int[]{x, y});
            }
        }
        reader.close();
        
        // Create a TSP instance with the read cities
        return new TSP(cities);
    }

    // Calculate the distance between two cities using Euclidean distance
    public double calculateDistance(int city1, int city2) {
        int[] city1Coordinates = cities.get(city1);
        int[] city2Coordinates = cities.get(city2);
        return Math.sqrt(Math.pow(city1Coordinates[0] - city2Coordinates[0], 2) + 
                         Math.pow(city1Coordinates[1] - city2Coordinates[1], 2));
    }

    // Calculate the total length of a given tour
    public double calculateTourLength(List<Integer> tour) {
        double length = 0;
        for (int i = 0; i < tour.size() - 1; i++) {
            length += calculateDistance(tour.get(i), tour.get(i + 1));
        }
        length += calculateDistance(tour.get(tour.size() - 1), tour.get(0));  // Returning to start city
        return length;
    }

    // Generate a random tour
    public List<Integer> generateRandomTour() {
        List<Integer> tour = new ArrayList<>();
        for (int i = 0; i < numCities; i++) {
            tour.add(i);
        }
        Collections.shuffle(tour);
        return tour;
    }
}
