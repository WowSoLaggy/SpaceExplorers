#pragma once

#include <LaggySdk/IEvent.h>


class InventoryItemClickedEvent : public Sdk::IEvent
{
public:
  InventoryItemClickedEvent(Container& i_container, ObjectPtr i_item)
    : d_container(i_container)
    , d_item(i_item)
  {
  }

  Container& getContainer() const { return d_container; }
  ObjectPtr getItem() const { return d_item; }

private:
  Container& d_container;
  ObjectPtr d_item;
};
