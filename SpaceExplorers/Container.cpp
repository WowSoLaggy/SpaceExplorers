#include "stdafx.h"
#include "Container.h"

#include "Container_events.h"
#include "Object.h"
#include "Prototypes.h"

#include <LaggySdk/Contracts.h>


void Container::checkIndex(int i_index) const
{
  CONTRACT_EXPECT(0 <= i_index);
  CONTRACT_EXPECT(i_index < d_size);
}


void Container::setSize(int i_size)
{
  d_size = i_size;
  
  if (d_items.size() != d_size)
    d_items.resize(d_size);
}

int Container::getSize() const
{
  return d_size;
}


void Container::resetAllItems()
{
  for (int i = 0; i < d_size; ++i)
    resetItem(i);
}

void Container::resetItem(int i_index)
{
  checkIndex(i_index);

  d_items.at(i_index) = nullptr;
  notify(SlotChangedEvent(i_index));
}

void Container::setItem(int i_index, ObjectPtr i_object)
{
  checkIndex(i_index);

  d_items.at(i_index) = i_object;
  notify(SlotChangedEvent(i_index));
}

ObjectPtr Container::getItem(int i_index) const
{
  checkIndex(i_index);

  return d_items.at(i_index);
}


std::optional<int> Container::getFreeSlot() const
{
  for (int i = 0; i < d_size; ++i)
  {
    if (!d_items[i])
      return i;
  }

  return std::nullopt;
}

std::optional<int> Container::getObjectIndex(ObjectPtr i_object) const
{
  for (int i = 0; i < d_size; ++i)
  {
    if (d_items[i] && d_items[i]->canBeStackedWith(i_object))
      return i;
  }

  return std::nullopt;
}


bool Container::tryAddObject(ObjectPtr i_object)
{
  std::optional<int> index;
  if (i_object->getPrototype().isStackable)
  {
    if (index = getObjectIndex(i_object))
    {
      getItem(*index)->addQuantity(i_object->getQuantity());
      return true;
    }
  }

  index = getFreeSlot();
  if (!index)
  {
    // No free space!
    return false;
  }

  setItem(*index, i_object);
  return true;
}
