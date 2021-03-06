#include "MapScene.hpp"

#include "engine/element/dynamic/AbstractDynamicMapElement.hpp"
#include "engine/element/static/AbstractStaticMapElement.hpp"
#include "engine/map/Map.hpp"
#include "engine/processing/TimeCycleProcessor.hpp"
#include "global/conf/DynamicElementInformation.hpp"
#include "global/conf/StaticElementInformation.hpp"
#include "viewer/DynamicElement.hpp"
#include "viewer/SelectionElement.hpp"
#include "viewer/StaticElement.hpp"
#include "viewer/Tile.hpp"

const QSizeF BASE_TILE_SIZE(58, 30);



MapScene::MapScene(const Map& map) :
    QGraphicsScene(),
    map(map),
    tileList(),
    dynamicElementList(),
    selectionElement(new SelectionElement(BASE_TILE_SIZE))
{
    setBackgroundBrush(QBrush(Qt::black));

    // Load the grass image.
    QPixmap grassImage("assets/img/grass.png");

    // Create the tiles and their graphics item.
    int line(0);
    int column(0);
    const QSize& mapSize(map.getSize());
    while (line < mapSize.height()) {
        // NOTE: Because we divide by 2 and casting as integer, we deliberately remove floating precision. However, the
        // adjustment needs to be 1 higher when "mapSize.width() - line" become negative. This is because -0.5 is cast
        // to 0 insted of 1.
        int adjust(line > mapSize.width() ? 1 : 2);
        while (column < (mapSize.width() - line + adjust) / 2) {
            Tile* tile(new Tile(MapCoordinates(column, line + column), BASE_TILE_SIZE));
            tile->pushStaticElement(new StaticElement(BASE_TILE_SIZE, MapSize(1), grassImage));

            addItem(tile);
            tileList.append(tile);
            connect(tile, &Tile::isCurrentTile, this, &MapScene::currentTileChanged);

            ++column;
        }
        ++line;
        column = -line / 2;
    }

    // Attach the selection element.
    addItem(selectionElement);

    // Load existing elements.
    for (auto element: map.getElements()) {
        auto staticElement(dynamic_cast<AbstractStaticMapElement*>(element));
        if (staticElement) {
            registerNewStaticElement(staticElement);
        }
    }

    connect(this, &MapScene::buildingCreationRequested, &map, &Map::createStaticElement);
    connect(&map, &Map::staticElementCreated, this, &MapScene::registerNewStaticElement);
    connect(&map, &Map::dynamicElementCreated, this, &MapScene::registerNewDynamicElement);
    connect(map.getProcessor(), &TimeCycleProcessor::processFinished, this, &MapScene::refresh);
}



void MapScene::requestBuildingPositioning(const StaticElementInformation* elementConf)
{
    selectionElement->setBuildingType(elementConf);
}



void MapScene::requestBuildingCreation(const StaticElementInformation* elementConf, const MapArea& area)
{
    emit buildingCreationRequested(elementConf, area);
}



void MapScene::registerNewStaticElement(const AbstractStaticMapElement* element)
{
    Tile* tile(getTileAt(element->getArea().getLeft()));
    addStaticElementBuilding(tile, element->getConf()->getSize(), element->getConf()->getImage());
}



void MapScene::registerNewDynamicElement(const AbstractDynamicMapElement* element)
{
    DynamicElement* graphicsItem(new DynamicElement(BASE_TILE_SIZE, element, element->getConf()->getImage()));
    dynamicElementList.append(graphicsItem);

    Tile* tile(getTileAt(element->getCurrentLocation().getRounded()));
    tile->registerDynamicElement(graphicsItem);
}



void MapScene::refresh()
{
    // Refresh all the dynamic elements.
    auto iterator(dynamicElementList.begin());
    while (iterator != dynamicElementList.end()) {
        auto element(*iterator);
        if (element->stillExists()) {
            const MapCoordinates& previousTileLocation(static_cast<Tile*>(element->parentItem())->getCoordinates());
            MapCoordinates newTileLocation(element->getCoordinates().getRounded());
            if (newTileLocation != previousTileLocation) {
                // Dynamic element just switch to another tile.
                getTileAt(previousTileLocation)->unregisterDynamicElement(element);
                getTileAt(newTileLocation)->registerDynamicElement(element);
            }

            element->refresh();
            ++iterator;
        } else {
            removeItem(element);
            delete element;
            iterator = dynamicElementList.erase(iterator);
        }
    }

    // Refresh the selection element.
    if (selectionElement->isVisible()) {
        refreshSelectionElement();
    }
}



Tile* MapScene::getTileAt(const MapCoordinates& location)
{
    for (auto tile : tileList) {
        if (tile->getCoordinates() == location) {
            return tile;
        }
    }

    return nullptr;
}



void MapScene::addStaticElementBuilding(Tile* tile, const MapSize& elementSize, const QPixmap& elementImage)
{
    tile->pushStaticElement(new StaticElement(BASE_TILE_SIZE, elementSize, elementImage));

    if (elementSize.getValue() > 1) {
        MapArea area(tile->getCoordinates(), elementSize);
        auto left(area.getLeft());
        auto right(area.getRight());
        auto current(left.getEast());

        while (current.getY() <= right.getY()) {
            while (current.getX() <= right.getX()) {
                getTileAt(current)->setVisible(false);
                current = current.getEast();
            }
            current.setX(left.getX());
            current = current.getSouth();
        }
    }
}



void MapScene::refreshSelectionElement()
{
    if (map.isFreeArea(selectionElement->getCoveredArea())) {
        selectionElement->setGood();
    } else {
        selectionElement->setBad();
    }
}



void MapScene::currentTileChanged(Tile* currentTile)
{
    if (selectionElement->isVisible()) {
        selectionElement->attachToTile(*currentTile);
        refreshSelectionElement();
    }
}
