package com.bignerdranch.android.geoquiz;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.Button;
import android.view.*;//View, Gravity
import android.widget.Toast;

class MyOnClickListener implements View.OnClickListener {
    private MainActivity context;
    private int resId;
    private int duration = Toast.LENGTH_SHORT;
    @Override
    public void onClick(View view) {
        Toast toast = Toast.makeText(context, resId, duration);
        toast.setGravity(Gravity.TOP, 0, 0);
        toast.show();
    }
    MyOnClickListener(MainActivity context, int resId) {
        this.context = context;
        this.resId = resId;
    }
}

public class MainActivity extends AppCompatActivity {
    /* Cons sequence:
    static assignment and block
    Base class constructor
    non static assignment and block<---so it is like CPP's initializer list
    Derived class's constuctor that excludes super, no matter super is called explicitly or implicitly
    */
    private Button[] mTrueButton = new Button[2];
    private Button[] mFalseButton = new Button[2];
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        setTitle("SetInMainClass");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mTrueButton[0] = (Button)findViewById(R.id.true_button);
        mFalseButton[0] = (Button)findViewById(R.id.false_button);
        mTrueButton[0].setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(MainActivity.this,
                        R.string.correct_toast,
                        Toast.LENGTH_LONG).show();
            }
        });
        mFalseButton[0].setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(MainActivity.this, R.string.incorrect_toast, Toast.LENGTH_LONG).show();
            }
        });
        mTrueButton[1] = (Button)findViewById(R.id.true_button2);
        mTrueButton[1].setOnClickListener(new MyOnClickListener(this, R.string.correct_toast2));
        mFalseButton[1] = (Button)findViewById(R.id.false_button2);
        mFalseButton[1].setOnClickListener(new MyOnClickListener(this, R.string.incorrect_toast2));
        //android.support.constraint.ConstraintLayout p = findViewById(R.id.activity_main);
    }
}
