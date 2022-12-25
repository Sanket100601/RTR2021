package com.ssp.window;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

// text view related imports
import  androidx.appcompat.widget.AppCompatTextView;
// color related import
import  android.graphics.Color;
import  android.view.Gravity;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
		getWindow().getDecorView().setBackgroundColor(Color.BLACK);
		AppCompatTextView myView = new AppCompatTextView(this);
		myView.setTextSize(64);
		myView.setTextColor(Color.GREEN);
		myView.setGravity(Gravity.CENTER);
		myView.setText("Hello World!!!");
		setContentView(myView);
    }
}