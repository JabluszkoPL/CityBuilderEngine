#ifndef ABSTRACTCHARACTER_HPP
#define ABSTRACTCHARACTER_HPP

#include "engine/element/AbstractDynamicMapElement.hpp"

/**
 * @brief The base class for characters.
 */
class AbstractCharacter : public AbstractDynamicMapElement
{
    public:
        AbstractCharacter(const MapCoordinates& initialLocation, const qreal speed);
};

#endif // ABSTRACTCHARACTER_HPP
