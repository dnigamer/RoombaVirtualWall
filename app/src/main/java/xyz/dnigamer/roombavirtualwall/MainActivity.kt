package xyz.dnigamer.roombavirtualwall

import android.content.Context
import android.hardware.ConsumerIrManager
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.Button
import android.widget.TextView

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val checkButton = findViewById<Button>(R.id.compatCheckButton)
        checkButton.setOnClickListener {
            checkCompatibility()
        }

        checkCompatibility()
    }

    private fun checkCompatibility() {
        val state = findViewById<TextView>(R.id.compStateLabel)
        val startBtn = findViewById<Button>(R.id.startIRButton)
        val stopBtn = findViewById<Button>(R.id.stopIRButton)
        val irstate = findViewById<TextView>(R.id.stateIRLabel)
        val manager = getSystemService(Context.CONSUMER_IR_SERVICE) as? ConsumerIrManager

        if (manager == null) {
            state.text = getString(R.string.noState)
            state.setTextColor(getColor(R.color.red))
            irstate.text = getString(R.string.notsupportedIR)
            startBtn.setBackgroundColor(getColor(R.color.primaryLight))
            startBtn.isClickable = false
            stopBtn.setBackgroundColor(getColor(R.color.primaryLight))
            stopBtn.isClickable = false
        } else {
            state.text = getString(R.string.yesState)
            state.setTextColor(getColor(R.color.green))
            startBtn.setBackgroundColor(getColor(R.color.primary))
            startBtn.isClickable = true
            stopBtn.setBackgroundColor(getColor(R.color.primary))
            stopBtn.isClickable = true
        }
    }
}