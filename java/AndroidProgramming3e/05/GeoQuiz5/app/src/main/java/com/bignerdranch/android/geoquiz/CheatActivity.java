package com.bignerdranch.android.geoquiz;

import android.content.Context;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class CheatActivity extends AppCompatActivity {
    private static final String TAG = "GeoQuiz2";
    private static final String EXTRA_ANSWER_IS_TRUE =
            "com.bignerdranch.android.geoquiz.answer_is_true";
    private static final String EXTRA_MAIN_INDEX =
            "com.bignerdranch.android.geoquiz.index";
    private final static String EXTRA_ANSWER_SHOWN =
            "com.bignerdranch.android.geoquiz.answer_shown";
    private final static String EXTRA_SHOW_TIMES =
            "com.bignerdranch.android.geoquiz.answer_shown_times";
    private boolean mAnswerIsTrue;
    private TextView mAnswerTextView;
    private Button mShowAnswerButton;
    private Button mExitButton;
    private int mCount = 0;
    public static Intent newIntent(Context packageContext, boolean answerIsTrue, int index) {
        Intent intent = new Intent(packageContext, CheatActivity.class);
        intent.putExtra(EXTRA_ANSWER_IS_TRUE, answerIsTrue);
        intent.putExtra(EXTRA_MAIN_INDEX, index);
        return intent;
    }
    public static boolean wasAnswerShown(Intent intent) {
        return intent.getBooleanExtra(EXTRA_ANSWER_SHOWN, false);
    }
    public static int getShownTimes(Intent intent) {
        return intent.getIntExtra(EXTRA_SHOW_TIMES, -1);
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_cheat);
        mAnswerIsTrue = getIntent().getBooleanExtra(EXTRA_ANSWER_IS_TRUE, false);
        int index = getIntent().getIntExtra(EXTRA_MAIN_INDEX, -1);
        dump("onCreate" + index);
        mAnswerTextView = (TextView)findViewById(R.id.answer_text_view);
        mShowAnswerButton = (Button)findViewById(R.id.show_answer_button);
        mShowAnswerButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int answer;
                if (mAnswerIsTrue) {
                    answer = R.string.true_button;
                } else {
                    answer = R.string.false_button;
                }
                mAnswerTextView.setText(answer);
                setAnswerShownResult(true);
                Log.d(TAG, "count " + mCount);
            }
        });
        mExitButton = (Button)findViewById(R.id.exit_button);
        mExitButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
                Log.d(TAG, "call finish");
            }
        });
    }
    private void setAnswerShownResult(boolean isAnswerShown) {
        Intent data = new Intent();
        data.putExtra(EXTRA_SHOW_TIMES, ++mCount);
        data.putExtra(EXTRA_ANSWER_SHOWN, isAnswerShown);
        setResult(RESULT_OK, data);
    }
    CheatActivity() {
        //super();//can be ignored!
        dump("CONS");
    }
    @Override
    protected void onStart() {
        super.onStart();
        dump("onStart");
    }
    @Override
    protected void onResume() {
        super.onResume();
        dump("onResume");
    }
    @Override
    protected void onPause() {
        super.onPause();
        dump("onPause");
    }
    @Override
    protected void onStop() {
        super.onStop();
        dump("onStop");
    }
    @Override
    protected void onDestroy() {
        super.onDestroy();
        dump("onDestroy" + getIntent().getIntExtra(EXTRA_MAIN_INDEX, -1));
    }
    @Override
    protected void onSaveInstanceState(Bundle savedInstanceState) {
        super.onSaveInstanceState(savedInstanceState);
        //if (m)
        dump("save");
    }
    private void dump(String func) {
        String fmt = String.format("%s: %s", func, String.valueOf(this));
        Log.d(TAG, fmt);
    }
}
