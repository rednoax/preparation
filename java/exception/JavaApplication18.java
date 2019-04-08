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
    public void close() throws FileNotFoundException {
        out.printf("case %d: close%n", i);
        switch (i) {
            case 3:
            case -1:
                throw new RuntimeException("RRE");
            case -2:
                throw new FileNotFoundException("RFNFE");
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
	at javaapplication18.JavaApplication18.main(JavaApplication18.java:93)
        2. "build.xml" that does exist under current dir
            then 0 or 1 to see different results;
        the output of input 0:
run:
build.xml
try+, input:0
try-
case 0: close
close
main-
BUILD SUCCESSFUL (total time: 6 seconds)
        the output of input 1:
run:
build.xml
try+, input:1
case 1: close
close
re
main-
BUILD SUCCESSFUL (total time: 10 seconds)
        the output of input 2:
run:
build.xml
try+, input:2
case 2: close
close
Exception in thread "main" java.io.FileNotFoundException: FNFE
	at javaapplication18.JavaApplication18.main(JavaApplication18.java:101)
C:\Users\Administrator\AppData\Local\NetBeans\Cache\10.0\executor-snippets\run.xml:111: The following error occurred while executing this line:
C:\Users\Administrator\AppData\Local\NetBeans\Cache\10.0\executor-snippets\run.xml:68: Java returned: 1
BUILD FAILED (total time: 4 seconds)

        the output of input 3: if the close throw Exception, add it to old Exception
        generated in {} block of try
run:
build.xml
try+, input:3
case 3: close
Exception in thread "main" java.io.FileNotFoundException: FNFE
	at javaapplication18.JavaApplication18.main(JavaApplication18.java:105)
	Suppressed: java.lang.RuntimeException: RRE
		at javaapplication18.Resource.close(JavaApplication18.java:19)
		at javaapplication18.JavaApplication18.main(JavaApplication18.java:98)
C:\Users\Administrator\AppData\Local\NetBeans\Cache\10.0\executor-snippets\run.xml:111: The following error occurred while executing this line:
C:\Users\Administrator\AppData\Local\NetBeans\Cache\10.0\executor-snippets\run.xml:68: Java returned: 1
BUILD FAILED (total time: 5 seconds)
        the output of -1, try {} block will not emit Exception but close() emits a catchable Exception:
run:
build.xml
try+, input:-1
try-
case -1: close
re
main-
BUILD SUCCESSFUL (total time: 5 seconds)
        the output of -2, try {} block will not emit Exception but close() emits a UN-CATCHABLE Exception:
run:
build.xml
try+, input:-2
try-
case -2: close
Exception in thread "main" java.io.FileNotFoundException: RFNFE
	at javaapplication18.Resource.close(JavaApplication18.java:21)
	at javaapplication18.JavaApplication18.main(JavaApplication18.java:121)        
        */
        int i;
        try (Resource ms = new Resource(new FileInputStream(file))) {
            out.print("try+, input:");
            i = console.nextInt();
            ms.set(i);
            if (i == 1)
                throw new RuntimeException("RE");
            else if (i >= 2) {
                throw new FileNotFoundException("FNFE");
            }
            out.println("try-");
        } catch (RuntimeException re) {
            out.println("re");
        }
        out.println("main-");
    }
}
    