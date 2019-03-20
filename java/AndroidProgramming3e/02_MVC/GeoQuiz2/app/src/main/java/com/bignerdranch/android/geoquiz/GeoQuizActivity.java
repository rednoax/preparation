package com.bignerdranch.android.geoquiz;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ImageButton;
import android.view.View;
import android.util.Log;

public class GeoQuizActivity extends AppCompatActivity {
    private static final String TAG = "QuizActivity";
    private Button mTrueButton;
    private Button mFalseButton;
    private Button[] mPrevButton = new Button[2];
    private Button[] mNextButton = new Button[2];
    private ImageButton mPrevImageButton;
    private ImageButton mNextImageButton;
    private TextView mQuestionTextView;
    private int mCurrentIndex = 0;//verbose assignment
    private Question[] mQuestionBank = new Question[] {
            new Question(R.string.question_australia, true),
            new Question(R.string.question_oceans, true),
            new Question(R.string.question_mideast, false),
            new Question(R.string.question_africa, false),
            new Question(R.string.question_americas, true),
            new Question(R.string.question_asia, true),
    };
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mTrueButton = (Button)findViewById(R.id.true_button);
        mFalseButton = (Button)findViewById(R.id.false_button);
        mPrevButton[0] = (Button)findViewById(R.id.prev_button);
        mPrevButton[1] = (Button)findViewById(R.id.prev_button2);
        mPrevImageButton = (ImageButton)findViewById(R.id.prev_button3);
        mNextButton[0] = (Button)findViewById(R.id.next_button);
        mNextButton[1] = (Button)findViewById(R.id.next_button2);
        mNextImageButton = (ImageButton)findViewById(R.id.next_button3);
        mQuestionTextView = (TextView)findViewById(R.id.question_text_view);
        updateQuestion(true);
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
        for (int i = 0; i < mNextButton.length; i++) {
            mPrevButton[i].setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    updateQuestion(false);
                }
            });
            mNextButton[i].setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    updateQuestion(true);//mQuestionTextView.setText(mQuestionBank[mCurrentIndex].getTextResId());
                }
            });
        }
        mPrevImageButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                updateQuestion(false);
            }
        });
        mNextImageButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                updateQuestion(true);
            }
        });
        mQuestionTextView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                updateQuestion(true);
            }
        });
    }
    private void updateQuestion(boolean inc) {
        int oldIndex = mCurrentIndex;
        if (inc)
            mCurrentIndex = (mCurrentIndex + 1) % mQuestionBank.length;
        else {
            mCurrentIndex -= 1;
            if (mCurrentIndex < 0)
                mCurrentIndex = mQuestionBank.length - 1;
        }
        String string = String.format("%d=>%d%n", oldIndex, mCurrentIndex);
        Log.v(TAG, string);
        int question = mQuestionBank[mCurrentIndex].getTextResId();
        string = String.format("[%d=>%d]%s", oldIndex, mCurrentIndex, getResources().getString(question));
        //mQuestionTextView.setText(question);
        mQuestionTextView.setText(string);
    }
    private void checkAnswer(boolean userPressedTrue) {
        int messageResId;
        boolean answerTrue = mQuestionBank[mCurrentIndex].isAnswerTrue();
        if (answerTrue == userPressedTrue)
            messageResId = R.string.correct_toast;
        else
            messageResId = R.string.incorrect_toast;
        //Toast.makeText(this, messageResId, Toast.LENGTH_SHORT).show();
        //use the following better format
        Toast.makeText(this, messageResId, Toast.LENGTH_SHORT)
                .show();
    }
}
