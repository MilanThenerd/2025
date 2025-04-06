import java.io.*;
import java.util.*;

public class TSP 
{
    private String name;
    private int dimension;
    private List<City> cities;

    public TSP(String filePath) throws IOException 
    {
        this.cities = new ArrayList<>();
        loadFromFile(filePath);
    }

    private void loadFromFile(String filePath) throws IOException 
    {
        try (BufferedReader reader = new BufferedReader(new FileReader(filePath))) 
        {
            String line;
            while ((line = reader.readLine()) != null) 
            {
                line = line.trim();
                if (line.startsWith("NAME:")) 
                {
                    this.name = line.split(":")[1].trim();
                } 
                else if (line.startsWith("DIMENSION:")) 
                {
                    this.dimension = Integer.parseInt(line.split(":")[1].trim());
                } 
                else if (line.startsWith("NODE_COORD_SECTION")) 
                {
                    loadCities(reader);
                } 
                else if (line.equals("EOF")) 
                {
                    break;
                }
            }
        }
    }

    private void loadCities(BufferedReader reader) throws IOException 
    {
        String line;
        while ((line = reader.readLine()) != null) 
        {
            line = line.trim();
            if (line.equals("EOF")) 
            {
                break;
            }
            String[] parts = line.split("\\s+");
            int id = Integer.parseInt(parts[0]);
            double x = Double.parseDouble(parts[1]);
            double y = Double.parseDouble(parts[2]);
            cities.add(new City(id, x, y));
        }
    }

    public String getName() 
    {
        return name;
    }

    public int getDimension() 
    {
        return dimension;
    }

    public List<City> getCities() 
    {
        return cities;
    }

    @Override
    public String toString() 
    {
        return "TSP{name='" + name + "', dimension=" + dimension + ", cities=" + cities + "}";
    }
}
