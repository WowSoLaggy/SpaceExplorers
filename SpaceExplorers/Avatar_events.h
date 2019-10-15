#pragma once

#include "Fwd.h"

#include <LaggySdk/IEvent.h>
#include <LaggySdk/Vector.h>


class BuildEvent : public Sdk::IEvent
{
public:
  double getProgress() const { return d_progress; }
  const Sdk::Vector2I& getPosition() const { return d_position; }

protected:
  BuildEvent(double i_progress, Sdk::Vector2I i_position)
    : d_progress(i_progress)
    , d_position(std::move(i_position))
  {
  }

private:
  const double d_progress = 0;
  const Sdk::Vector2I d_position;
};


class StartBuildEvent : public BuildEvent
{
public:
  StartBuildEvent(double i_progress, Sdk::Vector2I i_position)
    : BuildEvent(i_progress, std::move(i_position))
  {
  }
};

class UpdateBuildEvent : public BuildEvent
{
public:
  UpdateBuildEvent(double i_progress, Sdk::Vector2I i_position)
    : BuildEvent(i_progress, std::move(i_position))
  {
  }
};

class StopBuildEvent : public BuildEvent
{
public:
  StopBuildEvent(double i_progress, Sdk::Vector2I i_position)
    : BuildEvent(i_progress, std::move(i_position))
  {
  }
};



class ContainerOpenedEvent : public Sdk::IEvent
{
public:
  ContainerOpenedEvent(Avatar& i_sender, Structure& i_structure)
    : d_sender(i_sender)
    , d_structure(i_structure)
  { }

  Avatar& getSender() const { return d_sender; }
  Structure& getStructure() const { return d_structure; }

private:
  Avatar& d_sender;
  Structure& d_structure;
};


class ContainerClosedEvent : public Sdk::IEvent
{
public:
  ContainerClosedEvent(Avatar& i_sender)
    : d_sender(i_sender)
  { }

  Avatar& getSender() const { return d_sender; }

private:
  Avatar& d_sender;
};
