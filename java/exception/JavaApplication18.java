/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javaapplication18;
import static java.lang.System.*;
import java.io.*;
import java.util.*;

class Resource implements AutoCloseable {
    private int i = 0;
    @Override
    public void close() {
        switch (i) {
            case 3:
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
 * try (...) {...} + catch, demo
 */
public class JavaApplication18 {

    public static void main(String[] args) throws FileNotFoundException {
        // TODO code application logic here
        Scanner console = new Scanner(System.in);
        String file = console.next();
        /*
        input file name:
        1. "a.txt" that doesn't exist under current dir. The following uses try() {} with 1 catch:
            try will wrap the FileNotFoundException(It is said to be so in LXL's book)but its catch will not handle it!
        run:
a.txt
Exception in thread "main" java.io.FileNotFoundException: a.txt (系统找不到指定的文件。)
	at java.base/java.io.FileInputStream.open0(Native Method)
	at java.base/java.io.FileInputStream.open(FileInputStream.java:219)
	at java.base/java.io.FileInputStream.<init>(FileInputStream.java:157)
	at java.base/java.io.FileInputStream.<init>(FileInputStream.java:112)
	at javaapplication18.JavaApplication18.main(JavaApplication18.java:40)
        2. "build.xml" that does exist under current dir
            then 0 or 1 to see different results;
        the output of input 0:
run:
build.xml
try+, input:0
try-
close
main-
BUILD SUCCESSFUL (total time: 7 seconds)
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
Exception in thread "main" java.io.FileNotFoundException: FNFE
	at javaapplication18.JavaApplication18.main(JavaApplication18.java:69)

        the output of input 3: if the close throw Exception, add it to old Exception
        generated in {} block of try
run:
build.xml
try+, input:3
Exception in thread "main" java.io.FileNotFoundException: FNFE
	at javaapplication18.JavaApplication18.main(JavaApplication18.java:83)
	Suppressed: java.lang.RuntimeException: RRE
		at javaapplication18.Resource.close(JavaApplication18.java:17)
		at javaapplication18.JavaApplication18.main(JavaApplication18.java:76)
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
        }
        out.println("main-");
    }
}
    