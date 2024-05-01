#ifndef ROOMBA_VIRTUAL_WALL_IREXPRESSIONBUILDER_HPP
#define ROOMBA_VIRTUAL_WALL_IREXPRESSIONBUILDER_HPP

#include <vector>

class IRExpressionBuilder {
public:
    class SequenceDefinition {
    public:
        virtual void one(IRExpressionBuilder& builder, int index) = 0;
        virtual void zero(IRExpressionBuilder& builder, int index) = 0;
    };

    class SimpleSequence : public SequenceDefinition {
    public:
        SimpleSequence(int oneMark, int oneSpace, int zeroMark, int zeroSpace);
        void one(IRExpressionBuilder& builder, int index) override;
        void zero(IRExpressionBuilder& builder, int index) override;
    };

    IRExpressionBuilder();
    static IRExpressionBuilder irCommandBuilder(int frequency);
    int getFrequency();
    std::vector<int> getBuffer();
    static IRExpressionBuilder mark(int duration);
    IRExpressionBuilder space(int duration);
    IRExpressionBuilder pair(int on, int off);
    IRExpressionBuilder reversePair(int on, int off);
    IRExpressionBuilder delay(int ms);
    IRExpressionBuilder sequence(SequenceDefinition* definition, int length, int data);
    IRExpressionBuilder sequence(SequenceDefinition* definition, int length, long data);
    IRExpressionBuilder sequence(SequenceDefinition* definition, long topBit, int length, long data);
    std::vector<int> buildSequence();
    static std::vector<int> buildRawSequence(std::initializer_list<int> rawData);
    static std::vector<int> buildRawSequence(const std::vector<int>& buffer);
    template <typename Iterable>
    static std::vector<int> buildRawSequence(const Iterable& dataStream);

private:
    long TOP_BIT_32 = 0x1L; // 32-bit
    long TOP_BIT_64 = 0x1L; // 64-bit
    int frequency;
    bool lastMark;
    std::vector<int> buffer;

    IRExpressionBuilder(int frequencyKHz);
    IRExpressionBuilder appendSymbol(bool mark, int duration);
};

#endif