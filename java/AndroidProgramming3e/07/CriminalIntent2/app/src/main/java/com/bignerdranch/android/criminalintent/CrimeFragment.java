package com.bignerdranch.android.criminalintent;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.AttributeSet;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;

public class CrimeFragment extends Fragment {
    private static Bundle bundle;
    private final static String TAG = "Criminal2";
    private Crime mCrime;
    private EditText mTitleField;
    private Button mDateButton;
    private CheckBox mSolvedCheckBox;
    private static final boolean sAttachedToContainer = false;
    private int hashValue(Object obj) {
        int hashCode = -1;
        if (obj != null)
            hashCode = obj.hashCode();
        return hashCode;
    }
    private void dump(String s, boolean stackShown) {
        String fmt = String.format("[%s]:%s", s, String.valueOf(this));
        if (stackShown) {
            Log.d(TAG, fmt, new Exception());
        } else {
            Log.d(TAG, fmt);
        }
    }
    private void dump(String s) {
        dump(s, true);
    }
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mCrime = new Crime();
        dump(String.format("create(%s)%n" + "(%x)[%b]%n" + "bundle=%s%n",
                String.valueOf(savedInstanceState),
                hashValue(savedInstanceState), bundle == savedInstanceState,
                String.valueOf(bundle)));
    }
    public CrimeFragment() {
        super();//can be ignored
        dump("cons");
    }
    @Override
    public void onSaveInstanceState(@NonNull Bundle outState) {
        super.onSaveInstanceState(outState);
        bundle = outState;
        dump(String.format("save(%s)%n(%x)%n!=null:%b%n", String.valueOf(outState), hashValue(outState), outState != null));
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        /*
        1. if @sAttachedToContainer==false, created/returned View is LinearLayout in fragment_crime.xml;
        2. if @sAttachedToContainer==true, LinearLayout ViewGroup and its own Views in fragment_crime.xml
         are created, but the returned View is @container, ie FrameLayout in activity_main.xml
        */
        View v = inflater.inflate(R.layout.fragment_crime, container, sAttachedToContainer);
        //findViewById must be after Views(All Views in XML) instantiation via inflate
        ViewGroup vg = v.findViewById(R.id.fragment_top);
        mTitleField = v.findViewById(R.id.crime_title);
        mTitleField.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {
                dump("before");
            }
            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {
                mCrime.setTitle(s.toString());
                dump("on");
            }
            @Override
            public void afterTextChanged(Editable s) {
                dump("After");
            }
        });
        mDateButton = v.findViewById(R.id.crime_date);
        mDateButton.setText(mCrime.getDate().toString());
        mDateButton.setEnabled(false);
        mSolvedCheckBox = v.findViewById(R.id.crime_solved);
        //build error if no "CompoundButton."
        mSolvedCheckBox.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                mCrime.setSolved(isChecked);
                dump(String.format("checked %b", isChecked));
            }
        });
        String s = String.format("onCreateView(,%s,%s)%n" + "(%x)[%b]%n" +
                        "R.id.fragment_top==onCreateView's return: %b%n" +
                        "root container==onCreateView's return: %b:%n%x",
                String.valueOf(container), String.valueOf(savedInstanceState),
                hashValue(savedInstanceState), bundle == savedInstanceState,
                vg == v, container==v, hashValue(v));
        dump(s);
        return v;
    }

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
        dump(String.format("attach(%s)", String.valueOf(context)));
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        dump(String.format("onActivityCreated(%s)%n" +
                "(%x)[%b]",
                String.valueOf(savedInstanceState),
                hashValue(savedInstanceState),
                bundle == savedInstanceState));
    }

    @Override
    public void onStart() {
        super.onStart();
        dump("start");
    }

    @Override
    public void onResume() {
        super.onResume();
        dump("resume");
    }

    @Override
    public void onPause() {
        super.onPause();
        dump("pause");
    }

    @Override
    public void onStop() {
        super.onStop();
        dump("stop");
    }
    @Override
    public void onDestroyView() {
        super.onDestroyView();
        dump("onDestroyView");
    }
    @Override
    public void onDestroy() {
        super.onDestroy();
        dump("destroy");
    }

    @Override
    public void onDetach() {
        super.onDetach();
        dump("detach");
    }
}
