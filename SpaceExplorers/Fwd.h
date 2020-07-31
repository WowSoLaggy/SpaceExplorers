#pragma once


class IApp;
class Game;

class Button;
class Inventory;
class Label;
class Panel;
class BuildProgressBar;

struct ExternalSettings;
struct InternalSettings;

struct StructurePrototype;
struct ObjectPrototype;
struct Receipt;

class Camera;
class World;
class Tile;
class Avatar;
class Object;
using ObjectPtr = std::shared_ptr<Object>;
class Structure;
using StructurePtr = std::shared_ptr<Structure>;
class Door;
using DoorPtr = std::shared_ptr<Door>;
class Thing;
using ThingPtr = std::shared_ptr<Thing>;

class Container;
struct BuildContext;
using BuildContextPtr = std::shared_ptr<BuildContext>;

class BehaviorModelBase;
class DoorModel;
class ContainerModel;
class PowerSourceModel;
class PowerLoadModel;
