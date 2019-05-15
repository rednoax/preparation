package com.bignerdranch.android.criminalintent;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;

public abstract class SingleFragmentActivity extends AppCompatActivity {
    private final static String TAG = "CrimeSingleFragmentActivity";
    private void dump(String s) {
        String fmt = String.format("%s:%s", String.valueOf(this), s);
        Log.d(TAG, fmt);
    }
    public SingleFragmentActivity() {
        super();
        dump("CONS");
    }
    //protected is better than public
    protected abstract Fragment createFragment();
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_fragment);
        FragmentManager fm = getSupportFragmentManager();
        Fragment fragment = fm.findFragmentById(R.id.fragment_container);
        View view = findViewById(R.id.fragment_container);
        String fmt = String.format("onCreate: container(FrameLayout)[%s]%n" +
                "R.id.fragment_container: %s%n" +
                "LayoutInflater:%s",
                String.valueOf(fragment),
                String.valueOf(view),
                String.valueOf(LayoutInflater.from(this)));
        dump(fmt);
        if (fragment == null) {
            fragment = createFragment();
            fm.beginTransaction().add(R.id.fragment_container, fragment).commit();
        }
    }
}
