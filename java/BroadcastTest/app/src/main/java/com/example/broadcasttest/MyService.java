package com.example.broadcasttest;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;
/*Service Lifecycle
There are two reasons that a service can be run by the system. If someone calls Context.startService() then the system will retrieve the
service (creating it and calling its onCreate() method if needed) and then call its onStartCommand(Intent, int, int) method with the arguments
supplied by the client. The service will at this point continue running until Context.stopService() or stopSelf() is called. Note that multiple
calls to Context.startService() do not nest (though they do result in multiple corresponding calls to onStartCommand()), so no matter how many
times it is started a service will be stopped once Context.stopService() or stopSelf() is called; however, services can use their stopSelf(int)
method to ensure the service is not stopped until started intents have been processed.

For started services, there are two additional major modes of operation they can decide to run in, depending on the value they return from
onStartCommand(): START_STICKY is used for services that are explicitly started and stopped as needed, while START_NOT_STICKY or START_REDELIVER_INTENT
are used for services that should only remain running while processing any commands sent to them. See the linked documentation for more detail on the semantics.
*/
public class MyService extends Service {
    private static final String TAG = "My2Service";
    public MyService() {
        Log.d(TAG, String.format("MyService():%s", String.valueOf(this)));
    }

    @Override
    public void onCreate() {
        super.onCreate();
        Log.d(TAG, "onCreate");
    }
/*int onStartCommand (Intent intent, int flags, int startId)：
Called by the system every time a client explicitly starts the service by calling startService(Intent), providing the arguments it supplied and
a unique integer token representing the start request. Do not call this method directly.
Note that the system calls this on your service's main thread. A service's main thread is the same thread where UI operations take place for
Activities running in the same process. You should always avoid stalling the main thread's event loop. When doing long-running operations,
network calls, or heavy disk I/O, you should kick off a new thread, or use AsyncTask.
startId 	int: A unique integer representing this specific request to start. Use with stopSelfResult(int).
returns: ...
*/
    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        int ret = super.onStartCommand(intent, flags, startId);
        String s = String.format("onStartCommand(%s,%x,%x): %d", intent.toString(), flags, startId, ret);
        Log.d(TAG, s);
        return ret;//1: START_STICKY
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.d(TAG, "onDestroy");
    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("Not yet implemented");
    }
}
/*
void stopSelf(int startId): Old version of stopSelfResult(int) that doesn't return a result
stopSelfResult Added in API level 1
boolean stopSelfResult (int startId):<---plz note the following description conforms to both sticky and non sticky Service!!!
Stop the service if the most recent time it was started was startId. This is the same as calling stopService(Intent) for this particular service
but allows you to safely avoid stopping if there is a start request from a client that you haven't yet seen in onStart(Intent, int).
Be careful about ordering of your calls to this function.. If you call this function with the most-recently received ID before you have
called it for previously received IDs, the service will be immediately stopped anyway. If you may end up processing IDs out of order
(such as by dispatching them on separate threads), then you are responsible for stopping them in the same order you received them.
Parameters
startId 	int: The most recent start identifier received in onStart(Intent, int).
Returns
boolean 	Returns true if the startId matches the last start request and the service will be stopped, else false.

stopSelf is Service's method while stopService is Context's method
stopSelf Added in API level 1
void stopSelf ()
Stop the service, if it was previously started. This is the same as calling stopService(Intent) for this particular service.
*/