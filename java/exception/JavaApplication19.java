/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javaapplication19;
import static java.lang.System.*;
import java.io.*;
import java.util.*;

class Resource implements AutoCloseable {
    private int i = 0;
    @Override
    public void close() {
        switch(i) {
            case 3:
                out.println("case 3: close");
                throw new RuntimeException("RRE");
        }
        out.println("close");
    }
    Resource(Object o) {
    }
    public void set(int i) {
        this.i = i;
    }
}
/**
 *
 * try (...) {...} + multi catch, demo
 */
public class JavaApplication19 {

    public static void main(String[] args) throws FileNotFoundException {
        // TODO code application logic here
        Scanner console = new Scanner(System.in);
        String file = console.next();
        /*
        input file name:
        1. "a.txt" that doesn't exist under current dir. The following uses try() {} with 2 catch:
            try will wrap the FileNotFoundException(It is said to be so in LXL's book)but its catch will REALLY handle it!
            THERE IS NO SUCCESSFUL INSTANTIATE FOR class Resouce in try(), SO THERE IS NO NEED TO CALL ITS close()!
run:
a.txt
fnfe
main-
BUILD SUCCESSFUL (total time: 4 seconds)

        2. "build.xml" that does exist under current dir
            then 0 or 1 to see different results;
        the output of input 0:
run:
build.xml
try+, input:0
try-
close
main-
BUILD SUCCESSFUL (total time: 12 seconds)
        the output of input 1:
run:
build.xml
try+, input:1
close
re
main-
BUILD SUCCESSFUL (total time: 4 seconds)
        the output of input 2:
run:
build.xml
try+, input:2
close
fnfe
main-
BUILD SUCCESSFUL (total time: 4 seconds)
        the output of input 3, whose close() will throw Exception:
run:
build.xml
try+, input:3
case 3: close
fnfe
main-
BUILD SUCCESSFUL (total time: 3 seconds)
        */
        int i;
        try (Resource ms = new Resource(new FileInputStream(file))) {
            out.print("try+, input:");
            i = console.nextInt();
            if (i == 1)
                throw new RuntimeException("RE");
            else if (i >= 2) {
                ms.set(i);
                throw new FileNotFoundException("FNFE");
            }
            out.println("try-");
        } catch (RuntimeException re) {
            out.println("re");
        } catch (FileNotFoundException fnfe) {
            out.println("fnfe");
        }
        out.println("main-");
    }
}
    