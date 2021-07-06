#ifndef BOUNDARYLENGTHWRITER_HPP_
#define BOUNDARYLENGTHWRITER_HPP_

#include "AbstractCellPopulationWriter.hpp"
#include "ChasteSerialization.hpp"
#include <boost/serialization/base_object.hpp>

/**
 * A class written using the visitor pattern for writing the length of the
 * boundaries between labelled and unlabelled cells (as determined by the
 * cell property CellLabel) in a cell population to file. This is a measure
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
class BoundaryLengthWriter : public AbstractCellPopulationWriter<ELEMENT_DIM, SPACE_DIM>
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
    BoundaryLengthWriter();

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

    /**
     * Visit the population and write the data.
     *
     * This is an empty dummy function, since this class is not defined for use with a CaBasedCellPopulation.
     *
     * @param pCellPopulation a pointer to the MeshBasedCellPopulation to visit.
     */
    virtual void Visit(MeshBasedCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation);

    /**
     * Visit the population and write the data.
     *
     * This is an empty dummy function, since this class is not defined for use with a CaBasedCellPopulation.
     *
     * @param pCellPopulation a pointer to the CaBasedCellPopulation to visit.
     */
    virtual void Visit(CaBasedCellPopulation<SPACE_DIM>* pCellPopulation);

    /**
     * Visit the population and write the data.
     *
     * This is an empty dummy function, since this class is not defined for use with a CaBasedCellPopulation.
     *
     * @param pCellPopulation a pointer to the PottsBasedCellPopulation to visit.
     */
    virtual void Visit(PottsBasedCellPopulation<SPACE_DIM>* pCellPopulation);

    /**
     * Visit the population and write the data.
     *
     * This is an empty dummy function, since this class is not defined for use with a CaBasedCellPopulation.
     *
     * @param pCellPopulation a pointer to the VertexBasedCellPopulation to visit.
     */
    virtual void Visit(VertexBasedCellPopulation<SPACE_DIM>* pCellPopulation);
};

#include "SerializationExportWrapper.hpp"
// Declare identifier for the serializer
EXPORT_TEMPLATE_CLASS_ALL_DIMS(BoundaryLengthWriter)

#endif /* BOUNDARYLENGTHWRITER_HPP_ */
