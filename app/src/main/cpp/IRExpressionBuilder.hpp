#ifndef ROOMBA_VIRTUAL_WALL_IREXPRESSIONBUILDER_HPP
#define ROOMBA_VIRTUAL_WALL_IREXPRESSIONBUILDER_HPP

#include <utility>
#include <vector>

class IrCommand {
private:
    int frequency;
    std::vector<int> pattern;

public:
    IrCommand(int frequency, std::vector<int> pattern);

};

class IRExpressionBuilder {
public:
    static const long TOP_BIT_32;
    static const long TOP_BIT_64;

private:
    int frequency;
    std::vector<int> buffer;
    bool lastMark;

public:
    static IRExpressionBuilder irCommandBuilder(int frequency);
    explicit IRExpressionBuilder(int frequencyKHz);

    IRExpressionBuilder& mark(int interval);
    IRExpressionBuilder& space(int interval);
    IRExpressionBuilder& pair(int on, int off);
    IRExpressionBuilder& reversePair(int off, int on);

public:
    class SequenceDefinition {
    public:
        virtual void one(IRExpressionBuilder& builder, int index) = 0;
        virtual void zero(IRExpressionBuilder& builder, int index) = 0;
    };

    IRExpressionBuilder& appendSymbol(bool mark, int interval);

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

    IRExpressionBuilder delay(int ms);
    IRExpressionBuilder& sequence(SequenceDefinition* definition, int length, int data);
    IRExpressionBuilder& sequence(SequenceDefinition* definition, int length, long data);
    IRExpressionBuilder& sequence(SequenceDefinition* definition, long topBit, int length, long data);

    IrCommand build();
    std::vector<int> buildSequence();
    int getFrequency() const;
    std::vector<int> getBuffer();

    static SimpleSequence* simpleSequence(int oneMark, int oneSpace, int zeroMark, int zeroSpace);
    static std::vector<int> buildRawSequence(std::vector<int> buffer);
    template <typename Iterable>
    static std::vector<int> buildRawSequence(const Iterable& dataStream);

};

#endif