package com.example.broadcasttest;

import android.app.IntentService;
import android.content.Intent;
import android.os.CountDownTimer;
import android.util.Log;
/*
1. onCreate will run on main thread even this IntentService is started in HandlerThread.
   IntentService's source code show its onCreate will new its own HandlerThread then start it: but
   its onCreate has nothing to let this thread run!
   IntentService's onDestroy will run on main thread too even this IntentService is started from HandlerThread.

2. onHandleIntent will run a separated thread, which is neither main thread nor HandlerThread.
   onHandleIntent's thread will be different when each time this IntentService is started by startService

2.5 all timer's callback will run main thread all the time no matter if HandlerThread exists or not

3. when onHandleIntent has finished but timer has not finished. Exit the main Activity, "ps -t pid" shows HandlerThread
   is terminated immediately.(Main Activity's onDestroy=>quit() will terminate HandlerThread). But the unfinished
   timer will run on main threaduntil it finishes.
*/
public class My2IntentService1 extends IntentService {
    private static final String TAG = "My2IntentService1";
    private long INTERVAL_TIME = 1 * 1000;
    private long FINISH_TIME = 10 * 1000;
    public My2IntentService1() {
        super("My2IntentService1");
        Log.d(TAG, String.format("My2IntentService0():%s", String.valueOf(this)));
    }
    //if FINISH_TIME < INTERVAL_TIME, then onFinish will be called in FINISH_TIME ms, no onTick has chance to be called
    private CountDownTimer countDownTimer = new CountDownTimer(FINISH_TIME, INTERVAL_TIME) {
        @Override
        public void onTick(long millisUntilFinished) {
            Log.d(TAG, "onTick");
        }

        @Override
        public void onFinish() {
            Log.d(TAG, "onFinish");
        }
    };
    @Override
    public void onCreate() {
        super.onCreate();
        Log.d(TAG, "onCreate");
    }

    @Override
    protected void onHandleIntent(Intent intent) {
        String s = String.format("onHandleIntent(%s)", String.valueOf(intent));
        Log.d(TAG, s);
        countDownTimer.start();
        //RegisterThread.doHeadvy();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.d(TAG, "onDestroy");
    }
}
