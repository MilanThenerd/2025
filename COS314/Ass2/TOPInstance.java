import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

public class TOPInstance {
    public int n;                  // Number of nodes
    public int m;                  // Number of vehicles
    public double tmax;            // Maximum route time
    public Node[] nodes;           // Nodes (0 is depot)
    public double[][] distances;   // Distance matrix
    
    public void readInput(File file) throws IOException {
        try (BufferedReader br = new BufferedReader(new FileReader(file))) {
            // Read n, m, tmax
            String line = br.readLine();
            n = Integer.parseInt(line.split("\\s+")[1]);
            
            line = br.readLine();
            m = Integer.parseInt(line.split("\\s+")[1]);
            
            line = br.readLine();
            tmax = Double.parseDouble(line.split("\\s+")[1]);
            
            // Read nodes
            nodes = new Node[n];
            for (int i = 0; i < n; i++) {
                line = br.readLine();
                String[] parts = line.trim().split("\\s+");
                double x = Double.parseDouble(parts[0]);
                double y = Double.parseDouble(parts[1]);
                int score = Integer.parseInt(parts[2]);
                nodes[i] = new Node(x, y, score);
            }
            
            // Calculate distances
            calculateDistances();
        }
    }
    
    private void calculateDistances() {
        distances = new double[n][n];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                distances[i][j] = Math.sqrt(Math.pow(nodes[i].x - nodes[j].x, 2) + 
                                          Math.pow(nodes[i].y - nodes[j].y, 2));
            }
        }
    }
}