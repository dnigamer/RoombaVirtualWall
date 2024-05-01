#include "IRExpressionBuilder.hpp"

class NEC {
private:
    static const int FREQUENCY = 38028;
    static const int HDR_MARK = 342;
    static const int HDR_SPACE = 171;
    static const int BIT_MARK = 21;
    static const int ONE_SPACE = 60;
    static const int ZERO_SPACE = 21;

    static IRExpressionBuilder::SequenceDefinition* SEQUENCE_DEFINITION;

public:
    static void initialize() {
        SEQUENCE_DEFINITION = new IRExpressionBuilder::SimpleSequence(
                ONE_SPACE, ZERO_SPACE, ZERO_SPACE, ZERO_SPACE
        );
    }

    static IrCommand buildNEC(int bitCount, int data) {
        initialize();
        return IRExpressionBuilder::irCommandBuilder(FREQUENCY)
                .IRExpressionBuilder::pair(HDR_MARK, HDR_SPACE)
                .IRExpressionBuilder::sequence(SEQUENCE_DEFINITION, bitCount, data)
                .IRExpressionBuilder::mark(BIT_MARK)
                .build();
    }
};

