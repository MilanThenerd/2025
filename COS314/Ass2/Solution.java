import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class Solution {
    public List<List<Integer>> routes = new ArrayList<>();
    public List<Double> routesTime = new ArrayList<>();
    public double totalScore = 0;
    public Set<Integer> visitedNodes = new HashSet<>();
    
    public Solution copy() {
        Solution copy = new Solution();
        for (List<Integer> route : routes) {
            copy.routes.add(new ArrayList<>(route));
        }
        copy.routesTime = new ArrayList<>(routesTime);
        copy.totalScore = totalScore;
        copy.visitedNodes = new HashSet<>(visitedNodes);
        return copy;
    }
}