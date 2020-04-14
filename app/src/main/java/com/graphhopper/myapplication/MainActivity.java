package com.graphhopper.myapplication;

import android.Manifest;
import android.os.Bundle;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;
import java.io.IOException;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        int CONSTANT = 2;
        ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.READ_EXTERNAL_STORAGE}, CONSTANT);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        try {
            String out = fromNative(new File(getFilesDir(), "gh.json").getCanonicalPath());
            // TODO read string with GraphHopper client?
            JSONObject json = new JSONObject(out);
            tv.setText("calculated distance: " + json.getJSONArray("paths").getJSONObject(0).getDouble("distance"));
        } catch (IOException | JSONException e) {
            tv.setText("Problem when calculating route");
            e.printStackTrace();
        }
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String fromNative(String exchangeFileName);
}
