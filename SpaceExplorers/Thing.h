#pragma once


class Thing
{
public:
  virtual ~Thing() = default;

  virtual bool isStructure() const { return false; }
  virtual bool isObject() const { return false; }
  virtual bool isAvatar() const { return false; }
};
