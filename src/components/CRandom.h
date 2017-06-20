#pragma once

#include "util.h"

#include <limits>

class CRandom : public TypedObject {
    PUBLISHED:
        CRandom(int seed);
        ~CRandom();
        int Next();
        int Next(int minValue, int maxValue);
        int Next(int maxValue);
        double NextDouble();
        
    private:
        int inext;
        int inextp;
        
        double Sample();
        double GetSampleForLargeRange();
        int InternalSample();
       
       
    TYPE_HANDLE(CRandom, TypedObject);
};