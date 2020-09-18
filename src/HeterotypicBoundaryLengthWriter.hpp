#ifndef HETEROTYPICBOUNDARYLENGTHWRITER_HPP_
#define HETEROTYPICBOUNDARYLENGTHWRITER_HPP_

#include "AbstractCellPopulationWriter.hpp"
#include "ChasteSerialization.hpp"
#include <boost/serialization/base_object.hpp>

/**
 * A class written using the visitor pattern for writing the length of the
 * boundaries between luminal and myoepithelial cells (as determined by the
 * LuminalCellProperty and MyoepithelialCellProperty) in a cell population to file. This is a measure
 * of how mixed the populations are.
 *
 * The output file is called heterotypicboundary.dat by default.
 *
 * For usage of this measure for cell sorting, see for example the
 * heterotypic boundary length described in Zhang et al (2011). Computer
 * simulations of cell sorting due to differential adhesion. PLOS ONE
 * 6(10):e24999. doi:10.1371/journal.pone.0024999
 */
template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
class HeterotypicBoundaryLengthWriter : public AbstractCellPopulationWriter<ELEMENT_DIM, SPACE_DIM>
{
private:
    /** Needed for serialization. */
    friend class boost::serialization::access;
    /**
     * Serialize the object and its member variables.
     *
     * @param archive the archive
     * @param version the current version of this class
     */
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractCellPopulationWriter<ELEMENT_DIM, SPACE_DIM> >(*this);
    }

public:

    /**
     * Default constructor.
     */
    HeterotypicBoundaryLengthWriter();

    /**
     * Visit the population and write the labelled boundary length data.
     *
     * Outputs a line of tab-separated values of the form:
     * [fractional_length] [total_length] [fractional_neighbours] [total_neighbours]
     *
     * Here the indexing of nodes is as given by the NodeIterator.
     *
     * This line is appended to the output written by AbstractCellBasedWriter, which is a single
     * value [present simulation time], followed by a tab.
     *
     * @param pCellPopulation a pointer to the NodeBasedCellPopulation to visit.
     */
    virtual void Visit(NodeBasedCellPopulation<SPACE_DIM>* pCellPopulation);
};

#include "SerializationExportWrapper.hpp"
// Declare identifier for the serializer
EXPORT_TEMPLATE_CLASS_ALL_DIMS(HeterotypicBoundaryLengthWriter)

#endif /* HETEROTYPICBOUNDARYLENGTHWRITER_HPP_ */
