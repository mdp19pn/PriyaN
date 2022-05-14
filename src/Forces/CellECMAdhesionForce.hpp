#ifndef CELLECMADHESIONFORCE_HPP_
#define CELLECMADHESIONFORCE_HPP_

#include "ChasteSerialization.hpp"
#include <boost/serialization/base_object.hpp>

#include "AbstractForce.hpp"

template<unsigned DIM>
class CellECMAdhesionForce : public AbstractForce<DIM>
{
private:

    /** Spring stiffness. */
    double mStiffness;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractForce<DIM> >(*this);
        archive & mStiffness;
    }

public:

    /**
     * Constructor.
     */
    CellECMAdhesionForce();

    /**
     * Destructor.
     */
    ~CellECMAdhesionForce();

    /*
     * Set the spring stiffness.
     *
     * @params springStiffness the value to assign to the spring stiffness.
     */
    void SetStiffness(double stiffness);

    /**
     * Overridden AddForceContribution() method.
     *
     * @param rCellPopulation reference to the tissue
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
EXPORT_TEMPLATE_CLASS_SAME_DIMS(CellECMAdhesionForce)

#endif /*CELLECMADHESIONFORCE_HPP_*/