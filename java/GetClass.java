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
        cls = ((A1)oA2).getClass();
        compare(classes, cls);
        cls = ((A0)oA2).getClass();
        compare(classes, cls);
        compare(classes, oA2.getClass2());
    }   
}
