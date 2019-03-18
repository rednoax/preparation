/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package cc.openhome;
class Button {
    Button(String s) {//so that there will be no default cons without argument
        System.out.println(s);
    }
}

class O0 {
    public O0() {
        System.out.println("O0");
    }
}

class O1 extends O0 {
    public O1() {
        //no super
        System.out.println("O1");
    }
}

class O2 extends O0 {
    //no cons at all
}
/**
 *
 * @author Administrator
 */
public class Array {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        Button[] array = new Button[100];//no Button cons can be called!
        O1 obj1 = new O1();
        O2 obj2 = new O2();
    }
    
}
