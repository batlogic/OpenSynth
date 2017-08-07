/*
  ==============================================================================

    Module.cpp
    Created: 25 Apr 2017 2:00:10pm
    Author:  Alexander Heemann

  ==============================================================================
*/

#include "Module.h"
#include "ModulationMatrix.h"

void Module::writeModulationValue()
{
    modulationMatrix->setValueForSourceID(ID, currentValue);
}
