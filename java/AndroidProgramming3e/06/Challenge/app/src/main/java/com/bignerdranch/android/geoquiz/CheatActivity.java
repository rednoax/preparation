package com.bignerdranch.android.geoquiz;

import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.ViewAnimationUtils;
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
    private TextView mAPILevelView;
    private Button mShowAnswerButton;
    private Button mExitButton;
    private Button mDisplayButton;
    private boolean mVisiable = true;
    private int mCount = 0;
    static {
        Log.d(TAG, "######static block called only once when CheatActivity instance is 1st geneated or its static func is called");
    }
    public static void dummy() {
        Log.d(TAG, "run dummy");
    }
    /*
    Calling static func newIntent of Class CheatActivity from other class, ie MainActivity will trigger the class loading for CheatActivity
    iff it has not been loaded. Once the class is loaded, it will never been reload unless the PID exit(). In other words, for a specified
    PID, its class loading for CheatActivity or MainActivity can happen once at most (for CheatActivity, which is not launcher activity,
    if no click on "Cheat" button, then CheatActivity class will not be loaded at all). So static func newIntent is actually run after loading
    class CheatActivity. That's why "before new" is after Log.d in static block:
2019-05-16 21:29:47.095 5642-5642/com.bignerdranch.android.geoquiz D/GeoQuiz: Resume:com.bignerdranch.android.geoquiz.MainActivity@76749f4
2019-05-16 21:29:52.756 5642-5642/com.bignerdranch.android.geoquiz D/GeoQuiz***: ######static block called only once when CheatActivity instance is 1st geneated
2019-05-16 21:29:52.756 5642-5642/com.bignerdranch.android.geoquiz D/GeoQuiz***: before new
2019-05-16 21:29:52.756 5642-5642/com.bignerdranch.android.geoquiz D/GeoQuiz***: after new
2019-05-16 21:29:52.782 5642-5642/com.bignerdranch.android.geoquiz D/GeoQuiz: Pause:com.bignerdranch.android.geoquiz.MainActivity@76749f4
    */
    public static Intent newIntent(Context packageContext, boolean answerIsTrue, int index) {
        Log.d(TAG, "before new");
        Intent intent = new Intent(packageContext, CheatActivity.class);
        Log.d(TAG, "after new");
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
        mAPILevelView = (TextView)findViewById(R.id.api_level_view);
        String api = String.format("API Level %d", Build.VERSION.SDK_INT);
        mAPILevelView.setText(api);
        mShowAnswerButton = (Button)findViewById(R.id.show_answer_button);
        mDisplayButton = (Button)findViewById(R.id.display_button);
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
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                    int cx = mShowAnswerButton.getWidth() / 2;
                    int cy = mShowAnswerButton.getWidth() / 2;
                    float radius = mShowAnswerButton.getWidth();
                    Animator anim = ViewAnimationUtils.createCircularReveal(mShowAnswerButton, cx, cy, radius, 0);
                    anim.addListener(new AnimatorListenerAdapter() {
                        @Override
                        public void onAnimationEnd(Animator animation) {
                            super.onAnimationEnd(animation);
                            mShowAnswerButton.setVisibility(View.INVISIBLE);
                            mVisiable = false;
                        }
                    });
                    anim.start();
                    Log.d(TAG, "API" + Build.VERSION.SDK_INT + ":show ani");
                } else {
                    Log.d(TAG, "cannot run animation");
                }

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
        mDisplayButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int visiable;
                mVisiable = !mVisiable;
                if (mVisiable == true)
                    visiable = View.VISIBLE;
                else
                    visiable = View.INVISIBLE;
                mShowAnswerButton.setVisibility(visiable);
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
        String fmt = String.format("CONS %d", Build.VERSION.SDK_INT);
        dump(fmt);
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
