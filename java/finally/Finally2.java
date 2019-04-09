/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package finally2;
import static java.lang.System.out;
/**
 *
 * @author Administrator
 * try catch finally: if catch throw Exception, run finally first!
 * if one func in function stack throw Exception, it will be transfered to upper func
run:
main+
a+
before b's throw
catch+
finally
Exception in thread "main" java.lang.RuntimeException: b
	at finally2.Finally2.b(Finally2.java:15)
	at finally2.Finally2.a(Finally2.java:20)
	at finally2.Finally2.main(Finally2.java:30)
 */
public class Finally2 {
    private static void b() {
        out.println("before b's throw");
        throw new RuntimeException("b");
        //out.println("a-");
}
    private static void a() {
        out.println("a+");
        b();
        out.println("a-");
    }
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        try {
            out.println("main+");
            a();
            out.println("main-");
        } catch (Throwable t) {
            out.println("catch+");
            throw t;
            //out.println("main-");
        }
        finally {
            out.println("finally");
        }
        out.println("main fin");
    }
    
}
