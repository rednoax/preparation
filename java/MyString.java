/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javaapplication5;
import static java.lang.System.*;
import java.util.Date;

interface MyInterface {
    String myToString();
}

class MyObject {
    String myToString() {
        String s = "MyObject";
        out.println(s);
        return s;
    }
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
        Date date = new Date();
        out.printf("%s%n", date.toString());
        MyString ms = new MyString();
        MyInterface mi = ms;//no cast is needed since big assigned to small
        mi.myToString();
        MyObject mo = ms;//no cast is needed since big assigned to small
        mo.myToString();
        ms = (MyString)mo;//cast is must since small assigned to big
        ms.myToString();
        mo = new MyObject();
        mo.myToString();
    }    
}
