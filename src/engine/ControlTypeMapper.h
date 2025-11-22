#pragma once
#include <QString>
#include "model/ControlType.h"

class ControlTypeMapper
{
public:
    static ControlType map(const QString& rawtype);

private:
    static bool contains(const QString& raw, std::initializer_list<const char*> list);
};
