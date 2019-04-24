/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package charsequence;

interface CharSequence {
    public abstract String myToString();
    //public void swim(){}
}
//if MyString class doesnot Override interface function, it must be marked "abstract"
class MyString implements CharSequence {//"public final class String", so U cannot "extends String"
    private String mString = "Default";
    @Override
    public String myToString() {
        String s = "MyString:" + mString;
        System.out.printf("[%s(%s)]%n", s, mString.toString());
        return s;
    }
}
class View implements CharSequence {
    @Override
    public String myToString() {
        System.out.println("View");
        return null;
    }
}
class TextView extends View {
    private CharSequence mText;
    public void setText(CharSequence s) {
        mText = s;
    }
    public CharSequence getText() {
        return mText;
    }
    @Override
    public String myToString() {
        String s = "TextView";
        System.out.printf("%s%n", s);
        return s;
    }
    public void test(CharSequence s) {
        setText(s);
        getText().myToString();
    }
}
/**
 *
 * @author Administrator
 */
public class MyCharSequence {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        MyString ms = new MyString();
        View v = new View();
        TextView tv = new TextView();      
        tv.test(ms);
        tv.test(v);
        tv.test(tv);
    }
    
}
