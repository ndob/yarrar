package com.ndob.yarryarexample;

import android.os.Bundle;
import android.widget.LinearLayout;

import com.ndob.yarrar.YarrarActivity;

public class MainActivity extends YarrarActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        initialize((LinearLayout) findViewById(R.id.mainlayout));
    }

    @Override
    public void onYarrarInitialized() {
        float[] cubeVertices = {
            // bottom
            -1.0f,  -1.0f,  0.0f,
            1.0f,   -1.0f,  0.0f,
            -1.0f,  -1.0f,  2.0f,
            1.0f,   -1.0f,  0.0f,
            1.0f,   -1.0f,  2.0f,
            -1.0f,  -1.0f,  2.0f,

            // top
            -1.0f,  1.0f,   0.0f,
            -1.0f,  1.0f,   2.0f,
            1.0f,   1.0f,   0.0f,
            1.0f,   1.0f,   0.0f,
            -1.0f,  1.0f,   2.0f,
            1.0f,   1.0f,   2.0f,

            // front
            -1.0f,  -1.0f,  2.0f,
            1.0f,   -1.0f,  2.0f,
            -1.0f,  1.0f,   2.0f,
            1.0f,   -1.0f,  2.0f,
            1.0f,   1.0f,   2.0f,
            -1.0f,  1.0f,   2.0f,

            // back
            -1.0f,  -1.0f,  0.0f,
            -1.0f,  1.0f,   0.0f,
            1.0f,   -1.0f,  0.0f,
            1.0f,   -1.0f,  0.0f,
            -1.0f,  1.0f,   0.0f,
            1.0f,   1.0f,   0.0f,

            // left
            -1.0f,  -1.0f,  2.0f,
            -1.0f,  1.0f,   0.0f,
            -1.0f,  -1.0f,  0.0f,
            -1.0f,  -1.0f,  2.0f,
            -1.0f,  1.0f,   2.0f,
            -1.0f,  1.0f,   0.0f,

            // right
            1.0f,   -1.0f,  2.0f,
            1.0f,   -1.0f,  0.0f,
            1.0f,   1.0f,   0.0f,
            1.0f,   -1.0f,  2.0f,
            1.0f,   1.0f,   0.0f,
            1.0f,   1.0f,   2.0f
        };
        int coordinateSystemId = 30;
        addModel(coordinateSystemId, cubeVertices);
    }

}
