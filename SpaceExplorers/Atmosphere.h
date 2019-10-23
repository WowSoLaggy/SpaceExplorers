#pragma once

#include "Gases.h"

#include <iostream>
#include <unordered_map>


class Atmosphere
{
public:
  std::unordered_map<Gas, int>& getGases() { return d_gases; }
  const std::unordered_map<Gas, int>& getGases() const { return d_gases; }

  bool hasGases() const;

  int getPressure() const;

  // TODO: ae Change this to 'prviate'
public:
  // 100k of Oxygen stands for 100 kPa - standard atmospheric pressure
  std::unordered_map<Gas, int> d_gases;

public:
  void writeTo(std::ostream& io_stream) const;
  void readFrom(std::istream& io_stream);
};
