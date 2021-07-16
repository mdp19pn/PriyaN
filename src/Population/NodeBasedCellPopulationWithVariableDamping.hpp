#ifndef NODEBASEDCELLPOPULATIONWITHVARIABLEDAMPING_HPP_
#define NODEBASEDCELLPOPULATIONWITHVARIABLEDAMPING_HPP_

#include "NodeBasedCellPopulation.hpp"
#include "AbstractForce.hpp"

#include "ChasteSerialization.hpp"
#include <boost/serialization/base_object.hpp>


template<unsigned DIM>
class NodeBasedCellPopulationWithVariableDamping : public NodeBasedCellPopulation<DIM>
{
private:

    /** Needed for serialization. */
    friend class boost::serialization::access;
    /**
     * Serialize the object and its member variables.
     *
     * Note that serialization of the nodes is handled by load/save_construct_data,
     * so we don't actually have to do anything here except delegate to the base class.
     *
     * @param archive the archive
     * @param version the current version of this class
     */
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<NodeBasedCellPopulation<DIM> >(*this);
        archive & mLuminalCellDampingConstant;
        archive & mMyoepithelialCellDampingConstant;
        archive & mLuminalStemCellDampingConstant;
        archive & mMyoepithelialStemCellDampingConstant;
    }

    double mLuminalCellDampingConstant;
    double mMyoepithelialCellDampingConstant;
    double mLuminalStemCellDampingConstant;
    double mMyoepithelialStemCellDampingConstant;

public:

    /**
     * Default constructor.
     *
     * Note that the cell population will take responsibility for freeing the memory used by the nodes.
     *
     * @param rMesh a mutable nodes-only mesh
     * @param rCells a vector of cells
     * @param locationIndices an optional vector of location indices that correspond to real cells
     * @param deleteMesh whether to delete nodes-only mesh in destructor
     */
    NodeBasedCellPopulationWithVariableDamping(NodesOnlyMesh<DIM>& rMesh,
                                           std::vector<CellPtr>& rCells,
                                           const std::vector<unsigned> locationIndices=std::vector<unsigned>(),
                                           bool deleteMesh=false);

    /**
     * Constructor for use by the de-serializer.
     *
     * @param rMesh a mutable nodes-only mesh
     */
    NodeBasedCellPopulationWithVariableDamping(NodesOnlyMesh<DIM>& rMesh);

    /**
     * Overridden GetDampingConstant() method.
     *
     * Get the damping constant for the cell associated with this node,
     * i.e. d in drdt = F/d.
     *
     * This value depends on whether the cell is luminal or myoepithelial.
     *
     * @param nodeIndex the global index of this node
     * @return the damping constant at the Cell associated with this node
     */
    virtual double GetDampingConstant(unsigned nodeIndex);
    
    /**
     * Set mLuminalCellDampingConstant.
     * 
     * @param luminalCellDampingConstant  the new value of mLuminalCellDampingConstant
     */
    void SetLuminalCellDampingConstant(double luminalCellDampingConstant);

    /**
     * Set mMyoepithelialCellDampingConstant.
     * 
     * @param myoepithelialCellDampingConstant  the new value of mMyoepithelialCellDampingConstant
     */
    void SetMyoepithelialCellDampingConstant(double myoepithelialCellDampingConstant);

    /**
     * Set mLuminalStemCellDampingConstant.
     * 
     * @param LuminalStemCellDampingConstant  the new value of mMyoepithelialCellDampingConstant
     */
    void SetLuminalStemCellDampingConstant(double LuminalStemCellDampingConstant);

    /**
     * Set mMyoepithelialStemCellDampingConstant.
     * 
     * @param MyoepithelialStemCellDampingConstant  the new value of mMyoepithelialCellDampingConstant
     */
    void SetMyoepithelialStemCellDampingConstant(double MyoepithelialStemCellDampingConstant);

    /**
     * @return mLuminalCellDampingConstant
     */
    double GetLuminalCellDampingConstant();

    /**
     * @return mMyoepithelialCellDampingConstant
     */
    double GetMyoepithelialCellDampingConstant();

    /**
     * @return mLuminalStemCellDampingConstant
     */
    double GetLuminalStemCellDampingConstant();

    /**
     * @return mMyoepithelialStemCellDampingConstant
     */
    double GetMyoepithelialStemCellDampingConstant();

    /**
     * Overridden AddForceContribution() method.
     *
     * @param rCellPopulation reference to the tissue
     */
    void AddForceContribution(AbstractCellPopulation<DIM>& rCellPopulation);

    /**
     * Outputs CellPopulation parameters to file
     *
     * As this method is pure virtual, it must be overridden
     * in subclasses.
     *
     * @param rParamsFile the file stream to which the parameters are output
     */
    void OutputCellPopulationParameters(out_stream& rParamsFile);
};

#include "SerializationExportWrapper.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(NodeBasedCellPopulationWithVariableDamping)

namespace boost
{
namespace serialization
{
/**
 * Serialize information required to construct a NodeBasedCellPopulationWithVariableDamping.
 */
template<class Archive, unsigned DIM>
inline void save_construct_data(
    Archive & ar, const NodeBasedCellPopulationWithVariableDamping<DIM> * t, const unsigned int file_version)
{
    // Save data required to construct instance
    const NodesOnlyMesh<DIM>* p_mesh = &(t->rGetMesh());
    ar & p_mesh;
}

/**
 * De-serialize constructor parameters and initialise a NodeBasedCellPopulationWithVariableDamping.
 * Loads the mesh from separate files.
 */
template<class Archive, unsigned DIM>
inline void load_construct_data(
    Archive & ar, NodeBasedCellPopulationWithVariableDamping<DIM> * t, const unsigned int file_version)
{
    // Retrieve data from archive required to construct new instance
    NodesOnlyMesh<DIM>* p_mesh;
    ar >> p_mesh;

    // Invoke inplace constructor to initialise instance
    ::new(t)NodeBasedCellPopulationWithVariableDamping<DIM>(*p_mesh);
}
}
} // namespace ...

#endif /*NODEBASEDCELLPOPULATIONWITHVARIABLEDAMPING_HPP_*/
