#pragma once

#include "Behaviors.h"
#include "Fwd.h"
#include "Layers.h"

#include <string>


enum class AttachmentPosition : std::int32_t
{
  Up = 0,
  Left,
  Down,
  Right,
  Center,
};

enum class AttachmentGeneralPosition : std::int32_t
{
  None = 0,
  Center,
  Side,
};

struct StructurePrototype
{
  std::string name = "";

  std::string textureFileName = "";
  Behavior behavior = Behavior::Default;
  Layer layer = Layer::None;
  bool isPassable = true;
  bool hasAtmosphere = true;
  bool isLeak = false;
  bool isSupport = true;
  int containerSize = 0;
  int maxPower = 0;
  int requiredPower = 0;
  bool isAttachment = false;
  AttachmentGeneralPosition attachedTo = AttachmentGeneralPosition::None;
  std::set<AttachmentPosition> attachments;
};


struct Receipt
{
  const StructurePrototype* input = nullptr;
  const StructurePrototype* output = nullptr;
  const double time = 0;
  const std::string resultName;
  const ObjectPrototype* result = nullptr;
};

struct ObjectPrototype
{
  std::string name = "";

  std::string textureFileName = "";
  bool isStackable = false;

  bool hasReceipts() const { return !receipts.empty(); }
  std::vector<Receipt> receipts;
  bool consumedOnBuild = false;
};
