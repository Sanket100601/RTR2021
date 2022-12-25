package com.ssp.window;
import android.content.Context;
import android.graphics.Color;
import android.view.Gravity;

import androidx.appcompat.widget.AppCompatAutoCompleteTextView;

public class MyView extends AppCompatAutoCompleteTextView
{
	MyView(Context context)
	{
	super(context);
	setTextSize(48);
	setTextColor(Color.GREEN);
	setGravity(Gravity.CENTER);
	setText("Hello world!!!");
	}

}
