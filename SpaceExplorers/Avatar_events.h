#pragma once

#include <LaggySdk/IEvent.h>
#include <LaggySdk/Vector.h>


class AvatarBuildEvent : public Sdk::IEvent
{
public:
  double getProgress() const { return d_progress; }
  const Sdk::Vector2I& getPosition() const { return d_position; }

protected:
  AvatarBuildEvent(double i_progress, Sdk::Vector2I i_position)
    : d_progress(i_progress)
    , d_position(std::move(i_position))
  {
  }

private:
  const double d_progress = 0;
  const Sdk::Vector2I d_position;
};


class AvatarStartBuildEvent : public AvatarBuildEvent
{
public:
  AvatarStartBuildEvent(double i_progress, Sdk::Vector2I i_position)
    : AvatarBuildEvent(i_progress, std::move(i_position))
  {
  }
};

class AvatarUpdateBuildEvent : public AvatarBuildEvent
{
public:
  AvatarUpdateBuildEvent(double i_progress, Sdk::Vector2I i_position)
    : AvatarBuildEvent(i_progress, std::move(i_position))
  {
  }
};

class AvatarStopBuildEvent : public AvatarBuildEvent
{
public:
  AvatarStopBuildEvent(double i_progress, Sdk::Vector2I i_position)
    : AvatarBuildEvent(i_progress, std::move(i_position))
  {
  }
};
