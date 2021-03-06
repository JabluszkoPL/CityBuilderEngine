#ifndef DYNAMICELEMENT_HPP
#define DYNAMICELEMENT_HPP

#include <QtCore/QPointer>
#include <QtWidgets/QGraphicsPixmapItem>

#include "engine/map/MapCoordinates.hpp"

class AbstractDynamicMapElement;

class DynamicElement : public QGraphicsPixmapItem
{
    private:
        const QSizeF& baseTileSize;
        QPointer<const AbstractDynamicMapElement> element;

    public:
        DynamicElement(const QSizeF& baseTileSize, const AbstractDynamicMapElement* element, const QPixmap& elementImage);

        /**
         * @brief Indicae if the logical element linked to this graphical element still exists.
         */
        bool stillExists();

        MapCoordinates getCoordinates() const;

        void refresh();
};

#endif // DYNAMICELEMENT_HPP
