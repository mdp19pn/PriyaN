#ifndef MYOEPITHELIALCELLPROPERTY_HPP_
#define MYOEPITHELIALCELLPROPERTY_HPP_

/*
 * = Include necessary header files
 */
#include <boost/shared_ptr.hpp>
#include "AbstractCellProperty.hpp"
#include "ChasteSerialization.hpp"
#include <boost/serialization/base_object.hpp>

/*
 * == Defining the cell property class ==
 */
class MyoepithelialCellProperty : public AbstractCellProperty
{
protected:

    /**
     * Boolean encoding whether B1 integrin is expressed in myoepithelial cells.
     */
    bool mB1IntegrinExpression;

    /**
     * Boolean encoding whether B4 integrin is expressed in myoepithelial cells.
     */
    bool mB4IntegrinExpression;

private:

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
     * @param b1IntegrinExpression  whether B1 integrin is expressed in myoepithelial cells
     * @param b4IntegrinExpression  whether B4 integrin is expressed in myoepithelial cells
     */
    MyoepithelialCellProperty(bool b1IntegrinExpression=false, bool b4IntegrinExpression=true);

    /**
     * Virtual destructor, to make this class polymorphic.
     */
    virtual ~MyoepithelialCellProperty();

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

#include "SerializationExportWrapper.hpp"
// Declare identifier for the serializer
CHASTE_CLASS_EXPORT(MyoepithelialCellProperty)

#endif /*MYOEPITHELIALCELLPROPERTY_HPP_*/
