/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package arraylist;
import static java.lang.System.*;
interface Comparator<T> {//real Comparator is in java.util;
    int compare(T o1, T o2);
}

class StringComparator0 implements Comparator<String> {
    @Override
    public int compare(String s1, String s2) {
        int ret = s1.compareTo(s2);
        out.printf("0: %s %s: %d%n", s1, s2, ret);
        return ret;
    }
}

class StringComparator1 implements Comparator {
    @Override
    public int compare(Object o1, Object o2) {
        String s1 = (String)o1;
        String s2 = (String)o2;
        int ret = s1.compareTo(s2);
        out.printf("1: %s %s: %d%n", s1, s2, ret);
        return 0;
    }
}
/*
It seems meaningless to do so since the derived class generally give its own comparing method. So
no need to use generic type!
*/
class StringComparator2<E> implements Comparator<E> {
    @Override
    public int compare(E o1, E o2) {
        int h1 = o1.hashCode();
        int h2 = o2.hashCode();
        int ret = h1 - h2;
        out.printf("0x%x - 0x%x = 0x%x%n", h1, h2, ret);
        return ret;
    }
}

interface MyCollection<E> {
    void myCollectionPrint();
}

interface MyList<E> extends MyCollection<E> {
    void myListPrint();
}

/*
Both 2 pure virtual funcs must be defined(but you can use empty func)
if you don't want to add "abstract" before class MyArrayList<E>.
*/
class MyArrayList<E> implements MyList<E> {
    @Override
    public void myCollectionPrint() {
        out.printf("MyArrayList<E>'s myCollectionPrint%n");
    }
    @Override
    public void myListPrint() {
        out.printf("MyArrayList<E>'s myListPrint%n");
    }
}

class A {
    void findViewById(A obj) {
    }
}

class B extends A {
    /*
    <T extends A> void findViewById(T obj) {//wrong:build error
    }
    */
}
/**
 *
 * @author Administrator
 */
public class ImplementsInterface {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        //Comparator<T> c0 = new StringComparator0();//wrong:cannot find symbol 'class T'
        //Comparator c0 = new StringComparator0();//ok, the same as "Comparator<String> c0 = new StringComparator0();"
        Comparator<String> c0 = new StringComparator0();//ok
        c0.compare("a", "b");
        c0 = new StringComparator1();
        c0.compare("a", "b");
        c0 = new StringComparator2<>();
        c0.compare("A", "B");
        Comparator<StringComparator0> c1 = new StringComparator2<>();
        c1.compare(new StringComparator0(), new StringComparator0());
        MyList<String> ml = new MyArrayList<>();//MyArrayList extends MyList, so the assignment here is ok!
        /*
        here the argument of type "MyList<String>" to function test(MyCollection<?>) is OK:
        MyList implements MyCollection. Besides, "String" conforms to "? extends Object"
        */
        test(ml);
    }
    
    static void test(MyCollection<? extends Object> arg) {
        arg.myCollectionPrint();
    }
}
