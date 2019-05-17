/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package initialsequence;

import static java.lang.System.out;

class MyEnum {
    public MyEnum(String s, int i) {
        out.printf("MyEnum(\"%s\", %d)%n", s, i);
    }
    public MyEnum() {
        out.printf("MyEnum()%n");
    }
}

class MyClass {
    public MyClass() {
        out.printf("MyClass()%n");
    }
    public MyClass(String s) {
        out.printf("MyClass(\"%s\")%n", s);
    }
}

final class MyAction extends MyEnum {
    private MyClass myClass1 = new MyClass();
    private MyClass myClass2 = new MyClass("myClass2");
    private MyClass myClass3;
    //
    static int si0;
    static int si1 = 1;
    //
    static MyClass myClass_2= new MyClass("static myClass_2");
    static MyClass myClass_3;
    static {
        //cannot use si2, si3 directly, illegal forward reference! They're both 0 now!
        out.printf("+MyAction static block: si0/1/2/3: %d %d %d %d%n", si0, si1, MyAction.si2, MyAction.si3);
        si2 = 2;//Forward references are allowed on the left hand side of an assignment
        si3 = 3;//will be overridden later, but si2 will not be re-written for it has been initilized to 0
        out.printf("myClass_3/_2/_1/0:%s %s %s %s%n", String.valueOf(myClass_3), String.valueOf(myClass_2),
                String.valueOf(MyAction.myClass_1), String.valueOf(MyAction.myClass0));
        myClass0 = new MyClass("static myClass0 in static block");
        myClass_1 = new MyClass("static myClass_1 in static block");
        out.printf("myClass_3/_2/_1/0:%s %s %s %s%n", String.valueOf(myClass_3), String.valueOf(myClass_2),
                String.valueOf(MyAction.myClass_1), String.valueOf(MyAction.myClass0));
        out.printf("-MyAction static block: si0/1/2/3: %d %d %d %d%n", si0, si1, MyAction.si2, MyAction.si3);
        si4 = -1;
        out.printf("si4 %d%n", MyAction.si4);
    }
    {
        out.printf("+MyAction NON-static block: si0/1/2/3: %d %d %d %d%n", si0, si1, si2, si3);
        //myClass_3 = new MyClass("myClass3");
        //out.printf("-MyAction NON-static block: si0/1/2/3: %d %d %d %d%n", si0, si1, si2, si3);
        //static var/ref can be accessed from non static block
        out.printf("static myClass_3 %s%n", String.valueOf(myClass_3));
        out.printf("myClass4 %s%n", String.valueOf(this.myClass4));
        //out.println(myClass4);//illegal forward reference
        out.printf("si4 %d%n", si4);
    }
    static MyClass myClass0 = new MyClass("static myClass0");//will override the assignment in 1st static block
    static MyClass myClass_1;
    static int si2;
    static int si3 = -3;//will override the assignment in 1st static block
    static int si4 = 0;
    static {
        out.printf("2nd static block: si0/1/2/3: %d %d %d %d%n", si0, si1, si2, si3);
        out.printf("myClass_3/_2/_1/0:%s %s %s %s%n", String.valueOf(myClass_3), String.valueOf(myClass_2),
                String.valueOf(MyAction.myClass_1), String.valueOf(MyAction.myClass0));
        out.printf("si4 %d%n", si4);
    }
    //
    private MyClass myClass4 = new MyClass("myClass4");
    {
        out.printf("2nd non-static block, myClass4 %s%n", String.valueOf(myClass4));
    }
    //public final MyAction STOP;
    MyAction(String s, int i) {
        super(s, i);
        //STOP = new MyAction(s, i);
        out.printf("MyAction(\"%s\", %d)%n", s, i);
    }
    MyAction(String s) {
        super();
        //STOP = new MyAction(s);
        out.printf("MyAction(\"%s\")%n", s);
    }
    MyAction(int i) {//no explicite super
        //STOP = new MyAction(i);
        out.printf("MyAction(%d)%n", i);
    }
    {
        out.printf("last non-static block%n");
    }
}

class MyClass0 {
    static {
        out.printf("class MyClass0 loaded:");
    }
    public MyClass0() {
        out.printf("new MyClass0 instantiation will trigger loading MyClass0 iff it has not been loaded%n");
    }
}

class MyClass1 {
    static {
        out.printf("class MyClass1 loaded:");
    }
    public static void dummy() {
        out.printf("MyClass1 static func dummy() calling will trigger loading MyClass1 iff it has not been loaded%n");
    }
}
/**
 *
 * @author Administrator
 */
public class InitialSequence {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        testEnum();
    }
    static void testEnum() {
        out.printf("%ntestEnum 0:%n");
        MyAction ma0 = new MyAction("ma0", 0);
        out.printf("%ntestEnum 1:%n");
        MyAction ma1 = new MyAction("ma1");
        out.printf("%ntestEnum 2:%n");
        MyAction ma2 = new MyAction(2);
        /*
        2 ways to load some class iff this class hasnot been loaded:
        1/2. call its static func(no new its instantiation in the process)
        2/2. new one instantiation of this class
        */
        out.printf("How to trigger loading some class: new or call its static func:%n");
        MyClass1.dummy();
        new MyClass0();
    }    
}

