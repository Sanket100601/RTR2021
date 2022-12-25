package com.ssp.window;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Color;
import android.os.Bundle;
import android.view.View;
import android.view.Window;

public class MainActivity extends AppCompatActivity {
	private MyView myView;       // Behaviour like Global variable

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);    // it is like wm_create()

         //doing fullscreen
        getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN |        // Full screen zal
                               View.SYSTEM_UI_FLAG_HIDE_NAVIGATION |    // Nevigation bar hide zala
                               View.SYSTEM_UI_FLAG_IMMERSIVE
         );

         supportRequestWindowFeature(Window.FEATURE_NO_TITLE);
        //setting background color
        getWindow().getDecorView().setBackgroundColor(Color.BLACK);
        myView=new MyView(this);
        setContentView(myView);


    }
}
