package com.example.broadcasttest;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

public class My2HeavyService extends Service {
    private static final String TAG = "My2HeavyService";
    public My2HeavyService() {//run on main thread
        Log.d(TAG, String.format("My2HeavyService():%s", String.valueOf(this)));
    }
    @Override
    public void onCreate() {//run on main thread
        super.onCreate();
        Log.d(TAG, "onCreate");
    }
/*
1. if U startService twice, no matter if the 1st startService's onStartCommand finishes, the 2nd onStartCommand's startID==2 and no cons/onCreate after 2nd startService for
there has been one instantiation for this Service:
2019-06-09 12:23:46.456 6327-6327/com.example.broadcasttest D/My2HeavyService: onStartCommand(Intent { cmp=com.example.broadcasttest/.My2HeavyService },0,2): 1
all onStartCommand's return is 1==START_STICKY
the 2nd onStartCommand will not run until the 1st onStartCommand finishes, for all onStartCommand of this Service can only run on main thread!
2. if U startService for the 1st time, and before its onStartCommand finishes, you press startService button several times, only the 2nd startService's onStartCommand
will run after the 1st onStartCommand finishes, the other startService will be ignored. It is likely there is ANR popping during the process.
3. if U startService for the 1st time and press stopService before its onStartCommand finishes;
After the old Service is destructed, U startService again and press stopService Button before its onStartCommand finishes:
2019-06-09 12:36:35.163 8436-8436/com.example.broadcasttest D/My2Activity: Thread started
2019-06-09 12:36:38.462 8436-8457/com.example.broadcasttest D/My2Thread: 6
2019-06-09 12:36:38.504 8436-8436/com.example.broadcasttest D/My2HeavyService: My2HeavyService():com.example.broadcasttest.My2HeavyService@282c41<---Service's cons can only runs on main thread
2019-06-09 12:36:38.505 8436-8436/com.example.broadcasttest D/My2HeavyService: onCreate<---Service's onCreate can only runs on main thread
2019-06-09 12:36:38.507 8436-8436/com.example.broadcasttest D/My2HeavyService: onStartCommand(Intent { cmp=com.example.broadcasttest/.My2HeavyService },0,1): 1<---Service's onStartCommand can only runs on main thread and its startId==1, return START_STICK AL
2019-06-09 12:36:39.265 8436-8436/com.example.broadcasttest D/My2Thread: fin 0
....
2019-06-09 12:37:00.866 8436-8436/com.example.broadcasttest D/My2Thread: fin 29
2019-06-09 12:37:00.882 8436-8457/com.example.broadcasttest D/My2Thread: 6
2019-06-09 12:37:00.904 8436-8436/com.example.broadcasttest D/My2HeavyService: onDestroy<---because stopService Button is pressed.the old Service instantiation is destructed!
2019-06-09 12:37:10.531 8436-8457/com.example.broadcasttest D/My2Thread: 6
2019-06-09 12:37:10.570 8436-8436/com.example.broadcasttest D/My2HeavyService: My2HeavyService():com.example.broadcasttest.My2HeavyService@cb6041f<---cons&onCreate will run during instantiation of a new Service
2019-06-09 12:37:10.574 8436-8436/com.example.broadcasttest D/My2HeavyService: onCreate
2019-06-09 12:37:10.576 8436-8436/com.example.broadcasttest D/My2HeavyService: onStartCommand(Intent { cmp=com.example.broadcasttest/.My2HeavyService },0,1): 1<---startId is 1 for this is a new Service instantiation
2019-06-09 12:37:11.262 8436-8436/com.example.broadcasttest D/My2Thread: fin 0
.....
2019-06-09 12:37:29.983 8436-8436/com.example.broadcasttest D/My2Thread: fin 29
2019-06-09 12:37:29.999 8436-8457/com.example.broadcasttest D/My2Thread: 6
2019-06-09 12:37:30.028 8436-8436/com.example.broadcasttest D/My2HeavyService: onDestroy<---because stopService Button is pressed.
*/
    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {//run on main thread
        int ret = super.onStartCommand(intent, flags, startId);
        String s = String.format("onStartCommand(%s,%x,%x): %d", intent.toString(), flags, startId, ret);
        Log.d(TAG, s);
        RegisterThread.doHeadvy();
        return ret;//1: START_STICKY
    }
    @Override
    public void onDestroy() {//run on main thread
        super.onDestroy();
        Log.d(TAG, "onDestroy");
    }
    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("Not yet implemented");
    }
}
