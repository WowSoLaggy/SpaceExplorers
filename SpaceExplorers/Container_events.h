#pragma once

#include <LaggySdk/IEvent.h>


class SlotChangedEvent : public Sdk::IEvent
{
public:
  SlotChangedEvent(int i_index)
    : d_index(i_index)
  {
  }

  int getIndex() const { return d_index; }

private:
  int d_index;
};
