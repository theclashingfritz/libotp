#pragma once

#include <limits>

class Random {
    private:
        int inext;
        int inextp;
        
        double Sample();
        double GetSampleForLargeRange();
        int InternalSample();

    public:
        Random(int seed);
        ~Random();
        int Next();
        int Next(int minValue, int maxValue);
        int Next(int maxValue);
        double NextDouble();
};