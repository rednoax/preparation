/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javaapplication15;

import java.io.FileNotFoundException;

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
        if (args.length == 1)
            throw new FileNotFoundException("a");
        System.out.println("1");
        try {//必须存在try，若只有finally块会报错"有finally，但没有try"
            System.out.println("2");
        }
        finally {
            System.out.println("3");
        }    
        System.out.println("4");
    }
    
}
