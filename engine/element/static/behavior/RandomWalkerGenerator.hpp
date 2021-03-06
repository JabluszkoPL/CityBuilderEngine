#ifndef RANDOMWALKERGENERATOR_HPP
#define RANDOMWALKERGENERATOR_HPP

#include <QtCore/QList>
#include <QtCore/QPointer>

#include "engine/element/dynamic/RandomWalker.hpp"
#include "engine/element/static/behavior/AbstractWalkerGenerator.hpp"

class AbstractDynamicMapElement;
class AbstractProcessableStaticMapElement;
class DynamicElementInformation;

class RandomWalkerGenerator : public AbstractWalkerGenerator
{
        Q_OBJECT

    public:
        RandomWalkerGenerator(
            AbstractProcessableStaticMapElement* issuer,
            const DynamicElementInformation* walkerConf,
            const int generationInterval,
            const int maxWalkers = 1
        );
};

#endif // RANDOMWALKERGENERATOR_HPP
