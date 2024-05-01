#include <vector>
#include "IRBuilder.hpp"

class IRExpressionBuilder {
    long TOP_BIT_32 = 0x1L; // 32-bit
    long TOP_BIT_64 = 0x1L; // 64-bit

    int frequency;
    bool lastMark;
    std::vector<int> buffer;

    class SequenceDefinition {
    public:
        virtual void one(IRExpressionBuilder& builder, int index) = 0;
        virtual void zero(IRExpressionBuilder& builder, int index) = 0;
    };

    class SimpleSequence : public SequenceDefinition {
        int oneMark, oneSpace, zeroMark, zeroSpace;

    public:
        SimpleSequence(int oneMark, int oneSpace, int zeroMark, int zeroSpace)
                : oneMark(oneMark), oneSpace(oneSpace), zeroMark(zeroMark), zeroSpace(zeroSpace) {}

        void one(IRExpressionBuilder& builder, int index) override {
            builder.pair(oneMark, oneSpace);
        }

        void zero(IRExpressionBuilder& builder, int index) override {
            builder.pair(zeroMark, zeroSpace);
        }
    };


public:
    IRExpressionBuilder() : frequency(), lastMark(false) {}

    static IRExpressionBuilder irCommandBuilder(int frequency) {
        return IRExpressionBuilder(frequency);
    }

    int getFrequency() {
        return frequency;
    }

    std::vector<int> getBuffer() {
        return buffer;
    }

    IRExpressionBuilder mark(int duration) {
        return appendSymbol(true, duration);
    }

    IRExpressionBuilder space(int duration) {
        return appendSymbol(false, duration);
    }

    IRExpressionBuilder pair(int on, int off) {
        return mark(on).space(off);
    }

    IRExpressionBuilder reversePair(int on, int off) {
        return space(on).mark(off);
    }

    IRExpressionBuilder delay(int ms) {
        return space(ms * frequency / 1000);
    }

    IRExpressionBuilder sequence(SequenceDefinition* definition, int length, int data) {
        return sequence(definition, TOP_BIT_32, length, data);
    }

    IRExpressionBuilder sequence(SequenceDefinition* definition, int length, long data) {
        return sequence(definition, TOP_BIT_64, length, data);
    }

    IRExpressionBuilder sequence(SequenceDefinition* definition, long topBit, int length, long data) {
        for (int index = 0; index < length; index++) {
            if ((data & topBit) != 0) {
                definition->one(*this, index);
            } else {
                definition->zero(*this, index);
            }
            data <<= 1;
        }
        return *this;
    }

    // TODO: IRBuilder::build() is not implemented

    std::vector<int> buildSequence() {
        return buildRawSequence(this->buffer);
    }

    static std::vector<int> buildRawSequence(std::initializer_list<int> rawData) {
        return std::vector<int>(rawData);
    }

    static std::vector<int> buildRawSequence(const std::vector<int>& buffer) {
        return buffer;
    }

    template <typename Iterable>
    static std::vector<int> buildRawSequence(const Iterable& dataStream) {
        return std::vector<int>(std::begin(dataStream), std::end(dataStream));
    }

private:
    IRExpressionBuilder(int frequencyKHz) {
        this->frequency = frequencyKHz;
        this->buffer = std::vector<int>();
        this->lastMark = false;
    }

    IRExpressionBuilder appendSymbol(bool mark, int duration) {
        if (lastMark != mark) {
            buffer.push_back(duration);
            lastMark = mark;
        } else {
            buffer[buffer.size() - 1] += duration;
        }
        return *this;
    }
};

