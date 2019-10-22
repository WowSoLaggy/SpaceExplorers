#include "stdafx.h"
#include "Avatar.h"

#include "Avatar_events.h"
#include "BuildContext.h"
#include "ContainerModel.h"
#include "Inventory_events.h"
#include "Utils.h"
#include "World.h"

#include <LaggyDx/ImageDescription.h>
#include <LaggyDx/IResourceController.h>
#include <LaggyDx/ITextureResource.h>
#include <LaggySdk/Contracts.h>


Avatar::Avatar(
  Dx::IResourceController& i_resourceController,
  World& i_world, const ObjectPrototype& i_prototype)
  : Object(i_resourceController, i_world, i_prototype)
{
}


void Avatar::update(double i_dt)
{
  updateMovement(i_dt);
  if (isBuilding())
    updateBuild(i_dt);
  Object::update(i_dt);
}


void Avatar::moveLeft()
{
  d_moveXSign -= 1;
}

void Avatar::moveRight()
{
  d_moveXSign += 1;
}

void Avatar::moveUp()
{
  d_moveYSign -= 1;
}

void Avatar::moveDown()
{
  d_moveYSign += 1;
}


void Avatar::updateMovement(double i_dt)
{
  bool floatInSpace = !d_world.checkSupport(d_rect);
  if (!floatInSpace)
    d_curSpeed = { 0, 0 };

  if (!isInteracting())
  {
    auto canMove = [&](Sdk::RectI i_curRect, const Sdk::Vector2I& i_diff) -> bool
    {
      i_curRect.move(i_diff);
      i_curRect.shrink(2);

      return !d_world.checkCollision(i_curRect);
    };

    auto willFloat = [&](Sdk::RectI i_curRect, const Sdk::Vector2I& i_diff) -> bool
    {
      i_curRect.move(i_diff);
      return !d_world.checkSupport(i_curRect);
    };

    if (d_moveXSign != 0)
    {
      Sdk::Vector2I diff;
      diff.x = (int)(getMaxSpeed() * i_dt * d_moveXSign);
      
      if (!floatInSpace && canMove(d_rect, diff))
      {
        if (!isWalk() || !willFloat(d_rect, diff))
          d_curSpeed.x = diff.x;
      }
    }

    if (d_moveYSign != 0)
    {
      Sdk::Vector2I diff;
      diff.y = (int)(getMaxSpeed() * i_dt * d_moveYSign);
      
      if (!floatInSpace && canMove(d_rect, diff))
      {
        if (!isWalk() || !willFloat(d_rect, diff))
          d_curSpeed.y = diff.y;
      }
    }
  }

  if (d_curSpeed.x != 0 || d_curSpeed.y != 0)
    setPosition(getPosition() + d_curSpeed);

  if (d_moveXSign != 0 || d_moveYSign != 0)
  {
    updateMoveAnimation();
    d_moveXSign = 0;
    d_moveYSign = 0;
  }
}

void Avatar::updateMoveAnimation()
{
  if (d_moveXSign < 0)
    d_sprite.playAnimation("Left");
  else if (d_moveXSign > 0)
    d_sprite.playAnimation("Right");
  else if (d_moveYSign < 0)
    d_sprite.playAnimation("Up");
  else if (d_moveYSign > 0)
    d_sprite.playAnimation("Down");
}


bool Avatar::isInteracting() const
{
  return isBuilding() || isInspectingContainer();
}


void Avatar::startInspectingContainer(StructurePtr i_structure)
{
  CONTRACT_EXPECT(i_structure);

  i_structure->interact(Action::Open);

  notify(ContainerOpenedEvent(*this, *i_structure));
  d_inspectedContainer = i_structure;
}

void Avatar::stopInspectingContainer()
{
  if (!isInspectingContainer())
    return;

  if (d_inspectedContainer)
    d_inspectedContainer->interact(Action::Close);

  notify(ContainerClosedEvent(*this));
  d_inspectedContainer.reset();
}


void Avatar::interact(Action i_action, ThingPtr io_object,
                      ObjectPtr i_tool, const Sdk::Vector2I& i_where)
{
  if (isInteracting())
    return;

  CONTRACT_EXPECT(!i_tool || !i_tool->isAvatar());

  if (i_action == Action::Default)
  {
    if (i_tool && i_tool->getPrototype().hasReceipts())
    {
      interact(Action::Build, io_object, i_tool, i_where);
      return;
    }

    if (!io_object || i_tool)
      return;

    if (io_object->isStructure())
    {
      auto structure = std::dynamic_pointer_cast<Structure>(io_object);
      CONTRACT_ASSERT(structure);

      if ((structure->getCoords() - d_position).lengthSq() <= d_interactionDistSq)
      {
        if (structure->isContainer())
        {
          if (isInspectingContainer())
            interact(Action::Close, io_object, i_tool, i_where);
          else
            interact(Action::Open, io_object, i_tool, i_where);
        }
        else
          structure->interact(i_action);
      }
    }
  }
  else if (i_action == Action::Open)
  {
    auto structure = std::dynamic_pointer_cast<Structure>(io_object);
    CONTRACT_ASSERT(structure);

    if ((structure->getCoords() - d_position).lengthSq() > d_interactionDistSq)
      return;

    startInspectingContainer(structure);
  }
  else if (i_action == Action::Close)
  {
    stopInspectingContainer();
  }
  else if (i_action == Action::Pickup)
  {
    if (!io_object || !io_object->isObject())
      return;

    auto obj = std::dynamic_pointer_cast<Object>(io_object);
    CONTRACT_ASSERT(obj);

    if ((obj->getPosition() - d_position).lengthSq() > d_interactionDistSq)
      return;
    
    if (d_inventory.tryAddObject(obj))
      d_world.deleteObject(obj);
  }
  else if (i_action == Action::Drop)
  {
    if ((i_where - d_position).lengthSq() > d_interactionDistSq)
    {
      // Can't throw too far away
      return;
    }

    const auto& proto = i_tool->getPrototype();

    {
      const auto& texture = d_resourceController.getTextureResource(proto.textureFileName);
      const Sdk::Vector2I size { texture.getDescription().width, texture.getDescription().height };
      const auto topLeft = i_where - size / 2;
      Sdk::RectI rect = { topLeft, topLeft + size };
      rect.shrink(4);
      if (d_world.checkCollision(rect))
      {
        // No place for the object
        return;
      }
    }

    auto newObj = d_world.createObjectAt(proto, i_where, i_tool->getName());
    newObj->setQuantity(1);

    if (i_tool->getQuantity() <= 1)
    {
      auto indexOpt = d_inventory.getObjectIndex(i_tool);
      CONTRACT_ASSERT(indexOpt);
      d_inventory.resetItem(*indexOpt);
    }
    else
      i_tool->addQuantity(-1);
  }
  else if (i_action == Action::Build)
  {
    if (!i_tool)
      return;
    if (io_object && !io_object->isStructure())
      return;
    if ((i_where - d_position).lengthSq() > d_interactionDistSq)
      return;

    StructurePtr object = std::dynamic_pointer_cast<Structure>(io_object);

    const StructurePrototype* inputProto = object ? &object->getPrototype() : nullptr;
    const auto& receipts = i_tool->getPrototype().receipts;
    auto it = std::find_if(receipts.cbegin(), receipts.cend(), [&](const auto& i_receipt) {
      return i_receipt.input == inputProto;
    });
    if (it == receipts.cend())
      return;

    const auto& receipt = *it;
    const auto* outputProto = receipt.output;
    const auto tileCoords = worldToTile(i_where);

    auto* tile = d_world.getTile(tileCoords);
    if (tile)
    {
      const bool sameLayer = inputProto && outputProto && inputProto->layer == outputProto->layer;
      const bool outputLayerOccupied = outputProto && tile->hasStructureOnLayer(outputProto->layer);
      if (!sameLayer && outputLayerOccupied)
      {
        // Output layer shall not be occupied
        return;
      }
    }

    if (!d_world.checkSupport(tileCoords))
      return;

    startBuilding(object, i_tool, receipt, i_where);
  }
}


void Avatar::startBuilding(StructurePtr i_structure, ObjectPtr i_tool,
                           const Receipt& i_receipt, const Sdk::Vector2I& i_where)
{
  d_buildContext = std::make_shared<BuildContext>(i_structure, i_tool, i_receipt, i_where);

  if (i_structure && d_buildContext->takesTime())
  {
    const double progress = i_structure->getBuildTime() / i_receipt.time;
    notify(StartBuildEvent{ progress, i_where });
  }
}

void Avatar::stopBuilding()
{
  CONTRACT_EXPECT(d_buildContext);

  if (d_buildContext->object && d_buildContext->takesTime())
  {
    const double progress = d_buildContext->object->getBuildTime() / d_buildContext->receipt.time;
    notify(StopBuildEvent{ progress, d_buildContext->coords });
  }

  d_buildContext.reset();
}

void Avatar::updateBuild(double i_dt)
{
  CONTRACT_EXPECT(isBuilding());

  const auto& object = d_buildContext->object;
  const auto& receipt = d_buildContext->receipt;
  const auto& coords = d_buildContext->coords;

  if (object)
  {
    object->addBuildTime(i_dt);

    if (d_buildContext->takesTime())
      notify(UpdateBuildEvent{ object->getBuildTime() / receipt.time, coords });

    if (object->getBuildTime() < receipt.time)
      return;
  }

  finishBuild();
}

void Avatar::finishBuild()
{
  CONTRACT_EXPECT(isBuilding());

  const auto& object = d_buildContext->object;
  const auto& tool = d_buildContext->tool;
  const auto& receipt = d_buildContext->receipt;
  const auto& coords = d_buildContext->coords;
  const auto tileCoords = worldToTile(coords);


  const bool sameLayer = receipt.input && receipt.output && receipt.input->layer == receipt.output->layer;
  const bool dismantling = receipt.input && !receipt.output;
  if (sameLayer || dismantling)
  {
    // If input and output are on the same layer - remove the existing structure on this layer

    auto* tile = d_world.getTile(tileCoords);
    CONTRACT_ASSERT(tile);

    tile->removeStructure(receipt.input->layer);
  }


  if (receipt.output)
    d_world.createStructureAt(*receipt.output, tileCoords);


  if (receipt.result)
    d_world.createObjectAt(*receipt.result, coords, receipt.result->name);


  if (tool->getPrototype().consumedOnBuild)
  {
    if (tool->getQuantity() <= 1)
    {
      auto indexOpt = d_inventory.getObjectIndex(tool);
      CONTRACT_ASSERT(indexOpt);
      d_inventory.resetItem(*indexOpt);
    }
    else
      tool->addQuantity(-1);
  }

  stopBuilding();
}


int Avatar::getMaxSpeed()
{
  return isRun() ? RunSpeed : WalkSpeed;
}

void Avatar::setRun()
{
  d_walkMode = WalkMode::Run;
}

void Avatar::setWalk()
{
  d_walkMode = WalkMode::Walk;
}

bool Avatar::isRun() const
{
  return d_walkMode == WalkMode::Run;
}

bool Avatar::isWalk() const
{
  return d_walkMode == WalkMode::Walk;
}


void Avatar::processEvent(const Sdk::IEvent& i_event)
{
  if (const auto* itemClickedEvent = dynamic_cast<const InventoryItemClickedEvent*>(&i_event))
  {
    if (!d_inspectedContainer)
      return;

    auto& srcContainer = itemClickedEvent->getContainer();
    auto item = itemClickedEvent->getItem();
    CONTRACT_EXPECT(item);

    auto getInspectedContainer = [&]() -> Container&
    {
      auto containerModel = d_inspectedContainer->getContainerModel();
      CONTRACT_ASSERT(containerModel);
      return containerModel->getContainer();
    };

    auto& destContainer = (&d_inventory != &srcContainer) ? d_inventory : getInspectedContainer();
    
    if (destContainer.tryAddObject(item))
    {
      const auto idx = srcContainer.getObjectIndex(item);
      CONTRACT_ASSERT(idx);
      srcContainer.resetItem(*idx);
    }
  }
}
