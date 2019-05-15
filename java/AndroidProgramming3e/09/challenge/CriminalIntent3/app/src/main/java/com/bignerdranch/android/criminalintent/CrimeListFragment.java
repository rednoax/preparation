package com.bignerdranch.android.criminalintent;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import java.util.Locale;
/*
import static android.view.View.GONE;
import static android.view.View.VISIBLE;
*/
import static android.view.View.*;
import static java.lang.String.valueOf;

public class CrimeListFragment extends Fragment {
    private final static String TAG = "CrimeListFragment";
    private RecyclerView mCrimeRecyclerView;
    private CrimeAdapter mAdapter;
    private void dump(String s) {
        String fmt = String.format("%s:%s", valueOf(this), s);
        Log.d(TAG, fmt);
    }
    public CrimeListFragment() {
        super();
        String fmt = String.format("cons: Activity %s", getActivity2());
        dump(fmt);
    }
    FragmentActivity getActivity2() {
        FragmentActivity fa = getActivity();
        dump(String.format("get2:%s", fa));
        return fa;
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_crime_list, container, false);
        mCrimeRecyclerView = (RecyclerView)view.findViewById(R.id.crime_recycler_view);
        mCrimeRecyclerView.setLayoutManager(new LinearLayoutManager(getActivity2()));
        updateUI();
        String fmt = String.format("onCreateView(,%s,%s)%n" +
                "%s==%s:%b%n" +
                "getActivity:%s%n" +
                "LayoutInflater:%s%n",
                valueOf(container), valueOf(savedInstanceState),
                valueOf(view), valueOf(mCrimeRecyclerView), view == (View)mCrimeRecyclerView,
                valueOf(getActivity2()),
                String.valueOf(LayoutInflater.from(getActivity2())));
        dump(fmt);
        return view;
    }
    private void updateUI() {
        CrimeLab crimeLab = CrimeLab.get(getActivity2());
        List<Crime> crimes = crimeLab.getCrimes();
        mAdapter = new CrimeAdapter(crimes);
        mCrimeRecyclerView.setAdapter(mAdapter);
    }
    private class CrimeHolder extends RecyclerView.ViewHolder implements View.OnClickListener {
        private final static String TAG = "CrimeHolder";
        private Crime mCrime;
        private ViewGroup mViewGroup;//==itemView ALWAYS
        private TextView mTitleTextView;
        private TextView mDateTextView;
        private ImageView mSolvedImageView;
        @Override
        public void onClick(View v) {
            int position = getAdapterPosition();
            String fmt = String.format("%x onClick(%s): Activity:%s, @%d", this.hashCode(), String.valueOf(v), String.valueOf(getActivity2()), position);
            Toast.makeText(getActivity2(), mCrime.getTitle() + " clicked!", Toast.LENGTH_SHORT).show();
            Log.d(TAG, fmt);
            if (position == 0) {
                /*
                1. click 0, toast show "crime#0 clicked", then position 0&1 will swap: position 0 holds "crime#1" and position 1 holds "crime#0"
                2. after 1's swap, then click "crime#1" that is now at position 0, toast show "crime#1 clicked", then position 0&1 will swap. So
                at last it becomes to the original state before 1
                3. In challenge test, swap will fail! TODO: find the reason?
                * */
                mAdapter.notifyItemMoved(position, position + 1);
            }
        }

        public CrimeHolder(LayoutInflater layoutInflater, ViewGroup parent) {
            super(layoutInflater.inflate(mAdapter.mLayoutId, parent ,false));
            mViewGroup = itemView.findViewById(mAdapter.mLinearId);
            mTitleTextView = (TextView) itemView.findViewById(mAdapter.mCrimeTitleId);
            mDateTextView = (TextView) itemView.findViewById(mAdapter.mCrimeDateId);
            mSolvedImageView = itemView.findViewById(R.id.crime_solved);
            itemView.setOnClickListener(this);
            String fmt = String.format("%nCrimeHolder(,%b):%x%n" +
                    "itemView:%s%n" +
                    "inflate return==root ViewGroup:%b%n",
                    parent == mCrimeRecyclerView, this.hashCode(),
                    String.valueOf(itemView),
                    itemView == (View)mViewGroup);
            dump(fmt);
        }
        private String getFormattedDate(Date date) {
            SimpleDateFormat formatter = new SimpleDateFormat("EEEE, MMMM dd, YYYY", Locale.US);
            return formatter.format(date);
        }
        public void bind(Crime crime) {
            mCrime = crime;
            mTitleTextView.setText(mCrime.getTitle());
            //mDateTextView.setText(mCrime.getDate().toString());
            mDateTextView.setText(getFormattedDate(mCrime.getDate()));
            mSolvedImageView.setVisibility(crime.isSolved()? VISIBLE: GONE);
        }
    }
    private class CrimeAdapter extends RecyclerView.Adapter<CrimeHolder> {
        private final static String TAG = "CrimeAdapter";
        private final static int VIEWTYPE = 1;
        private int mLayoutId;
        private int mLinearId;
        private int mCrimeTitleId;
        private int mCrimeDateId;
        private List<Crime> mCrimes;
        public CrimeAdapter(List<Crime> crimes) {
            mCrimes = crimes;
        }
        private int mGetItemViewTypeTimes = 0;
        private int mLastPostion = -1;
        private int mOnBindViewHolderTimes = 0;
        public void recGetTimes(int position) {
            mGetItemViewTypeTimes++;
            mLastPostion = position;
        }
        public void recBindTimes(int position) {
            String s = "";
            if (mGetItemViewTypeTimes > mOnBindViewHolderTimes) {
                mOnBindViewHolderTimes++;
                if (position != mLastPostion) {
                    s ="***";
                }
                /*From debug:
                getItemViewType is called much more times than onBindViewHolder! However, onBindViewHolder(,position)
                will be always preceded with a call of getItemViewType(position). And their position are CERTAINLY the same!
                */
                s = String.format("%s%d==%d? @%d", s, mGetItemViewTypeTimes, mOnBindViewHolderTimes, mLastPostion);
                Log.d(TAG, s);
            }
        }
        @Override
        public CrimeHolder onCreateViewHolder(ViewGroup parent, int viewType) {
            LayoutInflater layoutInflater = LayoutInflater.from(getActivity2());
            String fmt = String.format("Adapter:onCreateViewHolder(0x%x,%d): LayoutInflater:%s",
                    parent.hashCode(), viewType, String.valueOf(layoutInflater));
            dump(fmt);
            mLayoutId = R.layout.list_item_crime;
            mLinearId = R.id.crime_linear_layout;
            mCrimeTitleId = R.id.crime_title;
            mCrimeDateId = R.id.crime_date;
            if (viewType == VIEWTYPE) {
                mLayoutId = R.layout.list_item_crime2;
                mLinearId = R.id.crime_linear_layout2;
                mCrimeTitleId = R.id.crime_title2;
                mCrimeDateId = R.id.crime_date2;
            }
            return new CrimeHolder(layoutInflater, parent);
        }
        @Override
        public void onBindViewHolder(CrimeHolder holder, int position) {
            Crime crime = mCrimes.get(position);
            String fmt = String.format("Adapter:onBind(%x, %d)", holder.hashCode(), position);
            dump(fmt);
            holder.bind(crime);
            recBindTimes(position);
        }
        @Override
        public int getItemCount() {
            dump("Adapter:getItemCount()");
            return mCrimes.size();
        }
        @Override
        public int getItemViewType(int position) {
            int ret = super.getItemViewType(position);
            recGetTimes(position);
            Crime crime = mCrimes.get(position);
            if (crime.isRequiresPolice()) {
                ret = VIEWTYPE;
            }
            String fmt = String.format("Adapter:getItemViewType(%d): %d", position, ret);
            dump(fmt);
            return ret;
        }
    }

}
