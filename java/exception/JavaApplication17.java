/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javaapplication17;
import static java.lang.System.*;
import java.io.*;
import java.util.*;

class Resource implements AutoCloseable {
    @Override
    public void close() {
        out.println("close");
    }
    Resource(Object o) {
    }
}
/**
 *
 * try (...) {...}. no catch demo
 */
public class JavaApplication17 {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws FileNotFoundException {
        // TODO code application logic here
        Scanner console = new Scanner(System.in);
        String file = console.next();
        /*
        input file name:
        1. "a.txt" that doesn't exist under current dir. The following uses try() {} without any catch:
            try's () will not handle the FileNotFoundException in it!
run:
a.txt
Exception in thread "main" java.io.FileNotFoundException: a.txt (系统找不到指定的文件。)
	at java.base/java.io.FileInputStream.open0(Native Method)
	at java.base/java.io.FileInputStream.open(FileInputStream.java:219)
	at java.base/java.io.FileInputStream.<init>(FileInputStream.java:157)
	at java.base/java.io.FileInputStream.<init>(FileInputStream.java:112)
	at javaapplication17.JavaApplication17.main(JavaApplication17.java:64)
C:\Users\Administrator\AppData\Local\NetBeans\Cache\10.0\executor-snippets\run.xml:111: The following error occurred while executing this line:
C:\Users\Administrator\AppData\Local\NetBeans\Cache\10.0\executor-snippets\run.xml:68: Java returned: 1
BUILD FAILED (total time: 3 seconds)
        
        2. "build.xml" that does exist under current dir
            then 0 or 1 to see different results;
        the output of input 0:
run:
build.xml
try+
0<---input 0
try-
close
main-
BUILD SUCCESSFUL (total time: 6 seconds)
        the output of input 1:
run:
build.xml
try+
1<---input 1
close
Exception in thread "main" java.lang.RuntimeException: RE
	at javaapplication17.JavaApplication17.main(JavaApplication17.java:58)        
        */
        try (Resource ms = new Resource(new FileInputStream(file))) {
            out.println("try+");
            if (console.nextInt() > 0)
                throw new RuntimeException("RE");
            out.println("try-");
        } 
        /*
        catch (RuntimeException re) {
            out.println("re");
        }
        */
        out.println("main-");
    }
    
}
    