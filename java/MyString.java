/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package mystring;
import static java.lang.System.*;
import java.util.*;
import java.util.function.*;

interface MyInterface {
    String myToString();
    int hashCode();//Whether this line exists, the result of this program will no change at all
    default void swim() {}//default function must has defination, build error otherwise!
}

class MyObject {
    public String myToString() {
        String s = "MyObject";
        out.println(s);
        return s;
    }
}

class MyString2 extends MyObject implements MyInterface {
    /*
    both base class and interface has the same signature function;
    You can skip overriding it since base class has do so.
    That's WHY when u use Comparator<T>, among its only 2 instance methods:
    equals and compare, u only need to override the latter. equals has
    been override in class Object
    */
}
/*
If there is the same func in both inherited class and inherited interface:
no matter child class instantiation is assigned to super class pointer or interface pointer,
their call to the func will run child's own overrided func
*/
public class MyString extends MyObject implements MyInterface {
    @Override
    public String myToString() {
        String s = "MyString";
        out.println(s);
        return s;
    }
    public static void main(String[] args) {
        int i$ = 0;//the name seems strange but it can contain $ indeed!
        Date date = new Date();
        out.printf("%s%n", date.toString());
        MyString ms = new MyString();
        MyInterface mi = ms;//no cast is needed since big assigned to small
        mi.myToString();
        MyObject mo = ms;//no cast is needed since big assigned to small
        mo.myToString();
        out.printf("%b%n", mo == mi);
        ms = (MyString)mo;//cast is must since small assigned to big
        ms.myToString();
        mo = new MyObject();
        mo.myToString();
        MyString2 ms2 = new MyString2();
        mi = ms2;
        mi.myToString();
        //PAY ATTENTION: toString is not explicitly claimed in MyInterface! both hashCode and toString via interface will use implmentation from class Object
        out.printf("%x:%s%n", mi.hashCode(), mi.toString());
        mo = ms2;
        mo.myToString();
        //Object:public final Class<?> getClass() will return final derived class's Class<?>
        out.printf("%s %b%n", mo.toString(), mo == ms2);
    }    
}
