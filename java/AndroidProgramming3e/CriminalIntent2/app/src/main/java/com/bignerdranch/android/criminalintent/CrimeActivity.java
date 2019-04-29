package com.bignerdranch.android.criminalintent;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.ViewGroup;

public class CrimeActivity extends AppCompatActivity {
    private static Bundle bundle;
    private static final String TAG = "Criminal";
    private void dump(String s) {
        String fmt = String.format("%s:%s", s, String.valueOf(this));
        Log.d(TAG, fmt);
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        dump("After super");
        setContentView(R.layout.activity_main);
        //dump("After set");
        ViewGroup fl = findViewById(R.id.fragment_container);
        FragmentManager fm = getSupportFragmentManager();
        //int id = R.id.fragment_container;
        int id = R.layout.fragment_crime;//OK!then CrimeFragment's onCreateView's 2nd arg is still
        Fragment fragment = fm.findFragmentById(id);
        String s = String.format("Create(%s)[%b]%n"
                        + "R.id.fragment_container: %s%n"
                        + "%s==%s?%n",
                String.valueOf(savedInstanceState), bundle == savedInstanceState,
                String.valueOf(fl),
                String.valueOf(fm.findFragmentById(R.id.fragment_container)), String.valueOf(fm.findFragmentById(R.layout.fragment_crime)));//Lint error!!!
        if (fragment == null) {
            fragment = new CrimeFragment();
            fm.beginTransaction().add(R.id.fragment_container, fragment).commit();
        }
        dump(s);
    }
    //
    @Override
    protected void onSaveInstanceState(Bundle savedInstanceState) {
        bundle = savedInstanceState;
        super.onSaveInstanceState(savedInstanceState);
        dump(String.format("save(%s)", String.valueOf(savedInstanceState)));
    }
    //
    public CrimeActivity() {
        //super();//can be ignored
        dump("CONS");
    }
    @Override
    protected void onStart() {
        super.onStart();
        dump("Start");
    }
    @Override
    protected void onResume() {
        super.onResume();
        dump("Resume");
    }
    @Override
    protected void onPause() {
        super.onPause();
        dump("Pause");
    }
    @Override
    protected void onStop() {
        super.onStop();
        dump("Stop");
    }
    @Override
    protected void onDestroy() {
        super.onDestroy();
        dump("Destroy");
    }
}
