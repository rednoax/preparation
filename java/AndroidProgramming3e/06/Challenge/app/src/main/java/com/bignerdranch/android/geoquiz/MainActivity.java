package com.bignerdranch.android.geoquiz;

import android.app.Activity;
import android.content.Intent;
import android.os.Build;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.ViewAnimationUtils;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {
    private Button mTrueButton;
    private Button mFalseButton;
    private Button mNextButton;
    private TextView mQuestionTextView;
    private Question[] mQuestionBank;
    private int mCurrentIndex = 0;
    private static final String KEY_INDEX = "index";
    private static final String KEY_CHEATER = "cheater";
    private static final String KEY_CHEAT_TIMES = "cheat_times";
    private static final String TAG = "GeoQuiz";
    private static int mIndex = 0;
    private static final int REQUEST_CODE_CHEAT = 0;
    private Button mCheatButton;
    private boolean[] mIsCheater;
    private int mShownTimes;
    private static Bundle sSaved;
    private int mCheatTimes = 3;//cannot be static:)
    static {
        Log.d(TAG, "######MainActivity static block called only once");
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mTrueButton = (Button)findViewById(R.id.true_button);
        mFalseButton = (Button)findViewById(R.id.false_button);
        mNextButton = (Button)findViewById(R.id.next_button);
        mQuestionTextView = (TextView)findViewById(R.id.question_text_view);
        mCheatButton = (Button)findViewById(R.id.cheat_button);
        //
        mQuestionBank = new Question[] {
                new Question(R.string.question_australia, true),
                new Question(R.string.question_oceans, true),
                new Question(R.string.question_mideast, false),
                new Question(R.string.question_africa, false),
                new Question(R.string.question_americas, true),
                new Question(R.string.question_asia, true),
        };
        mIsCheater = new boolean[mQuestionBank.length];
        String info = "(";
        if (savedInstanceState != null) {
            mCurrentIndex = savedInstanceState.getInt(KEY_INDEX);
            mIsCheater = savedInstanceState.getBooleanArray(KEY_CHEATER);
            if (savedInstanceState == sSaved)
                info += "sSaved";
            //mIsCheater =savedInstanceState.getBoolean(KEY_CHEATER, false);
            mCheatTimes = savedInstanceState.getInt(KEY_CHEAT_TIMES);
        }
        //
        mTrueButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                checkAnswer(true);
            }
        });
        mFalseButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                checkAnswer(false);
            }
        });
        mNextButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mCurrentIndex = (mCurrentIndex + 1) % mQuestionBank.length;
                //mIsCheater = false;
                updateText();
            }
        });
        setCheatButtonText();
        mCheatButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //Intent intent = new Intent(MainActivity.this, CheatActivity.class);
                boolean answerIsTrue = mQuestionBank[mCurrentIndex].isAnswerTrue();
                Intent intent = CheatActivity.newIntent(MainActivity.this, answerIsTrue, mIndex);
                startActivityForResult(intent, REQUEST_CODE_CHEAT);
                //setCheatButtonText();
            }
        });
        updateText();
        String s = String.format("[%d]Create%s%s:%s %s", mIndex, info, String.valueOf(savedInstanceState),
                String.valueOf(this), String.valueOf(getIntent()));
        Log.d(TAG, s);
    }
    private void setCheatButtonText() {
        String cheatButtonString = String.format("%s %d", "Cheat", mCheatTimes);
        mCheatButton.setText(cheatButtonString);
        if (mCheatTimes == 0)
            mCheatButton.setEnabled(false);
    }
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        String s;
        do {
            mIndex++;
            s = String.format("%d: %d %d %s", mIndex, requestCode, resultCode, String.valueOf(data));
            if (resultCode != Activity.RESULT_OK)
                break;
            if (requestCode == REQUEST_CODE_CHEAT && data != null) {
                mIsCheater[mCurrentIndex] = CheatActivity.wasAnswerShown(data);
                mShownTimes = CheatActivity.getShownTimes(data);
                s += String.format(": %b %d", mIsCheater, mShownTimes);
                //Log.d(TAG, s);
                if (mCheatTimes > 0) {
                    mCheatTimes--;
                    setCheatButtonText();
                }
            }
        } while (false);
        Log.d(TAG, s);
    }
    private void checkAnswer(boolean userPressedTrue) {
        boolean answer = mQuestionBank[mCurrentIndex].isAnswerTrue();
        int messageResId;
        if (mIsCheater[mCurrentIndex]) {
            messageResId = R.string.judgment_toast;
        } else if (answer == userPressedTrue)
            messageResId = R.string.correct_toast;
        else
            messageResId = R.string.incorrect_toast;
        Toast.makeText(this, messageResId, Toast.LENGTH_SHORT).show();
    }
    private void updateText() {
        mQuestionTextView.setText(mQuestionBank[mCurrentIndex].getTextResId());
    }
    //
    @Override
    protected void onSaveInstanceState(Bundle savedInstanceState) {
        super.onSaveInstanceState(savedInstanceState);
        Log.d(TAG, "save " + this);
        savedInstanceState.putInt(KEY_INDEX, mCurrentIndex);
        savedInstanceState.putBooleanArray(KEY_CHEATER, mIsCheater);//here mIsCheater can be false
        savedInstanceState.putInt(KEY_CHEAT_TIMES, mCheatTimes);
        sSaved = savedInstanceState;
    }
    //
    @Override
    protected void onStart() {
        super.onStart();
        String s = String.format("Start:%s", String.valueOf(this));
        Log.d(TAG, s);
    }
    @Override
    protected void onResume() {
        super.onResume();
        String s = String.format("Resume:%s", String.valueOf(this));
        Log.d(TAG, s);
        /*
        If the following CheatActivity's static func calling from MainActivity is opened, it will trigger loading class CheatActivity iff
        CheatActivity class has not been loaded for this PID.Then U will see CheatActivity is loaded(its static block runs) even "Cheat"
        button has not been clicked:
2019-05-16 21:32:02.313 5781-5781/com.bignerdranch.android.geoquiz D/GeoQuiz: Resume:com.bignerdranch.android.geoquiz.MainActivity@76749f4
2019-05-16 21:32:02.318 5781-5781/com.bignerdranch.android.geoquiz D/GeoQuiz***: ######static block called only once when CheatActivity instance is 1st geneated
2019-05-16 21:32:02.318 5781-5781/com.bignerdranch.android.geoquiz D/GeoQuiz***: run dummy
        */
        //CheatActivity.dummy();
    }
    @Override
    protected void onPause() {
        super.onPause();
        String s = String.format("Pause:%s", String.valueOf(this));
        Log.d(TAG, s);
    }
    @Override
    protected void onStop() {
        super.onStop();
        String s = String.format("Stop:%s", String.valueOf(this));
        Log.d(TAG, s);
    }
    @Override
    protected void onDestroy() {
        super.onDestroy();
        String s = String.format("Destroy:%s", String.valueOf(this));
        Log.d(TAG, s);
        mIndex++;
    }
    MainActivity() {
        super();
        String s = String.format("CONS:%s", String.valueOf(this));
        Log.d(TAG, s);
    }
}
