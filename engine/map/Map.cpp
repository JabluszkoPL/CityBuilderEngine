#include "Map.hpp"

#include "engine/element/building/MaintenanceBuilding.hpp"
#include "engine/element/building/Road.hpp"
#include "engine/element/character/RandomWalker.hpp"
#include "engine/element/AbstractStaticMapElement.hpp"





Map::Map(const QSize& size) :
    size(size),
    roadGraph(),
    processor(),
    staticElementList(),
    dynamicElementList()
{

}





const QSize& Map::getSize() const
{
    return size;
}





const QList<AbstractStaticMapElement*>& Map::getStaticElementList() const
{
    return staticElementList;
}





bool Map::isValidCoordinates(const MapCoordinates& coordinates) const
{
    int sum(coordinates.getY() + coordinates.getX());
    int diff(coordinates.getY() - coordinates.getX());
    return diff >= 0 && diff < size.height()
        && sum >= 0 && sum < size.width();
}





bool Map::isValidArea(const MapArea& area) const
{
    return isValidCoordinates(area.getLeft())
        && isValidCoordinates(area.getRight())
        && isValidCoordinates(area.getTop())
        && isValidCoordinates(area.getBottom());
}





bool Map::isFreeCoordinates(const MapCoordinates& coordinates) const
{
    for (auto element : staticElementList)
    {
        if (element->getArea().isInside(coordinates))
        {
            return false;
        }
    }

    return true;
}





bool Map::isFreeArea(const MapArea& area) const
{
    auto left(area.getLeft());
    auto right(area.getRight());

    MapCoordinates coordinates(left);
    while (coordinates.getY() <= right.getY())
    {
        while (coordinates.getX() <= right.getX())
        {
            if (!isFreeCoordinates(coordinates))
            {
                return false;
            }
            coordinates = coordinates.getEast();
        }
        coordinates = MapCoordinates(left.getX(), coordinates.getY() + 1);
    }

    return true;
}





RoadGraph& Map::getRoadGraph()
{
    return roadGraph;
}





TimeCycleProcessor& Map::getProcessor()
{
    return processor;
}





void Map::createStaticElement(StaticElementType type, const MapArea& area)
{
    if (!isFreeArea(area))
    {
        throw UnexpectedException("Try to create a motionless element on an occupyed area " + area.toString());
    }

    AbstractStaticMapElement* element;
    const RoadGraphNode* entryPointNode;
    switch (type)
    {
        case StaticElementType::Maintenance:
            entryPointNode = roadGraph.fetchNodeArround(area);
            element = new MaintenanceBuilding(*this, area, entryPointNode ? entryPointNode->getCoordinates() : MapCoordinates());
            processor.registerProcessable(static_cast<AbstractProcessableBuilding*>(element));
            break;

        case StaticElementType::Road:
            if (area.getSize().getValue() > 1)
            {
                throw UnexpectedException("Try to create a road on an area bigger than 1: " + area.toString());
            }
            element = new Road(roadGraph.createNode(area.getLeft()));
            break;
    }

    staticElementList.append(element);
}





void Map::createDynamicElement(Map::DynamicElementType type, const MapCoordinates& initialLocation)
{
    AbstractDynamicMapElement* element;
    switch (type)
    {
        case DynamicElementType::RandomWalker:
            element = new RandomWalker(roadGraph, initialLocation);
            break;
    }

    processor.registerProcessable(element);
    dynamicElementList.append(element);
}