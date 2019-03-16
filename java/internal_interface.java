/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package cc.openhome;

final class ViewTreeObserver {
    /*
    https://beginnersbook.com/2016/03/nested-or-inner-interfaces-in-java/
    An interface i.e. declared inside the interface or class, is known as nested
    interface. 
    1. It is static by default.
    2. the main purpose of using them is to resolve the namespace by grouping related
       interfaces (or related interface and class) together.
    */
    interface onPreDrawListener {
        public boolean onPreDraw();//public abstract can be all ignored, or ignoring one of the two
    }
}
class TextView implements ViewTreeObserver.onPreDrawListener {
    @Override
    public boolean onPreDraw() {
        System.out.printf("onPreDraw%n");
        return true;
    }
}
/**
 *
 * @author Administrator
 */
public class internal_interface {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        TextView textView = new TextView();
        textView.onPreDraw();
        // TODO code application logic here
    }
    
}
