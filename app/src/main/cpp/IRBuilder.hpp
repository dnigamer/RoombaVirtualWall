
#ifndef ROOMBA_VIRTUAL_WALL_IRBUILDER_HPP
#define ROOMBA_VIRTUAL_WALL_IRBUILDER_HPP

#include "IRExpressionBuilder.hpp"
#include <vector>
#include <string>

class NEC {
public:
    static void initialize();
    static IrCommand buildNEC(int bitCount, int data);
};

class SONY {
public:
    static void initialize();
    static IrCommand buildSONY(int bitCount, long data);
};

class RC5 {
public:
    static void initialize();
    static IrCommand buildRC5(int bitCount, int data);
};

class RC6 {
public:
    static void initialize();
    static IrCommand buildRC6(int bitCount, long data);
};

class DISH {
public:
    static void initialize();
    static IrCommand buildDISH(int bitCount, int data);
};

class SHARP {
public:
    static void initialize();
    static IrCommand buildSHARP(int bitCount, int data);
};

class PANASONIC {
public:
    static void initialize();
    static IrCommand buildPANASONIC(int address, int data);
};

class JVC {
public:
    static void initialize();
    static IrCommand buildJVC(int bitCount, int data, bool repeat);
};

class Pronto {
public:
    static IrCommand buildPronto(const std::string& protoText);
    static IrCommand buildPronto(const std::vector<int>& protoSequence);
};

#endif
