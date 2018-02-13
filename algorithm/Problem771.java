import java.util.HashSet;
import java.util.Set;

class Solution {
    public int numJewelsInStones(String J, String S) {
        Set<Character> set = new HashSet<Character>();
        int count = 0;
        for (int i = 0; i < J.length(); ++i) {
            set.add(J.charAt(i));
        }
        for (int i = 0; i < S.length(); ++i) {
            if (set.contains(S.charAt(i))) ++count;  
        }
        return count;
    }
}
