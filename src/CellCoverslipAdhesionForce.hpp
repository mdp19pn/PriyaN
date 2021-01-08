#ifndef CELLCOVERSLIPADHESIONFORCE_HPP_
#define CELLCOVERSLIPADHESIONFORCE_HPP_

#include "ChasteSerialization.hpp"
#include <boost/serialization/base_object.hpp>

#include "AbstractForce.hpp"

///\todo Document this class
template<unsigned DIM>
class CellCoverslipAdhesionForce : public AbstractForce<DIM>
{
private:

    /** Spring stiffness. */
    double mStiffness;

    /** Spring equilibrium length. */
    double mEquilibriumLength;

    /** Gravity. */
    double mGravity;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractForce<DIM> >(*this);
        archive & mStiffness;
        archive & mEquilibriumLength;
        archive & mGravity;
    }

public:

    /**
     * Constructor.
     */
    CellCoverslipAdhesionForce();

    /**
     * Destructor.
     */
    ~CellCoverslipAdhesionForce();

    /*
     * Set the spring stiffness.
     *
     * @params springStiffness the value to assign to the spring stiffness.
     */
    void SetStiffness(double stiffness);

    /*
     * Set the spring equilibrium length.
     *
     * @params equilibriumLength the value to assign to the spring equilibrium length.
     */
    void SetEquilibriumLength(double equilibriumLength);

    /*
     * Set the gravity.
     *
     * @params equilibriumLength the value to assign to the gravity.
     */
    void SetGravity(double gravity);

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
EXPORT_TEMPLATE_CLASS_SAME_DIMS(CellCoverslipAdhesionForce)

#endif /*CELLCOVERSLIPADHESIONFORCE_HPP_*/