/*
function template: it can be static or non static, either is OK
 */
package javaapplication22;
class A {
}
class B extends A {
}
class C extends B {
}
class D {
}
/**
 *
 * @author Administrator
 */
public class FuncTemplate {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        A[] a = new A[3];
        elemOf(a);
        B[] b = new B[3];
        elemOf(b);
        C[] c = new C[3];
        elemOf(c, 0);
        elemOf(c);
        D[] d = new D[3];
        //elemOf(d);//wrong: cannot find method, for class D has no relation to class A
        //C cObj = findViewById(c, 0);//Wrong: A cannot be converted to C, so cast is a MUST!
        C cObj = (C)findViewById(c, 0);
        A aObj = findViewById(c, 0);//OK
    }
    public static <T> T elemOf(T[] objs, int index) {
        return objs[index];
    }
    public static <T extends A> T elemOf(T[] objs) {
        return objs[0];
    }
    public static A findViewById(A[] objs, int i) {
        return objs[i];
    }
}
