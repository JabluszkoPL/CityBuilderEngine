#include "MapViewer.hpp"

#include "viewer/MapScene.hpp"



MapViewer::MapViewer(Map& map, QWidget* parent) :
    QGraphicsView(parent)
{
    setScene(new MapScene(map));
}



void MapViewer::buildingRequest(const StaticElementInformation* elementConf)
{
    static_cast<MapScene*>(scene())->requestBuildingPositioning(elementConf);
}
