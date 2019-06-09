package com.example.broadcasttest;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.CountDownTimer;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.util.Log;

public class RegisterThread extends HandlerThread {
    private final static String TAG = "My2Thread";
    private final static String BROADCAST_ACTION_TEST = "com.example.broadcasttest.TEST";
    private Context mContext;
    private static final int MESSAGE_START_SERVICE = 0;
    private static final int MESSAGE_STOP_SERVICE = 1;
    private static final int MESSAGE_REG_BROADCAST = 2;
    private static final int MESSAGE_START_TASK = 3;
    private static final int MESSAGE_START_TIMER = 4;
    private static final int MESSAGE_START_INTENT_SERVICE = 5;
    private static final int MESSAGE_HEAVY_SERVICE = 6;
    private static final int MESSAGE_SEND_BROADCAST = 7;
    private Handler mHandler;
    private long INTERVAL_TIME = 2 * 1000;
    private long FINISH_TIME = 7 * 1000;
    private IntentFilter mIntentFilter;
    private MyReceiver mReceiver;
    private CountDownTimer taskCountDownTimer = new CountDownTimer(FINISH_TIME, INTERVAL_TIME) {
        /*
        1. onTick is called after time .start @0 @INTERVAL_TIME @@INTERVAL_TIME*2 until FINISH_TIME, the last time's onTick
        near the FINISH_TIME may not be called from the test.
        2. onFinish is called @FINISH_TIME
        */
        @Override
        public void onTick(long millisUntilFinished) {
            Log.d(TAG, "onTick");
        }

        @Override
        public void onFinish() {
            Log.d(TAG, "onFinish");
        }
    };
    RegisterThread() {
        super(TAG);
    }

    @Override
    public boolean quit() {
        if (mReceiver != null/*&& mContext != null*/) {
            mContext.unregisterReceiver(mReceiver);
        }
        String s = String.format("quit, BroadcastReceiver %s", String.valueOf(mReceiver));
        Log.d(TAG, s);
        return super.quit();
    }
    public void queueRegisterBroadcast(Context context) {
        mHandler.obtainMessage(MESSAGE_REG_BROADCAST, context).sendToTarget();
    }
    public void queueSendBroadcast(Context context) {
        mHandler.obtainMessage(MESSAGE_SEND_BROADCAST, context).sendToTarget();
    }
    public void queueStartService(Context context) {
        String s = String.format("begin a service:");
        Log.d(TAG, s);
        mHandler.obtainMessage(MESSAGE_START_SERVICE, context).sendToTarget();
    }
    public void queueStopService(Context context) {
        String s = String.format("stop a service:");
        Log.d(TAG, s);
        mHandler.obtainMessage(MESSAGE_STOP_SERVICE, context).sendToTarget();
    }

    public void queueTask(Context context) {
        String s = String.format("queueTask");
        Log.d(TAG, s);
        mHandler.obtainMessage(MESSAGE_START_TASK, context).sendToTarget();
    }

    public void queueTimer(Context context) {
        String s = String.format("queueTimer");
        Log.d(TAG, s);
        mHandler.obtainMessage(MESSAGE_START_TIMER, context).sendToTarget();
    }
    //press "exit" before the scheduled Runnable runs, main activity's onDestroy will quit HandlerThread and there will be no chance anymore for Runnable to run
    public void queueRunnable(Runnable runnable, long delayMs) {
        mHandler.postDelayed(runnable, delayMs);
        Log.d(TAG, "post a Runnable to HandlerThread");
    }
    public void queueStartIntentService(Context context, int id) {
        switch (id) {
            case R.id.start_intent_service0:
            case R.id.start_intent_service1:
                mHandler.obtainMessage(MESSAGE_START_INTENT_SERVICE, id, 0, context).sendToTarget();
                /*
                String s = String.format("%s:%d", String.valueOf(context), id);
                Log.d(TAG, s);
                */
                break;
        }
    }
    public void queueHeavyService(Context context, int id) {
        switch (id) {
            case R.id.start_heavy_service:
            case R.id.stop_heavy_service:
                mHandler.obtainMessage(MESSAGE_HEAVY_SERVICE, id, 0, context).sendToTarget();
                /*
                String s = String.format("%s:%d", String.valueOf(context), id);
                Log.d(TAG, s);
                */
                break;
        }
    }
    @Override
    protected void onLooperPrepared() {
        mHandler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                MainActivity context = (MainActivity) msg.obj;
                Log.d(TAG, String.format("%d", msg.what));
                int id = msg.arg1;
                switch (msg.what) {
                    case MESSAGE_START_SERVICE:
                        context.myStartService();
                        break;
                    case MESSAGE_STOP_SERVICE:
                        context.myStopService();
                        break;
                    case MESSAGE_REG_BROADCAST:
                        /*
                        Even BroadcastReceiver is registered by a non main thread, the BroadcastReceiver's
                        callback will still run on main thread
                         */
                        mIntentFilter = new IntentFilter();
                        mIntentFilter.addAction(BROADCAST_ACTION_TEST);
                        mReceiver = new MyReceiver();
                        context.registerReceiver(mReceiver, mIntentFilter);
                        Log.d(TAG, "registerReceiver");
                        mContext = context;
                        break;
                    case MESSAGE_SEND_BROADCAST:
                        /*
                        1. HandlerThread rather than main thread will send the broadcast but the Broadcast callback
                        will run on main thread AL!
                        2. If the BroadcastReceiver has not been registered, there is no chance for its callback being called at all
                        */
                         if (mReceiver != null) {
                            context.sendBroadcast(new Intent(BROADCAST_ACTION_TEST));
                        } else {
                            Log.d(TAG, "***BroadcastReceiver has not been registered");
                        }
                        break;
                    case MESSAGE_START_TASK:
                        /*
                        no matter "HOME"/"EXIT" at the Mobile's bottom is pressed, the doHeavy() still go on until it finishes/
                        if "EXIT" is triggered, Main Activity's onDestroy will quit this separated thread, ps -t PID shows the
                        threads will not be terminated until it finishes doHeavy();
                        */
                        doHeadvy();
                        break;
                    case MESSAGE_START_TIMER:
                        /*
                        1.it will run on main thread from the beginning even it is launched in a non main thread
                        2. no matter "HOME"/"EXIT" at the Mobile's bottom is pressed, the timer still go on until it finishes for main thread exists always.
                        if "EXIT" is triggered, Main Activity's onDestroy will quit this separated thread, ps -t PID shows the
                        threads will be terminated immediately no matter if the timer is still running;
                        */
                        taskCountDownTimer.start();
                        break;
                    case MESSAGE_START_INTENT_SERVICE:
                        Intent intentService;
                        Class<?> cls = My2IntentService1.class;
                        if (id == R.id.start_intent_service0) {
                            cls = My2IntentService0.class;
                        } else if (id == R.id.start_intent_service1) {
                            cls = My2IntentService1.class;
                        }
                        intentService = new Intent(context, cls);
                        context.startService(intentService);
                        String s = String.format("startService:%s:%d", cls, id);
                        Log.d(TAG, s);
                        break;
                    case MESSAGE_HEAVY_SERVICE:
                        Intent intent = new Intent(context, My2HeavyService.class);
                        if (id == R.id.start_heavy_service) {
                            context.startService(intent);
                        }
                        else if (id == R.id.stop_heavy_service) {
                            context.stopService(intent);
                        }
                        break;
                }
            }
        };
    }

    public static void doHeadvy() {
        int i = 0, j = 0;
        String s;
        for (j = 0; j < 30; j++) {
            for (i = 0; i < 100000000; i++) {
            }
            s = String.format("fin %d", j);
            Log.d(TAG, s);
        }
    }
}
