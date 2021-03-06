#ifndef CONDITIONALRANDOMWALKERGENERATOR_HPP
#define CONDITIONALRANDOMWALKERGENERATOR_HPP

#include "engine/element/static/behavior/RandomWalkerGenerator.hpp"
#include "engine/processing/CycleDate.hpp"

class AbstractDynamicMapElement;
class AbstractProcessableStaticMapElement;
class DynamicElementInformation;

class ConditionalRandomWalkerGenerator : public RandomWalkerGenerator
{
        Q_OBJECT

    private:
        const DynamicElementInformation* dependencyWalkerConf;
        const int activityInterval;
        CycleDate canGenarateWalkerUntilDate;

    public:
        ConditionalRandomWalkerGenerator(
            AbstractProcessableStaticMapElement* issuer,
            const DynamicElementInformation* walkerConf,
            const DynamicElementInformation* dependencyWalkerConf,
            const int generationInterval,
            const int maxWalkers = 1
        );

        virtual void process(const CycleDate &date) override;

        virtual bool processInteraction(const CycleDate& date, AbstractDynamicMapElement* actor) override;

    protected:
        virtual bool canSetupNextGenerationDate(const CycleDate& currentDate) const override;
};

#endif // CONDITIONALRANDOMWALKERGENERATOR_HPP
