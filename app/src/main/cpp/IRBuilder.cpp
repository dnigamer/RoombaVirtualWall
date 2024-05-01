#include "IRExpressionBuilder.hpp"
#include <vector>
#include <string>
#include <sstream>

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
                .pair(HDR_MARK, HDR_SPACE)
                .sequence(SEQUENCE_DEFINITION, bitCount, data)
                .mark(BIT_MARK)
                .build();
    }
};

class SONY {
private:
    static const int FREQUENCY = 40000;
    static const int HDR_MARK = 96;
    static const int HDR_SPACE = 24;
    static const int ONE_MARK = 48;
    static const int ZERO_MARK = 24;

    static IRExpressionBuilder::SequenceDefinition* SEQUENCE_DEFINITION;

public:
    static void initialize() {
        SEQUENCE_DEFINITION = new IRExpressionBuilder::SimpleSequence(
                ONE_MARK, HDR_SPACE, ZERO_MARK, HDR_SPACE
        );
    }

    static IrCommand buildSONY(int bitCount, long data) {
        initialize();
        return IRExpressionBuilder::irCommandBuilder(FREQUENCY)
                .pair(HDR_MARK, HDR_SPACE)
                .sequence(SEQUENCE_DEFINITION, bitCount, data << (64 - bitCount))
                .build();
    }
};

class RC5 {
private:
    static const int FREQUENCY = 36000; // T = 27.78 us
    static IRExpressionBuilder::SequenceDefinition* SEQUENCE_DEFINITION;

public:
    static const int T1 = 32;

    class RC5SequenceDefinition : public IRExpressionBuilder::SequenceDefinition {
    public:
        void one(IRExpressionBuilder& builder, int index) override {
            builder.pair(RC5::T1, RC5::T1);
        }

        void zero(IRExpressionBuilder& builder, int index) override {
            builder.pair(RC5::T1, RC5::T1 * 3);
        }
    };


    static void initialize() {
        SEQUENCE_DEFINITION = new RC5SequenceDefinition();
    }

    static IrCommand buildRC5(int bitCount, int data) {
        initialize();
        return IRExpressionBuilder::irCommandBuilder(FREQUENCY)
                .pair(T1, T1)
                .sequence(SEQUENCE_DEFINITION, bitCount, data)
                .build();
    }
};

class RC6 {
private:
    static const int FREQUENCY = 36000; // T = 27.78 us
    static const int HDR_MARK = 96;
    static const int HDR_SPACE = 32;
    static IRExpressionBuilder::SequenceDefinition* SEQUENCE_DEFINITION;

public:
    static const int T1 = 16;

    class RC6SequenceDefinition : public IRExpressionBuilder::SequenceDefinition {
    private:
        int getTime(int index) {
            return index == 3 ? T1 + T1 : T1;
        }
    public:
        void one(IRExpressionBuilder& builder, int index) override {
            int t = getTime(index);
            builder.pair(t, t);
        }

        void zero(IRExpressionBuilder& builder, int index) override {
            int t = getTime(index);
            builder.reversePair(t, t);
        }
    };

    static void initialize() {
        SEQUENCE_DEFINITION = new RC6SequenceDefinition();
    }

    static IrCommand buildRC6(int bitCount, long data) {
        initialize();
        return IRExpressionBuilder::irCommandBuilder(FREQUENCY)
                .pair(HDR_MARK, HDR_SPACE)
                .pair(T1, T1)
                .sequence(SEQUENCE_DEFINITION, bitCount, data << (64 - bitCount))
                .build();
    }
};

class DISH {
private:
    static const int FREQUENCY = 56000; // T = 17.857 us
    static const int HDR_MARK = 22;
    static const int HDR_SPACE = 342;
    static const int BIT_MARK = 22;
    static const int ONE_SPACE = 95;
    static const int ZERO_SPACE = 157;
    static const int TOP_BIT = 0x8000;

    static IRExpressionBuilder::SequenceDefinition* SEQUENCE_DEFINITION;

public:
    static void initialize() {
        SEQUENCE_DEFINITION = new IRExpressionBuilder::SimpleSequence(
                BIT_MARK, ONE_SPACE, BIT_MARK, ZERO_SPACE
        );
    }

    static IrCommand buildDISH(int bitCount, int data) {
        initialize();
        return IRExpressionBuilder::irCommandBuilder(FREQUENCY)
                .pair(HDR_MARK, HDR_SPACE)
                .sequence(SEQUENCE_DEFINITION, TOP_BIT, bitCount, data)
                .build();
    }
};

class SHARP {
private:
    static const int FREQUENCY = 38000; // T = 26.315 us
    static const int BIT_MARK = 9;
    static const int ONE_SPACE = 69;
    static const int ZERO_SPACE = 30;
    static const int DELAY = 46;

    static const int INVERSE_MASK = 0x3FF;
    static const int TOP_BIT = 0x4000;

    static IRExpressionBuilder::SequenceDefinition* SEQUENCE_DEFINITION;

public:
    static void initialize() {
        SEQUENCE_DEFINITION = new IRExpressionBuilder::SimpleSequence(
                BIT_MARK, ONE_SPACE, BIT_MARK, ZERO_SPACE
        );
    }

    static IrCommand buildSHARP(int bitCount, int data) {
        initialize();
        return IRExpressionBuilder::irCommandBuilder(FREQUENCY)
                .sequence(SEQUENCE_DEFINITION, TOP_BIT, bitCount, data)
                .pair(BIT_MARK, ZERO_SPACE)
                .delay(DELAY)
                .sequence(SEQUENCE_DEFINITION, TOP_BIT, bitCount, data ^ INVERSE_MASK)
                .pair(BIT_MARK, ZERO_SPACE)
                .delay(DELAY)
                .build();
    }
};

class PANASONIC {
private:
    static const int FREQUENCY = 35000; // T = 28.571 us
    static const int HDR_MARK = 123;
    static const int HDR_SPACE = 61;
    static const int BIT_MARK = 18;
    static const int ONE_SPACE = 44;
    static const int ZERO_SPACE = 14;

    static const int ADDRESS_TOP_BIT = 0x8000;
    static const int ADDRESS_LENGTH = 16;
    static const int DATA_LENGTH = 32;

    static IRExpressionBuilder::SequenceDefinition* SEQUENCE_DEFINITION;

public:
    static void initialize() {
        SEQUENCE_DEFINITION = new IRExpressionBuilder::SimpleSequence(
                BIT_MARK, ONE_SPACE, BIT_MARK, ZERO_SPACE
        );
    }

    static IrCommand buildPANASONIC(int address, int data) {
        initialize();
        return IRExpressionBuilder::irCommandBuilder(FREQUENCY)
                .pair(HDR_MARK, HDR_SPACE)
                .sequence(SEQUENCE_DEFINITION, ADDRESS_TOP_BIT, ADDRESS_LENGTH, address)
                .sequence(SEQUENCE_DEFINITION, DATA_LENGTH, data)
                .mark(BIT_MARK)
                .build();
    }
};

class JVC {
private:
    static const int FREQUENCY = 38000; // T = 26.316 us
    static const int HDR_MARK = 304;
    static const int HDR_SPACE = 152;
    static const int BIT_MARK = 23;
    static const int ONE_SPACE = 61;
    static const int ZERO_SPACE = 21;

    static IRExpressionBuilder::SequenceDefinition* SEQUENCE_DEFINITION;

public:
    static void initialize() {
        SEQUENCE_DEFINITION = new IRExpressionBuilder::SimpleSequence(
                BIT_MARK, ONE_SPACE, BIT_MARK, ZERO_SPACE
        );
    }

    static IrCommand buildJVC(int bitCount, int data, bool repeat) {
        initialize();

        IRExpressionBuilder builder = IRExpressionBuilder::irCommandBuilder(FREQUENCY);
        if (!repeat)
            builder.pair(HDR_MARK, HDR_SPACE);

        return builder.sequence(SEQUENCE_DEFINITION, bitCount, data << (64 - bitCount))
                .mark(BIT_MARK)
                .build();
    }
};

class Pronto {
public:
    static IrCommand buildPronto(const std::string& protoText) {
        std::istringstream iss(protoText);
        std::string part;
        std::vector<int> protoSequence;

        while (std::getline(iss, part, ' ')) {
            protoSequence.push_back(std::stoi(part, nullptr, 16));
        }

        return buildPronto(protoSequence);
    }

    static IrCommand buildPronto(const std::vector<int>& protoSequence) {
        int frequency = static_cast<int>(1000000 / (protoSequence[1] * 0.241246));

        IRExpressionBuilder builder = IRExpressionBuilder::irCommandBuilder(frequency);

        for (size_t i = 4; i < protoSequence.size(); i += 2) {
            builder.pair(protoSequence[i], protoSequence[i + 1]);
        }

        return builder.build();
    }
};
