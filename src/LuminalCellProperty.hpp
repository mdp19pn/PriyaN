#ifndef LUMINALCELLPROPERTY_HPP_
#define LUMINALCELLPROPERTY_HPP_

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
class LuminalCellProperty : public AbstractCellProperty
{
protected:

    /**
     * Colour for use by visualizer.
     */
    unsigned mColour;

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
        archive & mColour;
    }

public:

    /**
     * Constructor.
     *
     * @param colour  what colour cells with this mutation state should be in the visualizer
     */
    LuminalCellProperty(unsigned colour=5);

    /**
     * Virtual destructor, to make this class polymorphic.
     */
    virtual ~LuminalCellProperty();

    /**
     * @return #mColour.
     */
    unsigned GetColour() const;
};

#include "SerializationExportWrapper.hpp"
// Declare identifier for the serializer
CHASTE_CLASS_EXPORT(LuminalCellProperty)
    
#endif /*LUMINALCELLPROPERTY_HPP_*/
