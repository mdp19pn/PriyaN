#ifndef MAMMARYSTEMCELLPROPERTY_HPP_
#define MAMMARYSTEMCELLPROPERTY_HPP_

#include <boost/shared_ptr.hpp>
#include "AbstractMammaryCellProperty.hpp"
#include "ChasteSerialization.hpp"
#include <boost/serialization/base_object.hpp>

/**
 * Subclass of AbstractMammaryCellProperty defining a myoepithelial cell.
 */
class MammaryStemCellProperty : public AbstractMammaryCellProperty
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
     * @param b1IntegrinExpression  whether B1 integrin by this myoepithelial cell
     * @param b4IntegrinExpression  whether B4 integrin by this myoepithelial cell
     */
    MammaryStemCellProperty(bool b1IntegrinExpression=true, bool b4IntegrinExpression=true);

    /**
     * Virtual destructor, to make this class polymorphic.
     */
    virtual ~MammaryStemCellProperty();
};

#include "SerializationExportWrapper.hpp"
// Declare identifier for the serializer
CHASTE_CLASS_EXPORT(MammaryStemCellProperty)

#endif /*MAMMARYSTEMCELLPROPERTY_HPP_*/
