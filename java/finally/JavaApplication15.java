/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javaapplication15;

import java.io.FileNotFoundException;
import static java.lang.System.*;
import java.util.*;
/**
 *
 * @author Administrator
 */
public class JavaApplication15 {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws FileNotFoundException{
        // TODO code application logic here
        Scanner console = new Scanner(in);
        out.print("input an Integer:");
        int i = console.nextInt();
        //if input a, the following code will never be run, just throw java.util.InputMismatchException
        /*input 0:
run:
input an Integer:0
before enter try
f
Exception in thread "main" java.io.FileNotFoundException: in try        
        */
        /*input 1:
run:
input an Integer:1
before enter try
in try
f
at last
BUILD SUCCESSFUL (total time: 3 seconds)
        */
        out.println("before enter try");
        try {//必须存在try，若只有finally块会报错"有finally，但没有try"
            switch (i) {
                case 0:
                    throw new FileNotFoundException("in try");
                default:
                    out.println("in try");
            }
        }
        finally {
            out.println("f");
        }    
        out.println("at last");
    }
    
}
