#pragma once

#include "Gases.h"

#include <iostream>
#include <unordered_map>


class Atmosphere
{
public:
  const std::unordered_map<Gas, int>& getGases() const { return d_gases; }

  bool hasGases() const;

  int getPressure() const;

  std::unordered_map<Gas, double> getGasesRatios() const;

  // Returns actual amount that was taken
  int tryTakeGas(Gas i_type, int i_amount);
  void giveGas(Gas i_type, int i_amount);
  void removeAllGases();

private:
  // 100k of Oxygen stands for 100 kPa - standard atmospheric pressure
  std::unordered_map<Gas, int> d_gases;

public:
  void writeTo(std::ostream& io_stream) const;
  void readFrom(std::istream& io_stream);
};
