#ifndef CELLVELOCITYWRITER_HPP_
#define CELLVELOCITYWRITER_HPP_

#include "AbstractCellWriter.hpp"
#include "ChasteSerialization.hpp"
#include <boost/serialization/base_object.hpp>

/**
 * A class written using the visitor pattern for writing the velocities of the cell
 * associated with a cell population to file.
 *
 * The output file is called cellsvelocities.dat by default.
 *
 * \todo some code duplication in methods; potential for refactoring (#2404)
 */
template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
class CellVelocityWriter : public AbstractCellPopulationWriter<ELEMENT_DIM, SPACE_DIM>
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
    CellVelocityWriter();

    /**
     * Visit the population and write the location of each Cell.
     *
     * Outputs a line of space-separated values of the form:
     * ... [Cell index] [Cell x-pos] [Cell y-pos] [Cell z-pos] [Cell x-vel] [Cell y-vel] [Cell z-vel]...
     *
     * where z-pos and z-vel are used in 3 dimensions.
     * Here the indexing of cells is as given by the CellIterator.
     *
     * This line is appended to the output written by AbstractCellBasedWriter, which is a single
     * value [present simulation time], followed by a tab.
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
     * Visit the population and write the location of each Cell.
     *
     * Outputs a line of space-separated values of the form:
     * ... [Cell index] [Cell x-pos] [Cell y-pos] [Cell z-pos] [Cell x-vel] [Cell y-vel] [Cell z-vel]...
     *
     * where z-pos and z-vel are used in 3 dimensions.
     * Here the indexing of Cells is as given by the CellIterator.
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
     * This is an empty dummy function, since this class is not defined for use with a PottsBasedCellPopulation.
     *
     * @param pCellPopulation a pointer to the PottsBasedCellPopulation to visit.
     */
    virtual void Visit(PottsBasedCellPopulation<SPACE_DIM>* pCellPopulation);

    /**
     * Visit the population and write the location of each Cell.
     *
     * Outputs a line of space-separated values of the form:
     * ... [Cell index] [Cell x-pos] [Cell y-pos] [Cell z-pos] [Cell x-vel] [Cell y-vel] [Cell z-vel]...
     *
     * where z-pos and z-vel are used in 3 dimensions.
     * Here the indexing of Cells is as given by the CellIterator.
     *
     * This line is appended to the output written by AbstractCellBasedWriter, which is a single
     * value [present simulation time], followed by a tab.
     *
     * @param pCellPopulation a pointer to the VertexBasedCellPopulation to visit.
     */
    virtual void Visit(VertexBasedCellPopulation<SPACE_DIM>* pCellPopulation);
};

#include "SerializationExportWrapper.hpp"
// Declare identifier for the serializer
EXPORT_TEMPLATE_CLASS_ALL_DIMS(CellVelocityWriter)

#endif /*CELLVELOCITYWRITER_HPP_*/
