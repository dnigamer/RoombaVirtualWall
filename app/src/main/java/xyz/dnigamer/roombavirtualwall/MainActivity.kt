package xyz.dnigamer.roombavirtualwall

import android.content.Context
import android.hardware.ConsumerIrManager
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.Button
import android.widget.TextView
import kotlinx.coroutines.*

class MainActivity : AppCompatActivity() {
    private var isRunning = false

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

    private val job = Job()
    private val scope = CoroutineScope(Dispatchers.Default + job)

    private fun startIR() {
        if (!isRunning) {
            val irState = findViewById<TextView>(R.id.stateIRLabel)
            Log.d("IR", "IR is on")
            isRunning = true
            irState.text = getString(R.string.onIR)

            val manager = getSystemService(Context.CONSUMER_IR_SERVICE) as ConsumerIrManager
            val irExpression = IrExpression.NEC.buildNEC(32, 0xA2)
            scope.launch {
                while (isRunning) {
                    delay(300) // replace 100 with the delay in milliseconds you want between each execution
                    withContext(Dispatchers.Main) {
                        manager.transmit(irExpression.frequency, irExpression.pattern)
                    }
                }
            }
        } else {
            Log.d("IR", "IR is already on")
        }
    }

    private fun stopIR() {
        val irState = findViewById<TextView>(R.id.stateIRLabel)
        isRunning = false
        irState.text = getString(R.string.offIR)
        Log.d("IR", "IR is off")
        job.cancel() // cancel the coroutine when stopping IR
    }
}