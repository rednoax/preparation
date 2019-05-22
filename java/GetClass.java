/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package getclass;
import static java.lang.System.*;

abstract class A0 {
    Class<?> getClass2() {
        return getClass();
    }
}

class A1 extends A0 {
}

class A2 extends A1 {
}
/**
 *
 * @author Administrator
 */
public class GetClass {
    public static void compare(Class<?>[] classes, Class<?> obj) {
        for (int i = 0; i < classes.length; i++) {
            Class<?> cls = classes[i];
           out.printf("%s\t%b%n", cls, cls == obj);
        }        
        out.println();
    }
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        Class<?>[] classes = {
            A0.class,
            A1.class,
            A2.class,
        };
        for (int i = 0; i < classes.length; i++) {
            Class<?> cls = classes[i];
           out.printf("%s\t%s%n", cls.toGenericString(), cls.toString());
        }
        A2 oA2 = new A2();
        Class<?> cls = oA2.getClass();
        compare(classes, cls);
        cls = ((A1)oA2).getClass();//REASON A:this contains virt func pointers, no matter how to cast this, the A2 Object(refered by this)'s virt func ptr will not change
        compare(classes, cls);
        cls = ((A0)oA2).getClass();//the same to REASON A
        compare(classes, cls);
        compare(classes, oA2.getClass2());//oA2 supply this, which refer to Class A2 instantiation whose virt func getClass return A2
        A1 oA1 = new A1();
        compare(classes, oA1.getClass());//oA1 supply this, which refer to Class A1 instantiation whose virt func getClass return A1
        oA1 = oA2;
        compare(classes, oA1.getClass());//oA1 supply this, which refer to Class A2 instantiation whose virt func getClass return A2
    }   
}
