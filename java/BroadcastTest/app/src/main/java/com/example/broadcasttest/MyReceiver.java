package com.example.broadcasttest;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class MyReceiver extends BroadcastReceiver {
    private static final String TAG = "My2Receiver";

    @Override
    public void onReceive(Context context, Intent intent) {
        String s = String.format("%s Received: %s", String.valueOf(context), intent.getAction());
        Log.d(TAG, s);
    }
}