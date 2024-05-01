package xyz.dnigamer.roombavirtualwall

import android.content.Context
import android.hardware.ConsumerIrManager
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.Button
import android.widget.TextView

class MainActivity : AppCompatActivity() {
    companion object {
        init {
            System.loadLibrary("rvwmain")
        }
    }
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val checkButton = findViewById<Button>(R.id.compatCheckButton)
        checkButton.setOnClickListener {
            checkCompatibility()
        }

        if (checkCompatibility()) {
            val startButton = findViewById<Button>(R.id.startIRButton)
            startButton.setOnClickListener {
                startIR()
            }

            val stopButton = findViewById<Button>(R.id.stopIRButton)
            stopButton.setOnClickListener {
                stopIR()
            }
        }
    }

    private fun checkCompatibility(): Boolean{
        val state = findViewById<TextView>(R.id.compStateLabel)
        val startBtn = findViewById<Button>(R.id.startIRButton)
        val stopBtn = findViewById<Button>(R.id.stopIRButton)
        val irstate = findViewById<TextView>(R.id.stateIRLabel)
        val manager = getSystemService(Context.CONSUMER_IR_SERVICE) as ConsumerIrManager

        if (!manager.hasIrEmitter()) {
            state.text = getString(R.string.noState)
            state.setTextColor(getColor(R.color.red))
            irstate.text = getString(R.string.notsupportedIR)
            startBtn.setBackgroundColor(getColor(R.color.primaryLight))
            startBtn.isClickable = false
            stopBtn.setBackgroundColor(getColor(R.color.primaryLight))
            stopBtn.isClickable = false
            return false
        } else {
            state.text = getString(R.string.yesState)
            state.setTextColor(getColor(R.color.green))
            startBtn.setBackgroundColor(getColor(R.color.primary))
            startBtn.isClickable = true
            stopBtn.setBackgroundColor(getColor(R.color.primary))
            stopBtn.isClickable = true
            return true
        }
    }

    private fun startIR() {
        // TODO: Implement IR transmission (send IR codes) - start
    }

    private fun stopIR() {
        // TODO: Implement IR transmission (send IR codes) - stop
    }
}