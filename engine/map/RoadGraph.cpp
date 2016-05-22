#include "RoadGraph.hpp"

#include "engine/map/RoadGraphNode.hpp"





RoadGraph::RoadGraph() :
    nodeList()
{

}





RoadGraphNode* RoadGraph::fetchNodeAt(const MapCoordinates& coordinates) const
{
    for (int i(0), iEnd(nodeList.length()); i < iEnd; ++i)
    {
        if (nodeList.at(i)->getCoordinates() == coordinates)
        {
            return nodeList.at(i);
        }
    }

    return nullptr;
}





const RoadGraphNode* RoadGraph::fetchNodeArround(const MapArea& area) const
{
    // Fetch a road node arround the building starting at the coordinates at north of left point of the area covered by
    // the building. The algorithm turn arround the area following clockwise.

    auto left(area.getLeft());
    auto right(area.getRight());
    int moveX(1);
    int moveY(0);

    MapCoordinates coordinates(left.getNorth());
    auto node(fetchNodeAt(coordinates));
    while (!node)
    {
        coordinates.setX(coordinates.getX() + moveX);
        coordinates.setY(coordinates.getY() + moveY);

        if (moveX == 1 && coordinates.getX() > right.getX())
        {
            // Overstep top corner.
            moveX = 0;
            moveY = 1;
        }
        else if (moveY == 1 && coordinates.getY() > right.getY())
        {
            // Overstep right corner.
            moveX = -1;
            moveY = 0;
        }
        else if (moveX == -1 && coordinates.getX() < left.getX())
        {
            // Overstep bottom corner.
            moveY = -1;
            moveX = 0;
        }
        else if (moveY == -1 && coordinates.getY() < left.getY())
        {
            // Overstep left corner. No node found.
            return node; // nulptr
        }
        else
        {
            node = fetchNodeAt(coordinates);
        }
    }

    return node;
}





QList<const RoadGraphNode*> RoadGraph::getNextNodeList(const MapCoordinates& previousLocation, const MapCoordinates& currentLocation) const
{
    auto node(fetchNodeAt(currentLocation));
    if (!node)
    {
        throw UnexpectedException("Tried to get next nodes from a non existing node.");
    }

    auto list(node->getNeighbourNodeList());
    if (list.size() > 1)
    {
        // If there is more than one node arround, we delete the node at the previous location so the walker do not turn
        // arround in the middle of the road.
        foreach (auto element, list)
        {
            if (element->getCoordinates() == previousLocation)
            {
                list.removeOne(element);
                break;
            }
        }
    }

    return list;
}





const RoadGraphNode* RoadGraph::createNode(const MapCoordinates& coordinates)
{
    auto node(fetchNodeAt(coordinates));
    if (node)
    {
        throw UnexpectedException("A node already exists at the coordinates " + coordinates.toString());
    }

    nodeList.append(new RoadGraphNode(*this, coordinates));

    return nodeList.last();
}





void RoadGraph::deleteNode(const MapCoordinates& coordinates)
{
    auto node(fetchNodeAt(coordinates));
    if (node)
    {
        delete nodeList.takeAt(nodeList.indexOf(node));
    }
}