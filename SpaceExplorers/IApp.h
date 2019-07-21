#pragma once


class IApp
{
public:
  virtual void stop() = 0;

  virtual int getClientWidth() = 0;
  virtual int getClientHeight() = 0;
};
