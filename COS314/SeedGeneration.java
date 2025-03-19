import java.util.Random;

public class SeedGeneration {
    public static void main(String[] args) {
        long seed = System.currentTimeMillis();
        Random rand = new Random(seed);
        System.out.println("Seed Value: " + seed);
    }
}