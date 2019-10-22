#pragma once

#include "Gases.h"

#include <iostream>
#include <unordered_map>


class Atmosphere
{
  // TODO: ae Change this to 'prviate'
public:
  // 100k of Oxygen stands for 100 kPa - standard atmospheric pressure
  std::unordered_map<Gas, int> d_gases;

public:
  void writeTo(std::ostream& io_stream) const;
  void readFrom(std::istream& io_stream);
};
