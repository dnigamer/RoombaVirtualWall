package xyz.dnigamer.roombavirtualwall


class IrExpressionBuilder private constructor(val frequency: Int) {
    private val buffer: MutableList<Int>
    private var lastMark: Boolean? = null

    init {
        buffer = ArrayList()
    }

    private fun appendSymbol(mark: Boolean, interval: Int): IrExpressionBuilder {
        if (lastMark == null || lastMark != mark) {
            buffer.add(interval)
            lastMark = mark
        } else {
            val lastIndex = buffer.size - 1
            buffer[lastIndex] = buffer[lastIndex] + interval
        }
        return this
    }

    fun mark(interval: Int): IrExpressionBuilder {
        return appendSymbol(true, interval)
    }

    fun space(interval: Int): IrExpressionBuilder {
        return appendSymbol(false, interval)
    }

    fun pair(on: Int, off: Int): IrExpressionBuilder {
        return mark(on).space(off)
    }

    fun reversePair(off: Int, on: Int): IrExpressionBuilder {
        return space(off).mark(on)
    }

    fun delay(ms: Int): IrExpressionBuilder {
        return space(ms * frequency / 1000)
    }

    fun sequence(definition: SequenceDefinition, length: Int, data: Int): IrExpressionBuilder {
        return sequence(definition, TOP_BIT_32, length, data.toLong())
    }

    fun sequence(definition: SequenceDefinition, length: Int, data: Long): IrExpressionBuilder {
        return sequence(definition, TOP_BIT_64, length, data)
    }

    fun sequence(
        definition: SequenceDefinition,
        topBit: Long,
        length: Int,
        data: Long
    ): IrExpressionBuilder {
        for (index in 0 until length) {
            if (data and topBit != 0L) {
                definition.one(this, index)
            } else {
                definition.zero(this, index)
            }
            data shl 1
        }
        return this
    }

    fun build(): IrExpression {
        return IrExpression(frequency, buildSequence())
    }

    private fun buildSequence(): IntArray {
        return buildRawSequence(buffer)
    }

    fun getBuffer(): List<Int> {
        return buffer
    }

    interface SequenceDefinition {
        fun one(builder: IrExpressionBuilder?, index: Int)
        fun zero(builder: IrExpressionBuilder?, index: Int)
    }

    companion object {
        const val TOP_BIT_32 = 0x1L shl 31
        const val TOP_BIT_64 = 0x1L shl 63
        fun irExpressionBuilder(frequency: Int): IrExpressionBuilder {
            return IrExpressionBuilder(frequency)
        }

        fun simpleSequence(
            oneMark: Int,
            oneSpace: Int,
            zeroMark: Int,
            zeroSpace: Int
        ): SequenceDefinition {
            return object : SequenceDefinition {
                override fun one(builder: IrExpressionBuilder?, index: Int) {
                    builder?.pair(oneMark, oneSpace)
                }

                override fun zero(builder: IrExpressionBuilder?, index: Int) {
                    builder?.pair(zeroMark, zeroSpace)
                }
            }
        }

        fun buildRawSequence(vararg rawData: Int): IntArray {
            return rawData
        }

        fun buildRawSequence(buffer: List<Int>): IntArray {
            val result = IntArray(buffer.size)
            for (i in buffer.indices) {
                result[i] = buffer[i]
            }
            return result
        }

        fun buildRawSequence(dataStream: Iterable<Int>): IntArray {
            if (dataStream is List<*>) {
                return buildRawSequence(dataStream as List<Int>)
            }
            val buffer = ArrayList<Int>()
            for (data in dataStream) {
                buffer.add(data)
            }
            return buildRawSequence(buffer)
        }
    }
}

