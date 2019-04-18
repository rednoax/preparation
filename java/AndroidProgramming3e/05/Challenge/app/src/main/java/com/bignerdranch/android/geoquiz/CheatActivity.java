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
    private static final String TAG = "GeoQuiz***";
    private static final String EXTRA_ANSWER_IS_TRUE =
            "com.bignerdranch.android.geoquiz.answer_is_true";
    private static final String EXTRA_MAIN_INDEX =
            "com.bignerdranch.android.geoquiz.index";
    private final static String EXTRA_ANSWER_SHOWN =
            "com.bignerdranch.android.geoquiz.answer_shown";
    private final static String EXTRA_SHOW_TIMES =
            "com.bignerdranch.android.geoquiz.answer_shown_times";
    private final static String KEY_INDEX = "cheat_index";
    private static Bundle sSaved;
    private boolean mAnswerIsTrue;
    private boolean mIsCheater;
    private TextView mAnswerTextView;
    private Button mShowAnswerButton;
    private Button mExitButton;
    private int mCount = 0;
    static {
        Log.d(TAG, "######static block called only once when CheatActivity instance is 1st geneated");
    }
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
        mAnswerTextView = (TextView)findViewById(R.id.answer_text_view);
        mShowAnswerButton = (Button)findViewById(R.id.show_answer_button);
        int index = getIntent().getIntExtra(EXTRA_MAIN_INDEX, -1);
        String fmt = "(";
        if (savedInstanceState != null) {
            if(savedInstanceState == sSaved)
                fmt += "sSaved";
            mCount = savedInstanceState.getInt(EXTRA_SHOW_TIMES, -1);
            mIsCheater = savedInstanceState.getBoolean(EXTRA_ANSWER_SHOWN, false);
            mAnswerTextView.setText(savedInstanceState.getString(KEY_INDEX, ""));
            if (mIsCheater == true)
                setAnswerShownResult();
        }
        dump("onCreate" + index + fmt +savedInstanceState);
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
                mCount++;
                mIsCheater = true;
                setAnswerShownResult();
                Log.d(TAG, "setResult:" + mCount);
            }
        });
        mExitButton = (Button)findViewById(R.id.exit_button);
        mExitButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
                Log.d(TAG, "Exit Button presses");
            }
        });
    }
    @Override
    public void finish() {
        super.finish();
        Log.d(TAG, "finish()");
    }
    private void setAnswerShownResult() {//boolean isAnswerShown) {
        Intent data = new Intent();
        data.putExtra(EXTRA_SHOW_TIMES, mCount);
        data.putExtra(EXTRA_ANSWER_SHOWN, mIsCheater);
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
        sSaved = savedInstanceState;
        savedInstanceState.putInt(EXTRA_SHOW_TIMES, mCount);
        savedInstanceState.putBoolean(EXTRA_ANSWER_SHOWN, mIsCheater);
        savedInstanceState.putString(KEY_INDEX, mAnswerTextView.getText().toString());
        dump("onSaveInstanceState");
    }
    private void dump(String func) {
        String fmt = String.format("%s: %s", func, String.valueOf(this));
        Log.d(TAG, fmt);
    }
}
