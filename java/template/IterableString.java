/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package iterablestring;
import static java.lang.System.out;
/**
@FunctionalInterface cannot be used on the following Iterator<T>
*/
interface Iterator<T> {
    boolean hasNext();
    T next();
}
@FunctionalInterface
interface Consumer<T> {
    void accept(T obj);
}

@FunctionalInterface
interface Iterable<T> {
    /*
    u canot define variable like "Iterator<T> iterator", for this is an interface;
    but u can use a abstract function to return it
    */
    public abstract Iterator<T> iterator();
    /*
    From https://dzone.com/articles/interface-default-methods-java:
    After introducing Default Method, it seems that interfaces and abstract classes are the same. However, they
    are still a different concept in Java 8. The abstract class can define constructors. They are more structured
    and can have a state associated with them. While in contrast, default method can be implemented only in the terms
    of invoking other interface methods, with no reference to a particular implementation's state. Hence, both are
    used for different purposes and choosing between two really depends on the scenario context.
    Additionally, methods in classes can use and modify method arguments as well as the fields of their class, but
    default method, on the other hand, can only access its arguments as interfaces do not have any state.
    In brief:
    An interface cannot define member variables(BUT you can define an abstract func to return it like the above iterator),
    so default func cannot modify member variable, which is acceptable in func of abstract class(abstract class can define
    their own member variables, the "abstract" generally only means there is abstract member func in this class. Of course,
    one claas without abstract member func can still be marked "abstract", then this class cannot be instantiated object).
    */
    default void forEach(Consumer<? super T> action) {
        Iterator<T> iterator = iterator();
        while (iterator.hasNext()) {
            T obj = iterator.next();
            action.accept(obj);
        }
    }
}

public class IterableString implements Iterable<String> {

    /**
     * @param args the command line arguments
     */
    private String mString;
    private int mIndex;
    IterableString(String str) {
        mString = str;
        mIndex = 0;
    }
    
    void reset() {
        mIndex = 0;
        out.printf("%n");
    }
    
    @Override
    public Iterator<String> iterator() {
        return new Iterator<>() {//all abstract method hasNext and next must be defined, build error otherwise.
            @Override
            public boolean hasNext() {
                boolean ret = true;
                if (mIndex >= mString.length())
                    ret = false;
                return ret;
            }
            @Override
            public String next() {
                //String sub = String.valueOf(mString.toCharArray(), mIndex, mString.length() - mIndex);
                String sub = String.valueOf(mString.toCharArray(), mIndex, 1);/*to make "is0.forEach(new Consumer<Object>()"
                can print ascii via String's hashCode, we have to use a length==1's String*/
                mIndex++;
                return sub;
            }
        };
    }
    static void forEach(Iterable iterable) {
        Iterator iterator = iterable.iterator();
        while (iterator.hasNext()) {
            String str = (String)iterator.next();
            out.printf("{%c}", str.charAt(0));
        }
    }
    static void forEach2(Iterable iterable) {
        //for(Object o: iterable) {//iterable here is implemented by me so enhanced
        //loop seems cannot be used for self-implmented iterface.*/
        //}
    }
    public static void main(String[] args) {
        IterableString is0 = new IterableString("1st test");
        forEach(is0);
        is0.reset();//necessary, the next forEach will do nothing if u don't reset!
        is0.forEach(new Consumer<Object>() {
            @Override
            public void accept(Object o) {
                /*
                The hash code for a String object is computed as s[0]*31^(n-1) + s[1]*31^(n-2) + ... + s[n-1]
                so you can get ascii iff the string length==1, like String "A"
                */
                out.printf("[%c]", o.hashCode());
            }
        });
        is0.reset();//necessary, the next forEach will do nothing if u don't reset!
        is0.forEach(new Consumer<>() {/*here it's actually "Consumer<String>()", for is0
            is of type "IterableString" who implements "iteratable<String>". And default func
            forEach(Consumer<? super T> action) in iterable uses argument of "Consumer<? super String>"
            */
            @Override
            public void accept(String s) {//try change here to "Object s" to see the build error info!
                out.printf("<%c>", s.charAt(0));
            }
        });
        is0.reset();//necessary, the next forEach will do nothing if u don't reset!
        is0.forEach(s->out.printf("[%c]", s.charAt(0)));//lambda
    }
    
}
