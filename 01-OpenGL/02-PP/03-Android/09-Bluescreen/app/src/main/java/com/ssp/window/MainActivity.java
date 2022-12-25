package com.ssp.window;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowCompat;

import android.os.Bundle;
import android.graphics.Color;
import android.content.pm.ActivityInfo; // for Landscape orientation pm=package management

//For Full screen
import androidx.core.view.WindowCompat;
import androidx.core.view.WindowInsetsControllerCompat;
import androidx.core.view.WindowInsetsCompat;

public class MainActivity extends AppCompatActivity {
    private GLESView glesView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        //Hide action bar ot title bar
        getSupportActionBar().hide();

        //Fullscreen
        WindowCompat.setDecorFitsSystemWindows(getWindow(),false);

        //Hiding system bars and IME
        WindowInsetsControllerCompat windowInsetsControllerCompat= WindowCompat.getInsetsController(getWindow(), getWindow().getDecorView());
        windowInsetsControllerCompat.hide(WindowInsetsCompat.Type.systemBars()|WindowInsetsCompat.Type.ime());

        // Forced Landscape Orientation
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

        //setting background color
        getWindow().getDecorView().setBackgroundColor(Color.BLACK);
        glesView=new GLESView(this);
        setContentView(glesView);
    }

    @Override
    protected void onPause()
    {
        super.onPause();
    }

    @Override
    protected void onResume()
    {
        super.onResume();
    }
}
