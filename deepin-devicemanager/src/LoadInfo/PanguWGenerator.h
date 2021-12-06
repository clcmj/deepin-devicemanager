#ifndef PANGUWGENERATOR_H
#define PANGUWGENERATOR_H

#include "PanguVGenerator.h"

class PanguWGenerator : public PanguVGenerator
{
public:
    PanguWGenerator();

    /**@brief:generator disk info from lshw*/
    virtual void getDiskInfoFromLshw();

    virtual void generatorNetworkDevice();
};

#endif // PANGUWGENERATOR_H
