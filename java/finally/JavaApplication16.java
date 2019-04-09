/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javaapplication16;
import java.util.Scanner;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.io.FileInputStream;
import static java.lang.System.*;
/**
1. if there is Exception before try, then try[catch]finally will be all skipped!
run:
a
Exception in thread "main" java.util.InputMismatchException
	at java.base/java.util.Scanner.throwFor(Scanner.java:939)
	at java.base/java.util.Scanner.next(Scanner.java:1594)
	at java.base/java.util.Scanner.nextInt(Scanner.java:2258)
	at java.base/java.util.Scanner.nextInt(Scanner.java:2212)
	at javaapplication16.JavaApplication16.func(JavaApplication16.java:26)
	at javaapplication16.JavaApplication16.main(JavaApplication16.java:48)
	
2. no Exception in try, when try's {} finishes, finally 's {} will be called
run:
0
before try
try+
try-
f
after finally
BUILD SUCCESSFUL (total time: 2 seconds)

3. try's {} throw a Exception that cannot be handled by catch, then before this throw, run finally's {} first
run:
1
before try
try+
try throw an Exception that CANNOT be handled by catch
f
Exception in thread "main" java.lang.RuntimeException: tREt
	at javaapplication16.JavaApplication16.func(JavaApplication16.java:32)
	at javaapplication16.JavaApplication16.main(JavaApplication16.java:50)

4. try's {} throw a Exception that can be handled by catch, catch will not throw any Exception, finally's {} will be
   run before catch's finishes(the finish is fin of code but not via return)
run:
2
before try
try+
try throw an Exception that can be handled by catch
catch
before catch fin
f
after finally
BUILD SUCCESSFUL (total time: 1 second)

5. try's {} throw a Exception that can be handled by catch, catch will then throw new Exception, finally's {} will be
   run before catch's throw
run:
3
before try
try+
try throw an Exception that can be handled by catch
catch
catch>2
f
Exception in thread "main" java.lang.RuntimeException: catch2-REc
	at javaapplication16.JavaApplication16.func(JavaApplication16.java:42)
	at javaapplication16.JavaApplication16.main(JavaApplication16.java:51)

6. try's {} throw a Exception that can be handled by catch, catch will not throw any Exception, finally's {} will be
   run before catch's finishes(the finish is NOT fin of code but via return)
run:
4
before try
try+
try throw an Exception that can be handled by catch
catch
catch>2
before catch return
f
BUILD SUCCESSFUL (total time: 1 second)
 */
public class JavaApplication16 {

    /**
     * @param args the command line arguments
     */
    public static void func() throws FileNotFoundException {
        // TODO code application logic here
        new FileInputStream("build.xml");
        Scanner console = new Scanner(in);
        //i: a, 0, 1, 2, 3, 4
        int i = console.nextInt();
        out.println("before try");
        try {
            out.println("try+");
            if (i == 1) {
                out.println("try throw an Exception that CANNOT be handled by catch");
                throw new RuntimeException("tREt");
            } else if (i > 1) {
                out.println("try throw an Exception that can be handled by catch");
                throw new IOException("tIOE");
            }
            out.println("try-");
        } catch (IOException re) {
            out.println("catch");
            if (i > 2) {
                out.println("catch>2");
                switch(i) {
                    case 3:
                        throw new RuntimeException("catch2-REc");
                    case 4:
                        out.println("before catch return");
                        return;
                }
            }
            out.println("before catch fin");
        }
        finally {
            out.println("f");
        }
        out.println("after finally");
    }
    public static void main(String[] args) throws FileNotFoundException {
        func();
    }
}
