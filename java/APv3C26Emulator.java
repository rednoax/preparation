/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package apv3c26emulator;
import java.util.*;
import java.util.function.Consumer;
import static java.lang.System.out;

class ThumbnailDownloader<T> {
    private int mInt = -1;
    private static int sInt = -1;
    private List<Runnable> list = new ArrayList<>();
    public void setInt(int i) {
        out.printf("setInt:%d=>%d%n", mInt, i);
        mInt = i;
        sInt = i;
    }
    public void incInt() {
        mInt++;
        sInt++;
        out.printf("incInt:%d=>%d%n", mInt - 1, mInt);
    }
    public static UUID randomUUID() {
        UUID uuid = UUID.randomUUID();
        out.printf("%s%n", uuid.toString());
        return uuid;
    }
    private void post(Runnable r) {
        list.add(r);
    }
    public void forEach(Consumer<Runnable> action) {
        list.forEach(action);
    }
    public void handleRequest(T target) {
        UUID uuid = ThumbnailDownloader.randomUUID();
        out.printf("handleRequest(%s): stack var:%s%n", target, uuid);
        post(new Runnable() {
            /*
            here is in a non static anonymous inner class defined in an instance func of outter class(if it
            is not annonymnous but a named inner class, the followings conclusion should be still right):
            stack vars is snapshot when running handleRequest, while private instance/static members are
            the value when run() is run.
            */
            @Override
            public void run() {
                out.printf("sInt: %d, mInt:%d\t" +
                        "target:%s\t" +
                        "stack var:%s%n", sInt, mInt, target, uuid);
            }
        });
    }
}
/**
 *
 * @author Administrator
 */
public class APv3C26Emulator {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        ThumbnailDownloader<UUID> td = new ThumbnailDownloader<>();
        td.handleRequest(ThumbnailDownloader.randomUUID());//m/sInt==-1 durint its post but it will not effect run()'s m/sInt value that is the run time value when run() runs
        td.setInt(0);
        td.handleRequest(ThumbnailDownloader.randomUUID());//m/sInt==0 durint its post but it will not effect run()'s m/sInt value that is the run time value when run() runs
        td.setInt(1);
        out.printf("forEach start:%n");
        td.forEach(new Consumer<Runnable>() {//m/sInt==1 when 1st arriving here
            @Override
            public void accept(Runnable r) {
                r.run();
                td.incInt();
            }
        });
    }
 }
