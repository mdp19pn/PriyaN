#ifndef CELLCOVERSLIPBASEDCELLKILLER_HPP_
#define CELLCOVERLSIPBASEDCELLKILLER_HPP_

#include "AbstractCellKiller.hpp"

#include "ChasteSerialization.hpp"
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>

/**
 * A cell killer that kills cells if they are outside the domain.
 */
template<unsigned DIM>
class CellCoverslipBasedCellKiller : public AbstractCellKiller<DIM>
{
private:

    /** Needed for serialization. */
    friend class boost::serialization::access;
    /**
     * Archive the object.
     *
     * @param archive the archive
     * @param version the current version of this class
     */
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractCellKiller<DIM> >(*this);
    }

public:

    /**
     * Default constructor
     * @param pCellPopulation pointer to the cell population
     */
    CellCoverslipBasedCellKiller(AbstractCellPopulation<DIM>* pCellPopulation);

    /**
     * Loops over cells and kills cells outside boundary.
     */
    void CheckAndLabelCellsForApoptosisOrDeath();

    /**
     * Overridden OutputCellKillerParameters() method.
     *
     * @param rParamsFile the file stream to which the parameters are output
     */
    void OutputCellKillerParameters(out_stream& rParamsFile);
};

#include "SerializationExportWrapper.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(CellCoverslipBasedCellKiller)

namespace boost
{
    namespace serialization
    {
        /**
         * Serialize information required to construct a CellCoverslipBasedCellKiller.
         */
        template<class Archive, unsigned DIM>
        inline void save_construct_data(
            Archive & ar, const CellCoverslipBasedCellKiller<DIM> * t, const unsigned int file_version)
            {
                // Save data required to construct instance
                const AbstractCellPopulation<DIM>* const p_cell_population = t->GetCellPopulation();
                ar << p_cell_population;
            }

        /**
         * De-serialize constructor parameters and initialise a CellCoverslipBasedCellKiller.
         */
        template<class Archive, unsigned DIM>
        inline void load_construct_data(
            Archive & ar, CellCoverslipBasedCellKiller<DIM> * t, const unsigned int file_version)
        {
            // Retrieve data from archive required to construct new instance
            AbstractCellPopulation<DIM>* p_cell_population;
            ar >> p_cell_population;

            // Invoke inplace constructor to initialise instance
            ::new(t)CellCoverslipBasedCellKiller<DIM>(p_cell_population);
        }
    }
} // namespace ...

#endif /*CELLCOVERSLIPEBASEDCELLKILLER_HPP_*/
