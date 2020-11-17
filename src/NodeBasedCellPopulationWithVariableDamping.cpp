#include "NodeBasedCellPopulationWithVariableDamping.hpp"
#include "LuminalCellProperty.hpp"

template<unsigned DIM>
NodeBasedCellPopulationWithVariableDamping<DIM>::NodeBasedCellPopulationWithVariableDamping(NodesOnlyMesh<DIM>& rMesh,
                                      std::vector<CellPtr>& rCells,
                                      const std::vector<unsigned> locationIndices,
                                      bool deleteMesh)
    : NodeBasedCellPopulation<DIM>(rMesh, rCells, locationIndices, deleteMesh),
      mLuminalCellDampingConstant(1.0),
      mMyoepithelialCellDampingConstant(1.0)
{
}

template<unsigned DIM>
NodeBasedCellPopulationWithVariableDamping<DIM>::NodeBasedCellPopulationWithVariableDamping(NodesOnlyMesh<DIM>& rMesh)
    : NodeBasedCellPopulation<DIM>(rMesh)
{
    // No Validate() because the cells are not associated with the cell population yet in archiving
}

template<unsigned DIM>
double NodeBasedCellPopulationWithVariableDamping<DIM>::GetDampingConstant(unsigned nodeIndex)
{
    CellPtr p_cell = this->GetCellUsingLocationIndex(nodeIndex);
    if (p_cell->template HasCellProperty<LuminalCellProperty>())
    {
        return mLuminalCellDampingConstant;
    }
    else
    {
        return mMyoepithelialCellDampingConstant;
    }
}

template<unsigned DIM>
void NodeBasedCellPopulationWithVariableDamping<DIM>::SetLuminalCellDampingConstant(double luminalCellDampingConstant)
{
    mLuminalCellDampingConstant = luminalCellDampingConstant;
}

template<unsigned DIM>
void NodeBasedCellPopulationWithVariableDamping<DIM>::SetMyoepithelialCellDampingConstant(double myoepithelialCellDampingConstant)
{
    mMyoepithelialCellDampingConstant = myoepithelialCellDampingConstant;
}

template<unsigned DIM>
double NodeBasedCellPopulationWithVariableDamping<DIM>::GetLuminalCellDampingConstant()
{
    return mLuminalCellDampingConstant;
}

template<unsigned DIM>
double NodeBasedCellPopulationWithVariableDamping<DIM>::GetMyoepithelialCellDampingConstant()
{
    return mMyoepithelialCellDampingConstant;
}

template<unsigned DIM>
void NodeBasedCellPopulationWithVariableDamping<DIM>::OutputCellPopulationParameters(out_stream& rParamsFile)
{
    *rParamsFile << "\t\t\t<LuminalCellDampingConstant>" << mLuminalCellDampingConstant << "</LuminalCellDampingConstant>\n";
    *rParamsFile << "\t\t\t<MyoepithelialCellDampingConstant>" << mMyoepithelialCellDampingConstant << "</MyoepithelialCellDampingConstant>\n";

    // Call method on direct parent class
    NodeBasedCellPopulation<DIM>::OutputCellPopulationParameters(rParamsFile);
}

// Explicit instantiation
template class NodeBasedCellPopulationWithVariableDamping<1>;
template class NodeBasedCellPopulationWithVariableDamping<2>;
template class NodeBasedCellPopulationWithVariableDamping<3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(NodeBasedCellPopulationWithVariableDamping)
