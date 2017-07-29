#ifndef RANDOMWALKER_HPP
#define RANDOMWALKER_HPP

#include "engine/element/character/TargetedWalker.hpp"

/**
 * @brief A random walker walks on the roads and take random directions at road intersections.
 *
 * The random walker does have a walking credit. Each tile walked on consumes one credit. As soon as the walking credit
 * has expired, the random walker become a targeted walker.
 */
class RandomWalker : public TargetedWalker
{
    private:
        int walkingCredit;

    public:
        RandomWalker(const RoadGraph& roadGraph, const MapCoordinates& initialLocation, const int walkingCredit);

    protected:
        virtual MapCoordinates findNextGoingToLocation();
};

#endif // RANDOMWALKER_HPP
