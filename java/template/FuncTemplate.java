/*
function template: it can be static or non static, either is OK
 */
package javaapplication22;
class A {
}
class B extends A {
}
class C extends B {
    private class InnerC {
    }
}
class D {
}
/*//build error:? cannot be used in class template or function template DEFINATION
class E<?> {
}
*/
class E<T> {//ok
}
class F<T extends A> {//T can only be class A or its direct/indirect child class
}

interface MyInterface {
}

class G implements MyInterface {
}

class H {
    H(String s) {
    }
    MyInterface i;
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
        C obj = new C();
        //C.InnerC obj2  = obj.new InnerC();//wrong:C.InnerC is private in C
        G objG = new G();
        System.out.printf("%b\t%s%n", objG instanceof Object, objG);
        /*OK: even class H has no cons "H()"; besides, its member i is actually an interface ptr
        which will not prevent the instantaion of class H
        */
        H[] objH = new H[3];
        //H objH2 = new H();//wrong!
        // TODO code application logic here
        A[] a = new A[3];
        elemOf(a);
        B[] b = new B[3];
        elemOf(b);
        C[] c = new C[3];
        elemOf(c, 0);
        //C objC = (C)elemOf(c);//not wrong but no need since elemOf(c) return type is Class C
        D[] d = new D[3];
        //elemOf(d);//wrong: cannot find method, for class D has no relation to class A
        //C cObj = findViewById(c, 0);//wrong: A cannot be converted to C, so cast is a MUST!
        C cObj = (C)findViewById(c, 0);
        A aObj = findViewById(c, 0);//OK
        F<A> f0 = new F<>();
        F<B> f1 = new F<>();
        F<C> f2 = new F<>();
        //F<D> f3 = new F<>();//wrong for D is not extended from A
        F<? extends A> f4 = f0;
        f4 = f1;
        f4 = f2;
        F<A> f5 = f0;
        //f5 = f1;//wrong: F<B> cannot be converted to F<A>
        //f5 = f2;//wrong: F<C> cannot be converted to F<A>
        //F<C> f7 = f1;//wrong:F<B> cannot be converted to F<C>
    }
    public static <T> T elemOf(T[] objs, int index) {
        return objs[index];
    }
    public static <T extends A> T elemOf(T[] objs) {//T can be class A or its direct/indirect child class
        return objs[0];
    }
    public static A findViewById(A[] objs, int i) {
        return objs[i];
    }
    public static <T> void func(T arg) {//build error after replacing T with ? in function template defination!
    }
}
