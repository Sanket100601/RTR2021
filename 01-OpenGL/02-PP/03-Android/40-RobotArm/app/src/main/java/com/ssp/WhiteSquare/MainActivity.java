package com.ssp.window;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle; 

import android.graphics.Color;
import android.content.pm.ActivityInfo;  //pm : package manager

//for fullScreen
import androidx.core.view.WindowCompat;
import androidx.core.view.WindowInsetsControllerCompat;
import androidx.core.view.WindowInsetsCompat;

public class MainActivity extends AppCompatActivity {

    private GLESView glesView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
       // setContentView(R.layout.activity_main);
       //doing fullScreen
      /* getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN |
                                                        View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | 
                                                        View.SYSTEM_UI_FLAG_IMMERSIVE);

        //titlebar off  or actionbar  of
        supportRequestWindowFeature(Window.FEATURE_NO_TITLE);*/

        //above method is also ok for doing  full scree but it gives warning hence user below way.
	

       //hide actionBar or title bar
        getSupportActionBar().hide();

        //fullScreen
        WindowCompat.setDecorFitsSystemWindows(getWindow(),false);
        //1 para: which window
         //2 para: if this is true i.e  set the all view with the hlpe of decore i.e keyboard,actiobbar,navihgatio bar etc.  if () {
	    //flase i.e dont set this value.
            
        //hiding system bar and IME(input method editor)
        WindowInsetsControllerCompat windowInsetsControllerCompat=WindowCompat.getInsetsController(getWindow(),getWindow().getDecorView());
        
        windowInsetsControllerCompat.hide(WindowInsetsCompat.Type.systemBars() | WindowInsetsCompat.Type.ime());

        
       //seeting background color
       getWindow().getDecorView().setBackgroundColor(Color.rgb(0,0,0));

       //forcing to landscape
       setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);


       glesView=new GLESView(this);
       setContentView(glesView);

    }

    @Override
    protected void onPause(){
        super.onPause();
    }

    @Override
    protected void onResume(){
        super.onResume();
    }
}