/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package staticvarinbase;
import java.util.*;
import static java.lang.System.*;
/*
1. no matter if "abstract" is added or not, there is no change for this program
2. static member in Base class is exactly like its static block: each loaded class's static member
   can have only one copy(ie one instantiation), suppose:
class Some {
    Base obj0;
    Base obj1;//then obj0's mUUID is the same instantiation of obj0's mUUID
}
class Derived0 extends Base {
    //it can use "super" to specify "Base", which like "obj0" in the above
}
class Derived1 extends Base {
    //it can use "super" to specify "Base", which like "obj1" in the above    
}
*/
class Zero{
}
class Base {
    public static UUID mUUID = UUID.randomUUID();
    static {
        out.printf("Base static block:[%s]%n", mUUID.toString());
    }
    void showUUID() {
        UUID uuid = UUID.randomUUID();
        out.printf("%s%n", uuid);
    }
}
/*extends 2 classes at the same time is WRONG: "need { or implements"
class C0 extends Base, Zero {
*/
class C0 extends Base {
    static UUID mUUID2 = UUID.randomUUID();
    static {
        out.printf("C0 static block[%s]%n", mUUID.toString());
    }
    C0() {
        out.printf("C0's static mUUID2[%s]%n", mUUID2);
    }
}

class C1 extends Base {
    static UUID mUUID2 = UUID.randomUUID();
    static {
        out.printf("C1 static block[%s]%n", mUUID.toString());
    }
    C1() {
        out.printf("%s:mUUID2[%s]%n", String.valueOf(this), mUUID2);
    }
}
/**
 *
 * @author Administrator
 */
public class StaticVarInBase<T> {
    T obj;
    public void set(T obj) {
        this.obj = obj;
    }
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        C0 c0 = new C0();//Base's static block runs before derived class's static block
        out.printf("\tAfter new C0%n");
        C1 c1 = new C1();//Base's static block will not show for it has been run once
        out.printf("\tAfter new 1st C1%n");
        C1 c1_1 = new C1();//C1's static block will not show for ut has been run once
        out.printf("\tAfter new 2nd C1%n");
        //all the classes derived from Base will see the same copy for the Base's static member
        //Base's static member is bound to Base's .class
        out.printf("%b\t%b\t%b%n",
                c1 == c1_1,//false
                c0.mUUID == c1.mUUID, //true
                c1.mUUID == c1_1.mUUID);//true
        c1.mUUID = UUID.randomUUID();
        //then all derived class from Base will see the Base's static member changed!
        out.printf("c0:%s%n" +
                "c1:%s%n" +
                "c1_1:%s\t%b\t%b%n",
                c0.mUUID.toString(),
                c1.mUUID.toString(),
                c1_1.mUUID.toString(), c0.mUUID == c1.mUUID, c1.mUUID == c1_1.mUUID);
        out.printf("%b\t%b%n", 
                c0.mUUID2 == c1.mUUID2,//false
                c1.mUUID2 == c1_1.mUUID2);//true
    }
    
}
