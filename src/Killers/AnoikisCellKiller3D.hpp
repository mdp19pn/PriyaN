#ifndef ANOIKISCELLKILLER3D_HPP_
#define ANOIKISCELLKILLER3D_HPP_

#include "AbstractCellKiller.hpp"

#include "ChasteSerialization.hpp"
#include <boost/serialization/base_object.hpp>

/**
 *  Kills cells if they are outside a circle whose centre and radius can be
 *  passed in but are take default values.
 */
class AnoikisCellKiller3D : public AbstractCellKiller<3>
{
private:

    /** Centre of death. */
    c_vector<double,3> mCentre;

    /** Radius of death. */
    double mRadius;

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
        archive & boost::serialization::base_object<AbstractCellKiller<3> >(*this);
    }

public:

    /**
     * Constructor.
     *
     * @param pCellPopulation pointer to the cell population.
     * @param centre the centre of death.
     * @param radius the radius of death.
     */
    AnoikisCellKiller3D(AbstractCellPopulation<3>* pCellPopulation,
                              c_vector<double,3> centre,
                              double radius);

    /**
     * @return mCentre.
     */
    c_vector<double,3> GetCentre() const;

    /**
     * @return mRadius.
     */
    double GetRadius() const;

    /**
     * Loop over cells and kills cells outside boundary.
     */
    virtual void CheckAndLabelCellsForApoptosisOrDeath();

    /**
     * Outputs cell killer parameters to file
     *
     * As this method is pure virtual, it must be overridden
     * in subclasses.
     *
     * @param rParamsFile the file stream to which the parameters are output
     */
    void OutputCellKillerParameters(out_stream& rParamsFile);
};

#include "SerializationExportWrapper.hpp"
CHASTE_CLASS_EXPORT(AnoikisCellKiller3D)

namespace boost
{
namespace serialization
{
/**
 * Serialize information required to construct a AnoikisCellKiller3D.
 */
template<class Archive>
inline void save_construct_data(
    Archive & ar, const AnoikisCellKiller3D * t, const unsigned int file_version)
{
    // Save data required to construct instance
    const AbstractCellPopulation<3>* const p_cell_population = t->GetCellPopulation();
    ar & p_cell_population;
    c_vector<double,3> centre = t->GetCentre();
    ar & centre[0];
    ar & centre[1];
    ar & centre[2];
    double radius = t->GetRadius();
    ar & radius;
}

/**
 * De-serialize constructor parameters and initialise a AnoikisCellKiller3D.
 */
template<class Archive>
inline void load_construct_data(
    Archive & ar, AnoikisCellKiller3D * t, const unsigned int file_version)
{
    // Retrieve data from archive required to construct new instance
    AbstractCellPopulation<3>* p_cell_population;
    ar & p_cell_population;
    c_vector<double,3> centre;
    ar & centre[0];
    ar & centre[1];
    ar & centre[2];
    double radius;
    ar & radius;

    // Invoke inplace constructor to initialise instance
    ::new(t)AnoikisCellKiller3D(p_cell_population, centre, radius);
}
}
} // namespace ...


#endif /*ANOIKISCELLKILLER3D_HPP_*/