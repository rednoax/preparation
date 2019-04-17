package com.bignerdranch.android.geoquiz;

import android.app.Activity;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
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
    private static final String TAG = "GeoQuiz";
    private static int mIndex = 0;
    private static final int REQUEST_CODE_CHEAT = 0;
    private Button mCheatButton;
    private boolean mIsCheater;
    private int mShownTimes;
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
        if (savedInstanceState != null) {
            mCurrentIndex = savedInstanceState.getInt(KEY_INDEX);
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
                mIsCheater = false;
                updateText();
            }
        });
        mCheatButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //Intent intent = new Intent(MainActivity.this, CheatActivity.class);
                boolean answerIsTrue = mQuestionBank[mCurrentIndex].isAnswerTrue();
                Intent intent = CheatActivity.newIntent(MainActivity.this, answerIsTrue, mIndex);
                startActivityForResult(intent, REQUEST_CODE_CHEAT);
            }
        });
        updateText();
        String s = String.format("[%d]Create:%s %s", mIndex, String.valueOf(this), String.valueOf(getIntent()));
        Log.d(TAG, s);
    }
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        mIndex++;
        String s = String.format("%d: %d %d %s", mIndex, requestCode, resultCode, String.valueOf(data));
        Log.d(TAG, s);
        if (resultCode != Activity.RESULT_OK)
            return;
        if (requestCode == REQUEST_CODE_CHEAT && data != null) {
                mIsCheater = CheatActivity.wasAnswerShown(data);
                mShownTimes = CheatActivity.getShownTimes(data);
                s = String.format("%b %d", mIsCheater, mShownTimes);
                Log.d(TAG, s);
        }
    }
    private void checkAnswer(boolean userPressedTrue) {
        boolean answer = mQuestionBank[mCurrentIndex].isAnswerTrue();
        int messageResId;
        if (mIsCheater) {
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
    @Override
    protected void onSaveInstanceState(Bundle savedInstanceState) {
        super.onSaveInstanceState(savedInstanceState);
        Log.d(TAG, "save " + this);
        savedInstanceState.putInt(KEY_INDEX, mCurrentIndex);
    }
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
