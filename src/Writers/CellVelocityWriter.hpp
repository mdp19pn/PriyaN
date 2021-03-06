#ifndef CELLVELOCITYWRITER_HPP_
#define CELLVELOCITYWRITER_HPP_

#include "AbstractCellWriter.hpp"
#include "ChasteSerialization.hpp"
#include <boost/serialization/base_object.hpp>

/**
 * A class for writing cell velocities to file.
 */
template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
class CellVelocityWriter : public AbstractCellWriter<ELEMENT_DIM, SPACE_DIM>
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
        archive & boost::serialization::base_object<AbstractCellWriter<ELEMENT_DIM, SPACE_DIM> >(*this);
    }

public:

    /**
     * Default constructor.
     */
    CellVelocityWriter();

    /**
     * Overridden GetCellDataForVtkOutput() method.
     *
     * Get a double associated with a cell. This method reduces duplication
     * of code between the methods VisitCell() and AddVtkData().
     *
     * @param pCell a cell
     * @param pCellPopulation a pointer to the cell population owning the cell
     *
     * @return data associated with the cell
     */
    double GetCellDataForVtkOutput(CellPtr pCell, AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation);

    /**
     * Visit a cell and write its location index.
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
     * @param pCellPopulation a pointer to the cell population owning the cell.
     */
    virtual void VisitCell(CellPtr pCell, AbstractCellPopulation<ELEMENT_DIM,SPACE_DIM>* pCellPopulation);
};

#include "SerializationExportWrapper.hpp"
// Declare identifier for the serializer
EXPORT_TEMPLATE_CLASS_ALL_DIMS(CellVelocityWriter)

#endif /*CELLVELOCITYWRITER_HPP_*/
