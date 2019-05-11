/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package apv3c8emulator;
import static java.lang.System.*;
class Father {
    public int itemView = 1;
    private int itemView2 = 2;
    public void fatherGetActivity() {
        out.printf("fatherGetActivity()%n");
    }
    public void fatherGetActivity(int index) {
        out.printf("fatherGetActivity(%d)%n", index);        
    }    
}

class Child extends Father {
    public void childGetActivity() {
        //out.printf("childGetActivity %d%n", itemView2);//wrong for itemView2 is private in base class
        out.printf("childGetActivity%n");
    }
    /*
    1. super. can happen anywhere with any times in the function
    2. super.function: "super" here means direct father, and the functon must exist in the direct father, which can be
    defined by the direct father itself or derived from its ancestor. The ancestor can be direct or indirect, either is OK!
    */
    public void fatherGetActivity(int index) {
        out.printf("Child Override fatherGetActivity(%d) then call super: ", index);
        super.fatherGetActivity();
        super.fatherGetActivity(-1);
        //super.chileGetActivity();//wrong: cannot find symbol
    }
}

class GrandSon extends Child {
    public void fatherGetActivity() {
        out.printf("GrandSon Override fatherGetActivity()%n");
    }
    public void childGetActivity() {
        out.printf("GrandSon Override childGetActivity()%n");
    }
    public void fatherGetActivity(int index) {
        out.printf("GrandSon Override fatherGetActivity(%d)%n", index);
        super.fatherGetActivity();
        super.childGetActivity();
        super.fatherGetActivity(0);
    }
    public void test() {
        this.fatherGetActivity();
        this.childGetActivity();
        this.fatherGetActivity(1);
        //out.printf("itemView %d%n", itemView2);//wrong for itemView2 is private in Father
        out.printf("itemView %d%n", this.itemView);
    }
}

/*
RecycleView is just a namespace: To instantiate these 2 inner static class, 
no need to instantiate the outter class RecycleView.
*/
class RecyclerView {
    public abstract static class ViewHolder {
    }
    
    public abstract static class Adapter<VH extends ViewHolder> {
        public abstract VH onCreateViewHolder();
        public abstract void onBindViewHolder(VH holder, int position);
        public abstract int getItemCount();
    }
}
class Fragment {
    /*
    no matter which inner class calls getActivity:
    1, called in CrimeAdapter, which can ha only one instantiation
    2, called in CrimeViewHolder, which can have multiple instantiations
    "this" shown in getActivity() can be and only be the outter class "CrimeListFragment" 's this.
    */
    int getActivity() {
        int hashCode = this.hashCode();
        out.printf("[%s getActivity: %x]%n", String.valueOf(this), hashCode);
        return hashCode;
    }
}

class CrimeListFragment extends Fragment {
    private CrimeAdapter mAdapter;
    private int mPrivate = 1;
    public int mPublic = 2;
    protected int mProtected = 3;
    private static int sPrivate = 4;

    /*
    these 3 funcs(till the line marked "end") can be called in innter class ViewHolder;
    Even they are called in inner class, their "this" shown by printf can be and only
    be the outter class "CrimeListFragment" 's this.
    */
    private void privateFunc() {
        out.printf("privateFunc:%x%n", this.hashCode());
    }
    protected void protectedFunc() {
        out.printf("protectedFunc:%x%n", this.hashCode());
    }
    public void publicFunc() {
        out.printf("publicFunc:%x%n", this.hashCode());
    }
    //---end---
    private static void privateStaticFunc(){
        out.printf("privateStaticFunc%n");
    }
    public CrimeListFragment() {//outter class CrimeListFragment must be instantiared before its inner class instantiation
        out.printf("CrimeListFragment:");
        getActivity();
        onCreateView();
    }
    public void onCreateView() {
        updateUI();
    }
    public void updateUI() {
        mAdapter = new CrimeAdapter();//outter class can access its inner class(CrimeListFragment access CrimeAdapter)
        out.printf("Adapter %x updateUI:", mAdapter.hashCode());
        getActivity();
        for (int i = 0; i < 2; i++) {
            CrimeHolder ch = mAdapter.onCreateViewHolder();
            mAdapter.onBindViewHolder(ch, i);
        }
    }
    private class CrimeHolder extends RecyclerView.ViewHolder {
        CrimeHolder() {
            /*
            if using this.getActivity(), build error: cannot find symbol
            */
            out.printf("CrimeHolde %x:", this.hashCode());
            getActivity();
            out.printf("Adapter %x, mPrivate %x, mPublic %x, nProtected %x, sPrivate %x%n", 
                    mAdapter.hashCode(), mPrivate, mPublic, mProtected, sPrivate);
            privateFunc();
            publicFunc();
            //this.protectFunc();//wrong: cannot find symbol
            protectedFunc();//ok
            privateStaticFunc();
            mAdapter.getItemCount();            
        }
    }
    private class CrimeAdapter extends RecyclerView.Adapter<CrimeHolder> {
        @Override
        public CrimeHolder onCreateViewHolder() {
            return new CrimeHolder();//inner class can access each other: Adapter new ViewHolder, both of which are inner class
        }
        @Override
        public void onBindViewHolder(CrimeHolder holder, int position) {
            //the following this is CrimeAdapter that has only one instantiation in the whole program
            //and getActivity()'s this will be CrimeListFragment's this
            out.printf("onBindViewHolder(%x, %x):%x: %x%n", holder.hashCode(), position, this.hashCode(), getActivity());
        }
        @Override
        public int getItemCount() {
            out.printf("getItemCount:");
            getActivity();
            return 0;
        }
        
    }
}
/**
 *
 * @author Administrator
 */
public class APv3C8Emulator {

    /**
     * @param args the command line arguments
     */
    public static void superTest() {
        // TODO code application logic here
        GrandSon gs = new GrandSon();
        gs.test();
    }
    public static void fragmentTest() {
        new CrimeListFragment();
    }
    public static void main(String[] arg) {
        superTest();
        fragmentTest();
    }
}
