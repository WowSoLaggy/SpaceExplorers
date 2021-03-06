#pragma once

#include "Gases.h"

#include <iostream>
#include <unordered_map>


class Atmosphere
{
public:
  const std::map<Gas, int>& getGases() const { return d_gases; }
  int getGasCount() const;
  std::set<Gas> getGasesTypes() const;
  std::map<Gas, double> getGasesRatios() const;
  int getGasAmount(Gas i_type) const;

  bool hasGases() const;

  int getPressure() const;

  // Returns actual amount that was taken
  int tryTakeGas(Gas i_type, int i_amount);
  void giveGas(Gas i_type, int i_amount);
  void removeAllGases();

private:
  // 100k of Oxygen stands for 100 kPa - standard atmospheric pressure
  std::map<Gas, int> d_gases;

public:
  void writeTo(std::ostream& io_stream) const;
  void readFrom(std::istream& io_stream);
};
