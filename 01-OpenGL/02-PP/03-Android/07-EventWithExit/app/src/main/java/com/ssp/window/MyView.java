package com.ssp.window;
import android.content.Context;
import android.graphics.Color;
import android.view.GestureDetector;
import android.view.Gravity;
// event related packages
import android.view.GestureDetector.OnDoubleTapListener;
import android.view.GestureDetector.OnGestureListener;
import android.view.MotionEvent;
import androidx.appcompat.widget.AppCompatTextView;

import androidx.appcompat.widget.AppCompatAutoCompleteTextView;

public class MyView extends AppCompatTextView implements OnDoubleTapListener,OnGestureListener
{
	private GestureDetector gestureDetector;

	MyView(Context context)
	{
	super(context);
	setTextSize(48);
	setTextColor(Color.GREEN);
	setGravity(Gravity.CENTER);
	setText("Hello world!!!");

	//Gesture related code
	gestureDetector= new GestureDetector(context,this,null,false);
	gestureDetector.setOnDoubleTapListener(this);
	}

	@Override
	public boolean onTouchEvent(MotionEvent e)
	{
		//code
		if(!gestureDetector.onTouchEvent(e))
			super.onTouchEvent(e);
		return true;
	}

	//Three Methods of On Double Tab listener Interface
	@Override
	public boolean onDoubleTap(MotionEvent e)
	{
		setText("***double tab***");
		return true;
	}

	@Override
	public boolean onDoubleTapEvent(MotionEvent e)
	{
		return true;
	}

	@Override
	public boolean onSingleTapConfirmed(MotionEvent e)
	{
		setText("***Single tab***");
		return true;
	}

	//Six methods of OnGesture Listener
	@Override
	public boolean onDown(MotionEvent e)
	{
		return true;
	}

	@Override
	public boolean onFling(MotionEvent e1,MotionEvent e2,float velocityX,float velocityY)
	{
		return true;
	}

	@Override
	public void onLongPress( MotionEvent e)
	{
		setText("***Longpress***");
	}


	@Override
	public boolean onScroll(MotionEvent e1,MotionEvent e2,float distanceX,float distanceY)
	{
		setText("***Swipe or scroll***");
		return true;
	}

	@Override
	public void onShowPress(MotionEvent e)
	{

	}

	@Override
	public boolean onSingleTapUp(MotionEvent e)
	{
		return true;
	}

}
