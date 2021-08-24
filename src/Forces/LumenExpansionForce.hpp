#ifndef LUMENEXPANSIONFORCE_HPP_
#define LUMENEXPANSIONFORCE_HPP_

#include "ChasteSerialization.hpp"
#include <boost/serialization/base_object.hpp>

#include "AbstractForce.hpp"

template<unsigned DIM>
class LumenExpansionForce : public AbstractForce<DIM>
{
    friend class TestForcesNotForRelease;
private:

    friend class boost::serialization::access;
    /**
     * Boost Serialization method for archiving/checkpointing.
     * Archives the object and its member variables.
     *
     * @param archive  The boost archive.
     * @param version  The current version of this class.
     */
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractForce<DIM> >(*this);
        archive & mStrength;
    }

    /**
     * @return strength.
     */
    double mStrength;

public:

    /**
     * Constructor.
     */
    LumenExpansionForce();

    /**
     * @return mStrength.
     */
    double GetStrength();

    /**
     * Set mStrength.
     *
     * @param strength the new value of mStrength
     */
    void SetStrength(double strength);

    /**
     * Overridden AddForceContribution() method.
     *
     * @param rCellPopulation a cell population object
     */
    void AddForceContribution(AbstractCellPopulation<DIM>& rCellPopulation);

    /**
     * Overridden OutputForceParameters() method.
     *
     * @param rParamsFile the file stream to which the parameters are output
     */
    void OutputForceParameters(out_stream& rParamsFile);
};

#include "SerializationExportWrapper.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(LumenExpansionForce)

#endif /*LUMENEXPANSIONFORCE_*/