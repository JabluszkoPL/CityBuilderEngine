#ifndef BEHAVIORFACTORY_HPP
#define BEHAVIORFACTORY_HPP

#include <QtCore/QObject>
#include <QtCore/QList>

class AbstractStaticElementBehavior;
class AbstractProcessableStaticMapElement;
class BehaviorInformation;
class Map;
class SearchEngine;

class BehaviorFactory : public QObject
{
        Q_OBJECT

    private:
        const Map* map;
        const SearchEngine* searchEngine;

    public:
        BehaviorFactory(QObject* parent, const Map* map, const SearchEngine* searchEngine);

        AbstractStaticElementBehavior* generate(
            AbstractProcessableStaticMapElement* issuer,
            const BehaviorInformation* conf
        ) const;
};

#endif // BEHAVIORFACTORY_HPP
