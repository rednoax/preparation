package com.example.broadcasttest;

import android.app.IntentService;
import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;
/*
1. onCreate will run on main thread even this IntentService is started in HandlerThread.
   IntentService's source code show its onCreate will new its own HandlerThread then start it: but
   its onCreate has nothing to let this thread run!
   IntentService's onDestroy will run on main thread too even this IntentService is started from HandlerThread.

2. onHandleIntent will run a separated thread, which is neither main thread nor HandlerThread.
   onHandleIntent's thread will be different when each time this IntentService is started by startService

3. when onHandleIntent has not finished. Exit the main Activity, "ps -t pid" shows HandlerThread
   is terminated immediately.(Main Activity's onDestroy=>quit() will terminate HandlerThread). But the unfinished
   onHandleIntent will run until it finishes, then the onHandleIntent's thread will disappeared
   from "ps -t pid"
*/
public class My2IntentService0 extends IntentService {
    private static final String TAG = "My2IntentService0";
    public My2IntentService0() {
        super("My2IntentService0");
        Log.d(TAG, String.format("My2IntentService0():%s", String.valueOf(this)));
    }
/*
startId is [1, ..., if the Service instantiation has been a new one, @startId AL is from 1.
return: START_NOT_STICKY(==2)
*/
    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {//run on main thread, see IntentService source
        int ret = super.onStartCommand(intent, flags, startId);
        String s = String.format("onStartCommand(%s, %d, %d): %d", String.valueOf(intent), flags, startId, ret);
        Log.d(TAG, s);
        return ret;
    }

    @Override
    public void onCreate() {//run on main thread
        super.onCreate();
        Log.d(TAG, "onCreate");
    }
/*
1. If U start this IntentService, then start IntentService once more when the former's doHeavy has not finished:
2019-06-08 19:29:29.963 28864-28882/com.example.broadcasttest D/My2Thread: startService:class com.example.broadcasttest.My2IntentService0:2131165313<---HandlerThread calls startService
2019-06-08 19:29:29.975 28864-28864/com.example.broadcasttest D/My2IntentService0: My2IntentService0():com.example.broadcasttest.My2IntentService0@891e399<---Service's cons runs on main thread
2019-06-08 19:29:29.979 28864-28864/com.example.broadcasttest D/My2IntentService0: onCreate<---Service's onCreate runs on main thread.onCreate will launch HandlerThread
2019-06-08 19:29:29.980 28864-28864/com.example.broadcasttest D/My2IntentService0: onStartCommand(Intent { cmp=com.example.broadcasttest/.My2IntentService0 }, 0, 1): 2<---runs on main thread and startId is 1, return 2 is START_NOT_STICKY. onStartCommand will allocate a Message with its Intent&startId, then add it to HandlerThread's MessageQueue
2019-06-08 19:29:29.983 28864-28985/com.example.broadcasttest D/My2IntentService0: onHandleIntent: Intent { cmp=com.example.broadcasttest/.My2IntentService0 }<---run on HandlerThread(launched in onCreate) rather than main thread
2019-06-08 19:29:30.773 28864-28985/com.example.broadcasttest D/My2Thread: fin 0<---onHandleIntent's doHeavy is running on HandlerThread and has not finished
2019-06-08 19:29:31.512 28864-28985/com.example.broadcasttest D/My2Thread: fin 1
2019-06-08 19:29:32.191 28864-28882/com.example.broadcasttest D/My2Thread: 5<----startService will be called even the former onHandleThread has not finished
2019-06-08 19:29:32.195 28864-28882/com.example.broadcasttest D/My2Thread: startService:class com.example.broadcasttest.My2IntentService0:2131165313<---ditto, please note there is no Service cons and onCreate for the Service class has one instantiation and it has not been stopped
2019-06-08 19:29:32.204 28864-28864/com.example.broadcasttest D/My2IntentService0: onStartCommand(Intent { cmp=com.example.broadcasttest/.My2IntentService0 }, 0, 2): 2<---onStartCommand will call with startService AL, the startId is the last value plus 1. onStartCommand will queue a Message with its arguments:Intent&startId to Service's HandlerThread
2019-06-08 19:29:32.275 28864-28985/com.example.broadcasttest D/My2Thread: fin 2
2019-06-08 19:29:33.032 28864-28985/com.example.broadcasttest D/My2Thread: fin 3
2019-06-08 19:29:33.774 28864-28985/com.example.broadcasttest D/My2Thread: fin 4
2019-06-08 19:29:34.512 28864-28985/com.example.broadcasttest D/My2Thread: fin 5
.....
2019-06-08 19:29:51.558 28864-28985/com.example.broadcasttest D/My2Thread: fin 28
2019-06-08 19:29:52.319 28864-28985/com.example.broadcasttest D/My2Thread: fin 29<--when lst queued Message by onHanleIntent has been finished,it will stopSelf(1), 1 is its startId, the current startId is 2, so the stopSelf(1) can not stop the Service, which means Service's onDestroy will not be called. The Service instantiation and HandlerThread(only quit if onDestroy runs) still exists
2019-06-08 19:29:52.321 28864-28985/com.example.broadcasttest D/My2IntentService0: onHandleIntent: Intent { cmp=com.example.broadcasttest/.My2IntentService0 }<--Service's HandlerThread begin to handle the 2nd Message added by 2nd startService
2019-06-08 19:29:52.998 28864-28985/com.example.broadcasttest D/My2Thread: fin 0
2019-06-08 19:29:53.673 28864-28985/com.example.broadcasttest D/My2Thread: fin 1
2019-06-08 19:29:54.349 28864-28985/com.example.broadcasttest D/My2Thread: fin 2
2019-06-08 19:29:55.014 28864-28985/com.example.broadcasttest D/My2Thread: fin 3
......
2019-06-08 19:30:11.359 28864-28985/com.example.broadcasttest D/My2Thread: fin 28
2019-06-08 19:30:12.003 28864-28985/com.example.broadcasttest D/My2Thread: fin 29<---2nd queued Message is finished & handleMessage will call stopSelf(2) that will successfully stop the Service for the most recent time it was started was startId==2
2019-06-08 19:30:12.006 28864-28864/com.example.broadcasttest D/My2IntentService0: onDestroy<--Once the Service is successfully stopped, its onDestroy is called, which will quit the Service's HandlerThread, when the Service is started the 3rd time:its cons/onCreate will be called for the old Service instantiation has been freed. And the new Service's HandlerThread is a new thread with different tid as mine.
...
2019-06-08 19:59:27.367 28864-28882/com.example.broadcasttest D/My2Thread: 5
2019-06-08 19:59:27.377 28864-28882/com.example.broadcasttest D/My2Thread: startService:class com.example.broadcasttest.My2IntentService0:2131165313
2019-06-08 19:59:27.378 28864-28864/com.example.broadcasttest D/My2IntentService0: My2IntentService0():com.example.broadcasttest.My2IntentService0@98b4610<---new Service's cons, runs on main thread AL
2019-06-08 19:59:27.381 28864-28864/com.example.broadcasttest D/My2IntentService0: onCreate<---new Service's onCreate, which AL runs on main thread, and it will launch a new HandlerThread whose tid cannot be the same as last Service's instantiation's HandlerThread's tid
2019-06-08 19:59:27.383 28864-28864/com.example.broadcasttest D/My2IntentService0: onStartCommand(Intent { cmp=com.example.broadcasttest/.My2IntentService0 }, 0, 1): 2<---new Service's onStartCommand use startId 1 for this Service instantiation is a new one whose startid starts from 1, 2,...
2019-06-08 19:59:27.384 28864-29774/com.example.broadcasttest D/My2IntentService0: onHandleIntent: Intent { cmp=com.example.broadcasttest/.My2IntentService0 }
2019-06-08 19:59:28.057 28864-29774/com.example.broadcasttest D/My2Thread: fin 0
2019-06-08 19:59:28.707 28864-29774/com.example.broadcasttest D/My2Thread: fin 1
2. if U startService only once, but exit main Activity before Service's onHandlerIntent finishes doHeavy():
the Service has no UI and it is not stopped when U exit main Activity, so its onHandlerIntent will not be effected by press "exit" button at the bottom.
The doHeavy will go on until it finishes, then Service will be stopped=>onDestroy: quit HandlerThread, then the Service is destructed at last.
*/
    @Override
    protected void onHandleIntent(Intent intent) {//run in HandlerThread created in onCreate
        String s = String.format("onHandleIntent: %s", String.valueOf(intent));
        Log.d(TAG, s);
        RegisterThread.doHeadvy();
    }

    @Override
    public void onDestroy() {//run in main thread
        super.onDestroy();
        Log.d(TAG, "onDestroy");
    }
}
