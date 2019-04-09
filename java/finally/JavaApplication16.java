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
 *
 * @author Administrator
 */
public class JavaApplication16 {

    /**
     * @param args the command line arguments
     */
    public static void func() throws FileNotFoundException {
        // TODO code application logic here
        new FileInputStream("build.xml");
        Scanner console = new Scanner(in);
        //i: a, 0, 1, 2, 3
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
                throw new RuntimeException("catch2-REc");
            }
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
