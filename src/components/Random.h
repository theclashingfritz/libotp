#pragma once

#include "util.h"

#include <limits>

class Random : public TypedObject {
    PUBLISHED:
        Random(int seed);
        ~Random();
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
       
       
    TYPE_HANDLE(Random, TypedObject);
};