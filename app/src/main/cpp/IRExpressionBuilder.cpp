#include <utility>
#include <vector>

class IrCommand {
private:
    int frequency;
    std::vector<int> pattern;

public:
    IrCommand(int frequency, std::vector<int> pattern)
            : frequency(frequency), pattern(std::move(pattern)) {}
};



class IRExpressionBuilder {
public:
    static const long TOP_BIT_32 = 0x1L << 31;
    static const long TOP_BIT_64 = 0x1L << 63;

private:
    int frequency;
    std::vector<int> buffer;
    bool lastMark;

public:
    static IRExpressionBuilder irCommandBuilder(int frequency) {
        return IRExpressionBuilder(frequency);
    }

    explicit IRExpressionBuilder(int frequencyKHz)
            : frequency(frequencyKHz), lastMark(false) {}

    IRExpressionBuilder& appendSymbol(bool mark, int interval) {
        if (lastMark != mark) {
            buffer.push_back(interval);
            lastMark = mark;
        } else {
            buffer[buffer.size() - 1] += interval;
        }
        return *this;
    }

    IRExpressionBuilder& mark(int interval) {
        return appendSymbol(true, interval);
    }

    IRExpressionBuilder& space(int interval) {
        return appendSymbol(false, interval);
    }

    IRExpressionBuilder& pair(int on, int off) {
        return mark(on).space(off);
    }

    IRExpressionBuilder& reversePair(int off, int on) {
        return space(off).mark(on);
    }

    class SequenceDefinition {
    public:
        virtual void one(IRExpressionBuilder& builder, int index) = 0;
        virtual void zero(IRExpressionBuilder& builder, int index) = 0;
    };

    class SimpleSequence : public SequenceDefinition {
    private:
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

    IRExpressionBuilder delay(int ms) {
        return space(ms * frequency / 1000);
    }

    IRExpressionBuilder& sequence(SequenceDefinition* definition, int length, int data) {
        return sequence(definition, TOP_BIT_32, length, data);
    }

    IRExpressionBuilder& sequence(SequenceDefinition* definition, int length, long data) {
        return sequence(definition, TOP_BIT_64, length, data);
    }

    IRExpressionBuilder& sequence(SequenceDefinition* definition, long topBit, int length, long data) {
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

    IrCommand build() {
        return {frequency, buildSequence()};
    }

    std::vector<int> buildSequence() {
        return buffer;
    }

    int getFrequency() const {
        return frequency;
    }

    std::vector<int> getBuffer() {
        return buffer;
    }

    static SimpleSequence* simpleSequence(int oneMark, int oneSpace, int zeroMark, int zeroSpace) {
        return new SimpleSequence(oneMark, oneSpace, zeroMark, zeroSpace);
    }

    static std::vector<int> buildRawSequence(std::vector<int> buffer) {
        return buffer;
    }

    template <typename Iterable>
    static std::vector<int> buildRawSequence(const Iterable& dataStream) {
        return std::vector<int>(std::begin(dataStream), std::end(dataStream));
    }

};
