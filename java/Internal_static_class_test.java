/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package cc.openhome;
import static java.lang.System.*;
final class R {
    public static final class layout {
        public final static int activity_quiz = 0x7f030017;
        public layout(String s) {
            out.printf("layout cons " + s + ":");
        }
    }
}
/**
 *
 * @author Administrator
 */
public class Internal_static_class_test {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        R.layout o = new R.layout("static internal");
        out.printf("%x%n", R.layout.activity_quiz);
    }
    
}
