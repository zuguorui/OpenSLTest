package com.example.opensltest

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    var TAG: String = "MainActivity"
    var isPlaying: Boolean = false

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        var result: Boolean
        result = nCreateEngine()
        Log.d(TAG, "create engine ${result}")
        result = nCreatePlayer()
        Log.d(TAG, "create player ${result}")

        // Example of a call to a native method
        btn_play.setOnClickListener {
            var result = nPlay()
            Log.d(TAG, "play ${result}")
        }

        btn_stop.setOnClickListener {
            var result = nStop()
            Log.d(TAG, "stop ${result}")
        }
    }

    override fun onDestroy() {
        nDestroy()
        super.onDestroy()
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String
    external fun nCreateEngine(): Boolean
    external fun nCreatePlayer(): Boolean
    external fun nPlay(): Boolean
    external fun nStop(): Boolean
    external fun nDestroy(): Boolean

    companion object {

        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }
}
