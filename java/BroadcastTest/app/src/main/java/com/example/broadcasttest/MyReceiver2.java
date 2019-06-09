package com.example.broadcasttest;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class MyReceiver2 extends BroadcastReceiver {
    private static final String TAG = "My2Receiver2";
    @Override
    public void onReceive(Context context, Intent intent) {//runs on main thread after dialing "*#*#911#*#*"
        // TODO: This method is called when the BroadcastReceiver is receiving
        // an Intent broadcast.
        String s = String.format("onReceiver(%s,%s)", String.valueOf(context), String.valueOf(intent));
        Log.d(TAG, s);
    }
}
