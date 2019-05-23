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
        A2 oA2_1 = new A2();//All A2's instantiations will return the same Class<A2>
        compare(classes, oA2_1.getClass());
        /*
        getClass is native interface whose source is not in Object.java:
        https://stackoverflow.com/questions/26626259/how-does-the-getclass-is-implemented-in-java
        The following is my guess for it JNI implementation:
        1. Object has an internal field "Class<?> cls" which is private and instance method getClass() is needed to get it
           Note that getClass is not static for it needs "this" to access a specified "Object" to get its "Class<?> cls" field.
        2. The following use "SomeClass" to specify that the class to instantiate object. SomeClass can be "class Object" or
           ANY class. All classes inherit from "Class Object", U know.
           This field is set during SomeClass's Object instantiation. In other words, the class type is complete definite when
           its instantiation is generated. Object's "Class<?> cls" field is assigned with "Class<SomeClass>" during object instantiation
           and it will not changed since then. Note each Class has only one instantiation of Class<?> instantiation.
        3. So no matter how to cast this:
           a. cast explicitely like "(type)this"
           b. HIGEND class is used to instantiate objects and it is at the end of inheriting chain. LOWEND "class Object" is at the other end of this chain.
              If there are many middle class between HIGHEND "class SomeClass" and LOWEND "class Object", and in any middle class's instance method there is
              calling to getClass()(equals to this.getClass()). Its return is completely not certain until program is run. The Class<?>
              returned by getClass is not sure at all for you don't know the HIGHEND class who inherits from the middle class.
              If the highend class is SomeClass, then this.getClass in middle class will return Class<SomeClass>.
              If the highend class is OtherClass, then this.getClass in middle class will return Class<OtherClass>
              In other word, getClass here is like a pure vitual member function which has not been implemented. Any middle class will not and should not
              implment it but they can use it, for it is legal to use pure virtual member func in any level's classs.
              Only the highend class like SomeClass or OtherClass listed above, can define and must define the getClass.
           In brief, to know getClass's real return, U must know the highend class who is at the end of inheriting chain, and any middle class
           in the chain is free to use getClass() and they cannot know the real return for getClass(). 
        */
    }   
}
