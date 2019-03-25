package com.bignerdranch.android.geoquiz;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {
    private final static String TAG = "QuizActivity";
    private final static String[] KEY_INDEX = {
            "index",
            "numberOfCorrect",
    };
    private Button mTrueButton;
    private Button mFalseButton;
    private Button mNextButton;
    private TextView mQuestionTextView;
    private int mNumberOfCorrectAnswers = 0;
    private Question[] mQuestionBank = {
            new Question(R.string.question_australia, true),
            new Question(R.string.question_oceans, true),
            new Question(R.string.question_mideast, false),
            new Question(R.string.question_africa, false),
            new Question(R.string.question_americas, true),
            new Question(R.string.question_asia, true),
    };
    private int mCurrentIndex = 0;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        if (savedInstanceState != null) {
            mCurrentIndex = savedInstanceState.getInt(KEY_INDEX[0], 0);//must before updateQuestion
            //mNumberOfCorrectAnswers = savedInstanceState.getInt(KEY_INDEX[1], 0);
        }
        mTrueButton = (Button)findViewById(R.id.true_button);
        mTrueButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                setAnswerButtons(false);
                checkAnswer(true);
            }
        });
        mFalseButton = (Button)findViewById(R.id.false_button);
        mFalseButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                setAnswerButtons(false);
                checkAnswer(false);
            }
        });
        mNextButton = (Button)findViewById(R.id.next_button);
        mNextButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mCurrentIndex = (mCurrentIndex + 1) % mQuestionBank.length;
                if (mCurrentIndex == 0) {
                    mNextButton.setEnabled(false);
                    setAnswerButtons(false);
                    displayScore();
                } else {
                    updateQuestion();
                    setAnswerButtons(true);
                    if (mCurrentIndex == mQuestionBank.length - 1) {
                        mNextButton.setText(R.string.check_score);
                    }
                }
            }
        });
        mQuestionTextView = (TextView)findViewById(R.id.question_text_view);
        updateQuestion();
        Log.d(TAG, "onCreate:" + this);//Log.d will display PID-TId
    }

    private void displayScore() {
        int score = 100 * mNumberOfCorrectAnswers / mQuestionBank.length;
        String msg = "Your score is " + score;
        mQuestionTextView.setText(msg);
        Toast.makeText(this, msg, Toast.LENGTH_LONG).show();
    }
    private void checkAnswer(boolean userpressedTrue) {
        boolean questionTrue = mQuestionBank[mCurrentIndex].isAnswerTrue();
        int messageResId;
        if (questionTrue == userpressedTrue) {
            messageResId = R.string.correct_toast;
            mNumberOfCorrectAnswers += 1;
        } else {
            messageResId = R.string.incorrect_toast;
        }
        Toast.makeText(this, messageResId, Toast.LENGTH_SHORT)
            .show();
    }

    private void setAnswerButtons(boolean enabled) {
        mTrueButton.setEnabled(enabled);
        mFalseButton.setEnabled(enabled);
    }
    private void updateQuestion() {
        int question = mQuestionBank[mCurrentIndex].getTextResId();
        mQuestionTextView.setText(question);
    }
    @Override
    protected void onSaveInstanceState(Bundle bundle) {
        super.onSaveInstanceState(bundle);
        Log.d(TAG, "onSaveInstanceState:" + this);
        bundle.putInt(KEY_INDEX[0], mCurrentIndex);
        bundle.putInt(KEY_INDEX[1], mNumberOfCorrectAnswers);
    }
    @Override
    protected void onStart() {
        super.onStart();
        Log.d(TAG, "onStart:" + this);
    }
    @Override
    protected void onResume() {
        super.onResume();
        Log.d(TAG, "onResume:" + this);
    }
    @Override
    protected  void onPause() {
        super.onPause();
        Log.d(TAG, "onPause:" + this);
    }
    @Override
    protected  void onStop() {
        super.onStop();
        Log.d(TAG, "onStop:" + this);
    }
    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.d(TAG, "onDestroy:" + this);
    }
    public MainActivity() {
        //String.valueOf(Object) can handle null input. Of course this can not be null here
        //String s = String.format("==CONSTRUCTOR: %s%m", String.valueOf(this));//will throw exception
        String s = String.format("CONSTRUCTOR@%s%n", String.valueOf(this));
        Log.d(TAG, s);
    }
}
