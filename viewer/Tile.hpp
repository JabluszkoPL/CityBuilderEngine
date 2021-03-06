#ifndef TILE_HPP
#define TILE_HPP

#include <QtCore/QList>
#include <QtCore/QStack>
#include <QtWidgets/QGraphicsObject>

#include "engine/map/MapCoordinates.hpp"

class DynamicElement;
class StaticElement;

class Tile : public QGraphicsObject
{
        Q_OBJECT

    private:
        MapCoordinates location;
        QStack<StaticElement*> staticElementList;
        QList<DynamicElement*> dynamicElementList;

    public:
        Tile(const MapCoordinates& location, const QSizeF& baseTileSize);

        const MapCoordinates& getCoordinates() const;

        void pushStaticElement(StaticElement* element);

        /**
         * @brief Pop the last graphics item displayed on the tile.
         *
         * The graphics item's parent is reset so make sure you delete the object if you don't use it anymore.
         */
        StaticElement* popStaticElement();

        void registerDynamicElement(DynamicElement* element);

        void unregisterDynamicElement(DynamicElement* element);

        virtual QRectF boundingRect() const;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr);
        virtual QPainterPath shape() const;

    signals:
        void isCurrentTile(Tile* tile);

    protected:
        virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
};

#endif // TILE_HPP
