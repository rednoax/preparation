package com.example.broadcasttest;

import android.content.Intent;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
/*
A10:/ # ps | grep broadcast
u0_a100   21091 353   846060 47476 SyS_epoll_ a600f508 S com.example.broadcasttest
A10:/ # ps -t 21091
USER      PID   PPID  VSIZE  RSS   WCHAN            PC  NAME
u0_a100   21091 353   846060 47476 SyS_epoll_ a600f508 S com.example.broadcasttest
u0_a100   21096 21091 846060 47476 futex_wait a5fde6dc S Jit thread pool
u0_a100   21097 21091 846060 47476 do_sigtime a600f874 S Signal Catcher
u0_a100   21098 21091 846060 47476 poll_sched a600f74c S JDWP
u0_a100   21099 21091 846060 47476 futex_wait a5fde6dc S ReferenceQueueD
u0_a100   21100 21091 846060 47476 futex_wait a5fde6dc S FinalizerDaemon
u0_a100   21101 21091 846060 47476 futex_wait a5fde6dc S FinalizerWatchd
u0_a100   21102 21091 846060 47476 futex_wait a5fde6dc S HeapTaskDaemon
u0_a100   21103 21091 846060 47476 binder_thr a600f64c S Binder:21091_1
u0_a100   21104 21091 846060 47476 binder_thr a600f64c S Binder:21091_2
u0_a100   21106 21091 846060 47476 futex_wait a5fde6dc S Profile Saver
u0_a100   21108 21091 846060 47476 __skb_recv a600f468 S Thread-3
u0_a100   21109 21091 846060 47476 SyS_epoll_ a600f508 S My2Thread<----
u0_a100   21110 21091 846060 47476 SyS_epoll_ a600f508 S RenderThread
u0_a100   21111 21091 846060 47476 futex_wait a5fde6dc S GL updater
u0_a100   21113 21091 846060 47476 futex_wait a5fde6dc S hwuiTask1
u0_a100   21114 21091 846060 47476 futex_wait a5fde6dc S hwuiTask2
*/
public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    private static final String TAG = "My2Activity";
    private Handler mHanler;
    private RegisterThread mThread;
    private Button mStartServiceButton;
    private Button mStopServiceButton;
    private Button mStartHeavyService;
    private Button mStopHeavyService;
    private Button[] mSendBroadcastButton = new Button[2];
    private Button mStartTask;
    private Button mStartTimer;
    private Button mPost;
    private Button mPostToThread;
    private Button[] mStartIntentService = new Button[2];
    public void myStartService() {
        Intent startIntent = new Intent(this, MyService.class);
/*
Any Service's child class can has only 1 instantiation in a specified PID.
startService description:
1. If this service is not already running, it will be instantiated and started (creating a process for it if needed);
cons, onCreate and onStartCommand(Intent { cmp=com.example.broadcasttest/.MyService },0,1): 1
startId is from 1, the returned value is START_STICKY
2. if this service is running then it remains running. No cons/onCreate but onStartCommand(,0,startId) whose startId is last value plus 1:
onStart(Intent { cmp=com.example.broadcasttest/.MyService },0,2): 1
startId is last value plus 1, the returned value is START_STICKY always!
3. Every call to this method will result in a corresponding call to the target service's onStartCommand(Intent, int, int) method, with
the intent given here. This provides a convenient way to submit jobs to a service without having to bind and call on to its interface.
4. Note that calls to startService() are not nesting: no matter how many times you call startService(), a single call to stopService(Intent) will stop it.
to stop a Service means its onDestroy will be called and the Service 's instantiation in PID will be destructed after onDestroy!
After Service is stopped, startService will call cons/onCreate/onStartCommand(Intent,,1), Intent is that of startService and the startId is from 1 again!
5, Using startService() overrides the default service lifetime that is managed by bindService(Intent, ServiceConnection, int):
   it requires the service to remain running until stopService(Intent) is called, regardless of whether any clients are connected to it.
6. Returns: If the service is being started or is already running, the ComponentName of the actual service that was started is returned;
  else if the service does not exist null is returned.
*/
        this.startService(startIntent);
        Log.d(TAG, "myStartService");
    }
    public void myStopService() {
        Intent stopIntent = new Intent(this, MyService.class);
/*stopService Added in API level 1
boolean stopService (Intent service)
Request that a given application service be stopped. If the service is not running, nothing happens.
Otherwise it is stopped. Note that calls to startService() are not counted -- this stops the service
no matter how many times it was started.
*/
        this.stopService(stopIntent);
        Log.d(TAG, "myStopService");
    }
    @Override
    public void onClick(View v) {
        int id = v.getId();
        switch (id) {
            case R.id.start_service:
                mThread.queueStartService(this);
                break;
            case R.id.stop_service:
                mThread.queueStopService(this);
                break;
            case R.id.reg_broadcast:
                mThread.queueRegisterBroadcast(this);
                break;
            case R.id.send_broadcast:
                mThread.queueSendBroadcast(this);
                break;
            case R.id.start_task:
                mThread.queueTask(this);
                break;
            case R.id.start_timer:
                mThread.queueTimer(this);
                break;
            case R.id.post_to_main:
                //press "exit" before the scheduled Runnable runs, main thread still exists so the Runnable can run normally
                mHanler.postDelayed(new Runnable() {
/*
no matter "HOME"/"EXIT" at the Mobile's bottom is pressed after posting, the func run() still get called after delayed ms!
If EXIT main Activity before run get started, the HandlerThread will be terminated. But the Runnable is on Main thread, which
still exists after "EXIT" is pressed. So the posted Runnable will run normally.
*/
                    @Override
                    public void run() {
                        Log.d(TAG, "runned in main thread");
                    }
                }, 5 * 1000);
                Log.d(TAG, "Post");
                break;
            case R.id.post_to_thread:
                mThread.queueRunnable(new Runnable() {
/*
1. If the thread exit via Main Activity's exit, whose onDestroy will quit the thread(ps -t pid shows it immediately is terminated) and
the delayed Runnable to be run on the HandlerThread will be canceled for the thread has exit:
2019-06-07 14:13:01.035 31825-31825/com.example.broadcasttest D/My2Thread: post a Runnable<--Main thread post a Runnable
2019-06-07 14:13:06.041 31825-31847/com.example.broadcasttest D/My2Activity: runned in thread?<--the HandlerThread run the Main's Runnable
2019-06-07 14:13:11.015 31825-31825/com.example.broadcasttest D/My2Thread: post a Runnable<--Main thread post a Runnable
2019-06-07 14:13:12.580 31825-31825/com.example.broadcasttest D/My2Thread: quit<--HandlerThread is terminated before its handling MessageQueue, so no chance for the following run() to run
2. If you use HOME to pause Main Activity, there is still chance for HanlderThread to run this Runnable, for HandlerThread has not exit.
The test result conforms to the conclusion above
*/
                    @Override
                    public void run() {
                        Log.d(TAG, "runned in thread?");
                    }
                }, 5*1000);
                break;
            case R.id.start_intent_service0:
            case R.id.start_intent_service1:
                mThread.queueStartIntentService(this, id);
                break;
            case R.id.start_heavy_service:
            case R.id.stop_heavy_service:
                mThread.queueHeavyService(this, id);
                break;
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mHanler = new Handler();
        mThread = new RegisterThread();
        mThread.start();
        if (mThread.getLooper() != null) {
            Log.d(TAG, "Thread started");
        } else {
            Log.d(TAG, "***fail to launch thread");
        }
        mStartServiceButton = findViewById(R.id.start_service);
        mStopServiceButton = findViewById(R.id.stop_service);
        mStartTask = findViewById(R.id.start_task);
        mStartServiceButton.setOnClickListener(this);
        mStopServiceButton.setOnClickListener(this);
        mStartTask.setOnClickListener(this);
        //
        mStartTimer = findViewById(R.id.start_timer);
        mStartTimer.setOnClickListener(this);
        mPost = findViewById(R.id.post_to_main);
        mPost.setOnClickListener(this);
        mPostToThread = findViewById(R.id.post_to_thread);
        mPostToThread.setOnClickListener(this);
        mStartIntentService[0] = findViewById(R.id.start_intent_service0);
        mStartIntentService[0].setOnClickListener(this);
        mStartIntentService[1] = findViewById(R.id.start_intent_service1);
        mStartIntentService[1].setOnClickListener(this);
        //
        mStartHeavyService = findViewById(R.id.start_heavy_service);
        mStartHeavyService.setOnClickListener(this);
        mStopHeavyService = findViewById(R.id.stop_heavy_service);
        mStopHeavyService.setOnClickListener(this);
        //
        mSendBroadcastButton[0] = findViewById(R.id.reg_broadcast);
        mSendBroadcastButton[0].setOnClickListener(this);
        mSendBroadcastButton[1] = findViewById(R.id.send_broadcast);
        mSendBroadcastButton[1].setOnClickListener(this);
    }
    @Override
    public void onDestroy() {
        super.onDestroy();
        mThread.quit();
    }
}
