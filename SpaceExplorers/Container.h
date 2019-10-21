#pragma once

#include "Fwd.h"

#include <LaggyDx/LaggyDxFwd.h>
#include <LaggySdk/EventHandler.h>


class Container : public Sdk::EventHandler
{
public:
  void setSize(int i_size);
  int getSize() const;

  void resetAllItems();
  void resetItem(int i_index);
  void setItem(int i_index, ObjectPtr i_object);
  ObjectPtr getItem(int i_index) const;

  std::optional<int> getFreeSlot() const;
  std::optional<int> getObjectIndex(ObjectPtr i_object) const;
  std::optional<int> getIndexToAddTo(ObjectPtr i_object) const;

  bool tryAddObject(ObjectPtr i_object);

private:
  int d_size;
  std::vector<ObjectPtr> d_items;

  void checkIndex(int i_index) const;

public:
  void writeTo(std::ostream& io_stream) const;
  void readFrom(std::istream& io_stream, Dx::IResourceController& i_resouceContoller, World& i_world);
};
