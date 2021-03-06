#ifndef ROADPATHFINDERORDEREDNODELIST_HPP
#define ROADPATHFINDERORDEREDNODELIST_HPP

#include <QtCore/QLinkedList>

class RoadPathFinderNode;

class RoadPathFinderOrderedNodeList
{
    private:
        QLinkedList<RoadPathFinderNode*> nodeList;

    public:
        RoadPathFinderOrderedNodeList();

        bool isEmpty() const;

        RoadPathFinderNode* find(RoadPathFinderNode* node) const;

        RoadPathFinderNode* takeFirst();

        void insert(RoadPathFinderNode* node);
};

#endif // ROADPATHFINDERORDEREDNODELIST_HPP
