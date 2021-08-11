#ifndef PANGUWGENERATOR_H
#define PANGUWGENERATOR_H

#include "PanguVGenerator.h"

class PanguWGenerator : public PanguVGenerator
{
public:
    PanguWGenerator();

    /**@brief:generator disk info from lshw*/
    virtual void getDiskInfoFromLshw();
};

#endif // PANGUWGENERATOR_H
