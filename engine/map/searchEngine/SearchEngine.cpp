#include "SearchEngine.hpp"

#include "engine/element/static/AbstractStaticMapElement.hpp"
#include "engine/map/searchEngine/StaticSearchCriteria.hpp"



SearchEngine::SearchEngine(QObject* parent, const QLinkedList<AbstractStaticMapElement*>& staticElements) :
    QObject(parent),
    staticElements(staticElements)
{

}



QList<AbstractStaticMapElement*> SearchEngine::search(const StaticSearchCriteria& criteria) const
{
    QList<AbstractStaticMapElement*> results;
    for (auto element : staticElements) {
        if (element->getConf() == criteria.getConf()) {
            results.append(element);
        }
    }

    return results;
}
