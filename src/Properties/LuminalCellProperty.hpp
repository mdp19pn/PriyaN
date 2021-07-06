#ifndef LUMINALCELLPROPERTY_HPP_
#define LUMINALCELLPROPERTY_HPP_

#include <boost/shared_ptr.hpp>
#include "AbstractMammaryCellProperty.hpp"
#include "ChasteSerialization.hpp"
#include <boost/serialization/base_object.hpp>

/**
 * Subclass of AbstractMammaryCellProperty defining a luminal cell.
 */
class LuminalCellProperty : public AbstractMammaryCellProperty
{
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
        archive & boost::serialization::base_object<AbstractMammaryCellProperty>(*this);
    }

public:

    /**
     * Constructor.
     *
     * @param b1IntegrinExpression  whether B1 integrin by this luminal cell
     * @param b4IntegrinExpression  whether B4 integrin by this luminal cell
     */
    LuminalCellProperty(bool b1IntegrinExpression=true, bool b4IntegrinExpression=true);

    /**
     * Virtual destructor, to make this class polymorphic.
     */
    virtual ~LuminalCellProperty();
};

#include "SerializationExportWrapper.hpp"
// Declare identifier for the serializer
CHASTE_CLASS_EXPORT(LuminalCellProperty)
    
#endif /*LUMINALCELLPROPERTY_HPP_*/
