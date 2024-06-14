package xyz.dnigamer.roombavirtualwall

import xyz.dnigamer.roombavirtualwall.IrExpressionBuilder.Companion.irExpressionBuilder
import xyz.dnigamer.roombavirtualwall.IrExpressionBuilder.SequenceDefinition
import xyz.dnigamer.roombavirtualwall.IrExpressionBuilder.Companion.simpleSequence

class IrExpression(val frequency: Int, val pattern: IntArray) {
    object NEC {
        private const val FREQUENCY = 38028 // T = 26.296 us
        private const val HDR_MARK = 342
        private const val HDR_SPACE = 171
        private const val BIT_MARK = 21
        private const val ONE_SPACE = 60
        private const val ZERO_SPACE = 21
        private val SEQUENCE_DEFINITION: SequenceDefinition =
            simpleSequence(BIT_MARK, ONE_SPACE, BIT_MARK, ZERO_SPACE)

        fun buildNEC(bitCount: Int, data: Int): IrExpression {
            return irExpressionBuilder(FREQUENCY)
                .pair(HDR_MARK, HDR_SPACE)
                .sequence(SEQUENCE_DEFINITION, bitCount, data)
                .mark(BIT_MARK)
                .build()
        }
    }
}

