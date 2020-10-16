
#ifndef ABSTRACTMAMMARYCELLPROPERT_HPP_
#define ABSTRACTMAMMARYCELLPROPERT_HPP_

#include <boost/shared_ptr.hpp>
#include "AbstractCellProperty.hpp"
#include "ChasteSerialization.hpp"
#include <boost/serialization/base_object.hpp>

/**
 * Abstract base class for LuminalCellProperty and MyoepithelialCellProperty.
 */
class AbstractMammaryCellProperty : public AbstractCellProperty
{
protected:

    /**
     * Boolean encoding whether B1 integrin is expressed by this luminal or myoepithelial cell.
     */
    bool mB1IntegrinExpression;

    /**
     * Boolean encoding whether B4 integrin is expressed by this luminal or myoepithelial cell.
     */
    bool mB4IntegrinExpression;

    /** Needed for serialization. */
    friend class boost::serialization::access;
    /**
     * Archive the member variables.
     *
     * @param archive the archive
     * @param version the current version of this class
     */
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractCellProperty>(*this);
        archive & mB1IntegrinExpression;
        archive & mB4IntegrinExpression;
    }

public:

    /**
     * Constructor.
     *
     * @param b1IntegrinExpression  whether B1 integrin by this luminal or myoepithelial cell
     * @param b4IntegrinExpression  whether B4 integrin by this luminal or myoepithelial cell
     */
    AbstractMammaryCellProperty(bool b1IntegrinExpression=true, bool b4IntegrinExpression=true);

    /**
     * Virtual destructor, to make this class polymorphic.
     */
    virtual ~AbstractMammaryCellProperty();

    /**
     * @return #mColour.
     */
    unsigned GetColour() const;

    /**
     * @return #mB1IntegrinExpression.
     */
    bool GetB1IntegrinExpression() const;

    /**
     * @return #mB4IntegrinExpression.
     */
    bool GetB4IntegrinExpression() const;
};

#endif /*ABSTRACTMAMMARYCELLPROPERT_HPP_*/
