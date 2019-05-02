/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package wordcound;
import java.util.*;
import static java.lang.System.*;
/**
 *
 * @author Administrator
 */
public class WordCount {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        Scanner console = new Scanner(System.in);
        out.println("Input:");
        Set words = tokenSet(console.nextLine());
        out.printf("%d:%s%n", words.size(), words);
        // TODO code application logic here
    }
    static Set tokenSet(String line) {
        String[] tokens = line.split("  *");
        for (int i = 0; i < tokens.length; i++) {
            out.printf("[%s]", tokens[i]);
        }
        /*https://stackoverflow.com/questions/14535384/instanceof-for-generic-with-or-without
        1. Since instanceof will perform a check at runtime, not at compile time, you can not check for Type<GenericParameter> in an instanceof ... expression.
        2. instanceof List<?> boils down to instanceof List - which are exactly the same
        */
        out.printf("%n%b %b%n", Arrays.asList(tokens) instanceof List,
                Arrays.asList(tokens) instanceof List<?>);
        Set words = new HashSet(Arrays.asList(tokens));
        return words;
    }
    
}
