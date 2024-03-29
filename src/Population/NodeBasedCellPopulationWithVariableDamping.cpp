#include "NodeBasedCellPopulationWithVariableDamping.hpp"
#include "LuminalCellProperty.hpp"
#include "MyoepithelialCellProperty.hpp"
#include "LuminalStemCellProperty.hpp"
#include "MyoepithelialStemCellProperty.hpp"

template<unsigned DIM>
NodeBasedCellPopulationWithVariableDamping<DIM>::NodeBasedCellPopulationWithVariableDamping(NodesOnlyMesh<DIM>& rMesh,
                                      std::vector<CellPtr>& rCells,
                                      const std::vector<unsigned> locationIndices,
                                      bool deleteMesh)
    : NodeBasedCellPopulation<DIM>(rMesh, rCells, locationIndices, deleteMesh),
      mLuminalCellDampingConstant(1.0),
      mMyoepithelialCellDampingConstant(1.0),
      mLuminalStemCellDampingConstant(1.0), 
      mMyoepithelialStemCellDampingConstant(1.0)
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
    // Determine if cell is luminal, myoepithelial (if not, assume it is stem cell)
    CellPtr p_cell = this->GetCellUsingLocationIndex(nodeIndex);
    bool cell_is_luminal = p_cell->template HasCellProperty<LuminalCellProperty>();
    bool cell_is_myoepithelial = p_cell->template HasCellProperty<MyoepithelialCellProperty>();
    bool cell_is_luminal_stem = p_cell->template HasCellProperty<LuminalStemCellProperty>();

    // Determine if cell expresses b1 and/or b4 integrin
    bool cell_b1_expn = true;
    bool cell_b4_expn = true;
    
    if (cell_is_luminal)
    {
        CellPropertyCollection collection = p_cell->rGetCellPropertyCollection().GetProperties<LuminalCellProperty>();
        boost::shared_ptr<LuminalCellProperty> p_prop = boost::static_pointer_cast<LuminalCellProperty>(collection.GetProperty());
        cell_b1_expn = p_prop->GetB1IntegrinExpression();
        cell_b4_expn = p_prop->GetB4IntegrinExpression();
    }
    else if (cell_is_myoepithelial)
    {
        CellPropertyCollection collection = p_cell->rGetCellPropertyCollection().GetProperties<MyoepithelialCellProperty>();
        boost::shared_ptr<MyoepithelialCellProperty> p_prop = boost::static_pointer_cast<MyoepithelialCellProperty>(collection.GetProperty());
        cell_b1_expn = p_prop->GetB1IntegrinExpression();
        cell_b4_expn = p_prop->GetB4IntegrinExpression();
    }
    else if (cell_is_luminal_stem)// if cell is luminal stem cell
    {
        CellPropertyCollection collection = p_cell->rGetCellPropertyCollection().GetProperties<LuminalStemCellProperty>();
        boost::shared_ptr<LuminalStemCellProperty> p_prop = boost::static_pointer_cast<LuminalStemCellProperty>(collection.GetProperty());
        cell_b1_expn = p_prop->GetB1IntegrinExpression();
        cell_b4_expn = p_prop->GetB4IntegrinExpression();
    }
    else // if cell is myoepithelial stem cell
    {
        CellPropertyCollection collection = p_cell->rGetCellPropertyCollection().GetProperties<MyoepithelialStemCellProperty>();
        boost::shared_ptr<MyoepithelialStemCellProperty> p_prop = boost::static_pointer_cast<MyoepithelialStemCellProperty>(collection.GetProperty());
        cell_b1_expn = p_prop->GetB1IntegrinExpression();
        cell_b4_expn = p_prop->GetB4IntegrinExpression();
    }
    
    if (cell_is_luminal) // if cell is luminal
    {
        if (cell_b1_expn && cell_b4_expn)
        {
            return 1.0*mLuminalCellDampingConstant;
        }
        else if (cell_b1_expn || cell_b4_expn)
        {
            return 0.5*mLuminalCellDampingConstant;
        }
        else
        {
            return 1.0;
        }
    }
    else if (cell_is_myoepithelial) // if cell is myoepithelial
    {
        if (cell_b1_expn && cell_b4_expn)
        {
            return 1.0*mMyoepithelialCellDampingConstant;
        }
        else if (cell_b1_expn || cell_b4_expn)
        {
            return 0.5*mMyoepithelialCellDampingConstant;
        }
        else
        {
            return 1.0;
        }
    }
    else if (cell_is_luminal_stem)// if cell is luminal stem cell
    {
        if (cell_b1_expn && cell_b4_expn)
        {
            return 1.0*mLuminalStemCellDampingConstant;
        }
        else if (cell_b1_expn || cell_b4_expn)
        {
            return 0.5*mLuminalStemCellDampingConstant;
        }
        else
        {
            return 1.0;
        }
    }
    else // if cell is myoepithelial stem cell
    {
        if (cell_b1_expn && cell_b4_expn)
        {
            return 1.0*mMyoepithelialStemCellDampingConstant;
        }
        else if (cell_b1_expn || cell_b4_expn)
        {
            return 0.5*mMyoepithelialStemCellDampingConstant;
        }
        else
        {
            return 1.0;
        }
    }
}

template<unsigned DIM>
void NodeBasedCellPopulationWithVariableDamping<DIM>::AddForceContribution(AbstractCellPopulation<DIM>& rCellPopulation)
{
    // Helper variable that is a static cast of the cell population
    NodeBasedCellPopulation<DIM>* p_cell_population = static_cast<NodeBasedCellPopulation<DIM>*>(&rCellPopulation);
    
    // Iterate over nodes in the cell population
    for (unsigned node_index=0; node_index<p_cell_population->GetNumNodes(); node_index++)
    {
       double cell_height = p_cell_population->GetNode(node_index)->rGetLocation()[2];

        c_vector<double, DIM> force_contribution;
        for (unsigned i=0; i<DIM; i++)
        {
        force_contribution[0] = 0.0;
        force_contribution[1] = 0.0;
        force_contribution[2] = (mLuminalCellDampingConstant*cell_height) && (mMyoepithelialCellDampingConstant*cell_height);
        }
        rCellPopulation.GetNode(node_index)->AddAppliedForceContribution(force_contribution);
    }
}      

template<unsigned DIM>
double NodeBasedCellPopulationWithVariableDamping<DIM>::GetLuminalCellDampingConstant()
{
    return mLuminalCellDampingConstant;
}

template<unsigned DIM>
void NodeBasedCellPopulationWithVariableDamping<DIM>::SetLuminalCellDampingConstant(double luminalCellDampingConstant)
{
    mLuminalCellDampingConstant = luminalCellDampingConstant;
}

template<unsigned DIM>
double NodeBasedCellPopulationWithVariableDamping<DIM>::GetMyoepithelialCellDampingConstant()
{
    return mMyoepithelialCellDampingConstant;
}

template<unsigned DIM>
void NodeBasedCellPopulationWithVariableDamping<DIM>::SetMyoepithelialCellDampingConstant(double myoepithelialCellDampingConstant)
{
    mMyoepithelialCellDampingConstant = myoepithelialCellDampingConstant;
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
