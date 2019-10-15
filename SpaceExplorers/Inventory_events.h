#pragma once

#include <LaggySdk/IEvent.h>


class InventoryItemClickedEvent : public Sdk::IEvent
{
public:
  InventoryItemClickedEvent(ObjectPtr i_item)
    : d_item(i_item)
  {
  }

  ObjectPtr getItem() const { return d_item; }

private:
  ObjectPtr d_item;
};
