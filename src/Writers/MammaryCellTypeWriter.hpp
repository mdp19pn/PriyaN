
#ifndef MAMMARYCELLTYPEWRITER_HPP_
#define MAMMARYCELLTYPEWRITER_HPP_

#include "ChasteSerialization.hpp"
#include <boost/serialization/base_object.hpp>
#include "AbstractCellWriter.hpp"

/**
 * A class for writing mammary cell types (luminal and myoepithelial) to file.
 *
 * The output file is called results.viztypes by default. If VTK is switched on,
 * then the writer also specifies the VTK output for each cell, which is stored in
 * the VTK cell data "Mammary cell types" by default.
 */
template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
class MammaryCellTypeWriter : public AbstractCellWriter<ELEMENT_DIM, SPACE_DIM>
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
    MammaryCellTypeWriter();

    /* Overridden GetCellDataForVtkOutput() method.
     *
     * Get a double associated with a cell. This method reduces duplication
     * of code between the methods VisitCell() and AddVtkData().
     *
     * The double can take the following values:
     * 
     * 0.0 : Luminal cell, no B1 expression, no B4 expression
     * 1.0 : Luminal cell, B1 expression, no B4 expression
     * 2.0 : Luminal cell, no B1 expression, B4 expression
     * 3.0 : Luminal cell, B1 expression, B4 expression
     * 4.0 : Myoepithelial cell, no B1 expression, no B4 expression
     * 5.0 : Myoepithelial cell, B1 expression, no B4 expression
     * 6.0 : Myoepithelial cell, no B1 expression, B4 expression
     * 7.0 : Myoepithelial cell, B1 expression, B4 expression
     * 8.0 : Mammary Stem Cell
     *
     * @param pCell a cell
     * @param pCellPopulation a pointer to the cell population owning the cell
     *
     * @return data associated with the cell
     */
    double GetCellDataForVtkOutput(CellPtr pCell, AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation);

    /**
     * Overridden VisitCell() method.
     *
     * Visit a cell and write its mammary cell type and B1/B4 integrin expression.
     *
     * Outputs a line of space-separated values of the form:
     * ...[cell type] [B1 expn] [B4 expn] [location index] [x-pos] [y-pos] [z-pos] ...
     * with [y-pos] and [z-pos] included for 2 and 3 dimensional simulations, respectively.
     *
     * Here "cell type" takes the value 0.0 for luminal cells and 1.0 for 
     * myoepithelial cells.
     *
     * This is appended to the output written by AbstractCellBasedWriter, which is a single
     * value [present simulation time], followed by a tab.
     *
     * @param pCell a cell
     * @param pCellPopulation a pointer to the cell population owning the cell
     */
    virtual void VisitCell(CellPtr pCell, AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation);
};

#include "SerializationExportWrapper.hpp"
EXPORT_TEMPLATE_CLASS_ALL_DIMS(MammaryCellTypeWriter)

#endif /* MAMMARYCELLTYPEWRITER_HPP_ */
