#ifndef DATETIME_DAY_H
#define DATETIME_DAY_H

#include "../../Component.h"

class Day : public Component
{
public:
        explicit Day(unsigned int day) : Component(day) {}
};

#endif //DATETIME_DAY_H
