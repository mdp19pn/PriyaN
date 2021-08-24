#include "CellECMAdhesionForce.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "LuminalCellProperty.hpp"
#include "MyoepithelialCellProperty.hpp"

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
CellECMAdhesionForce<ELEMENT_DIM, SPACE_DIM>::CellECMAdhesionForce()
   : GeneralisedLinearSpringForce<ELEMENT_DIM, SPACE_DIM>(),
     mHomotypicLabelledSpringConstantMultiplier(1.0),
     mHeterotypicSpringConstantMultiplier(1.0)
{
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
double CellECMAdhesionForce<ELEMENT_DIM, SPACE_DIM>::VariableSpringConstantMultiplicationFactor(
    unsigned nodeAGlobalIndex,
    unsigned nodeBGlobalIndex,
    AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>& rCellPopulation,
    bool isCloserThanRestLength)
{
    if (isCloserThanRestLength)
    {
        return 1.0;
    }
    else 
    {
        // Node<SPACE_DIM>* p_node_a = rCellPopulation.GetNode(nodeAGlobalIndex);
        // Node<SPACE_DIM>* p_node_b = rCellPopulation.GetNode(nodeBGlobalIndex);

        // std::vector<unsigned> ECM_node;

        // if (!p_node_a->IsParticle())
        // {
        //     // Determine if cell A is luminal (if not, assume it is myoepithelial)
        //     CellPtr p_cell_A = rCellPopulation.GetCellUsingLocationIndex(nodeAGlobalIndex);
        //     bool cell_A_is_luminal = p_cell_A->template HasCellProperty<LuminalCellProperty>();
        
        //     // Determine if cell expresses b1 and/or b4 integrin
        //     bool cell_A_b1_expn = true;
        //     bool cell_A_b4_expn = true;

        //     if (cell_A_is_luminal)
        //     {
        //         CellPropertyCollection collection = p_cell_A->rGetCellPropertyCollection().GetProperties<LuminalCellProperty>();
        //         boost::shared_ptr<LuminalCellProperty> p_prop_A = boost::static_pointer_cast<LuminalCellProperty>(collection.GetProperty());
        //         cell_A_b1_expn = p_prop_A->GetB1IntegrinExpression();
        //         cell_A_b4_expn = p_prop_A->GetB4IntegrinExpression();
        //     }
        //     else 
        //     {
        //         CellPropertyCollection collection = p_cell_A->rGetCellPropertyCollection().GetProperties<MyoepithelialCellProperty>();
        //         boost::shared_ptr<MyoepithelialCellProperty> p_prop_A = boost::static_pointer_cast<MyoepithelialCellProperty>(collection.GetProperty());
        //         cell_A_b1_expn = p_prop_A->GetB1IntegrinExpression();
        //         cell_A_b4_expn = p_prop_A->GetB4IntegrinExpression();
        //     }
        // }
        // else // node is particle
        // {
        //     ECM_node.push_back(nodeAGlobalIndex);
        // }

        // if (!p_node_b->IsParticle())
        // {
        //     // Determine if cell B is luminal (if not, assume it is myoepithelial)
        //     CellPtr p_cell_B = rCellPopulation.GetCellUsingLocationIndex(nodeBGlobalIndex);
        //     bool cell_B_is_luminal = p_cell_B->template HasCellProperty<LuminalCellProperty>();
        
        //     // Determine if cell expresses b1 and/or b4 integrin
        //     bool cell_B_b1_expn = true;
        //     bool cell_B_b4_expn = true;

        //     if (cell_B_is_luminal)
        //     {
        //         CellPropertyCollection collection = p_cell_B->rGetCellPropertyCollection().GetProperties<LuminalCellProperty>();
        //         boost::shared_ptr<LuminalCellProperty> p_prop_B = boost::static_pointer_cast<LuminalCellProperty>(collection.GetProperty());
        //         cell_B_b1_expn = p_prop_B->GetB1IntegrinExpression();
        //         cell_B_b4_expn = p_prop_B->GetB4IntegrinExpression();
        //     }
        //     else 
        //     {
        //         CellPropertyCollection collection = p_cell_B->rGetCellPropertyCollection().GetProperties<MyoepithelialCellProperty>();
        //         boost::shared_ptr<MyoepithelialCellProperty> p_prop_B = boost::static_pointer_cast<MyoepithelialCellProperty>(collection.GetProperty());
        //         cell_B_b1_expn = p_prop_B->GetB1IntegrinExpression();
        //         cell_B_b4_expn = p_prop_B->GetB4IntegrinExpression();
        //     }
        // }
        // else // node is particle
        // {
        //     ECM_node.push_back(nodeBGlobalIndex);
        // }

        // // For heterotypic interactions, scale the spring constant by mHeterotypicSpringConstantMultiplier
        // if (cell_A_is_luminal && p_node_b->IsParticle())
        // {
        //     if (cell_A_b1_expn && cell_A_b4_expn)
        //     {
        //         return 2.0*mHeterotypicSpringConstantMultiplier;
        //     }
        //     else if (cell_A_b1_expn != cell_A_b4_expn)
        //     {
        //         return 1.0*mHeterotypicSpringConstantMultiplier;
        //     }
        //     else
        //     {
        //         return 0.5;
        //     }
        // }
        // else if (!cell_A_is_luminal && p_node_b->IsParticle())
        // {
        //     if (cell_A_b1_expn && cell_A_b4_expn)
        //     {
        //         return 4.0*mHeterotypicSpringConstantMultiplier;
        //     }
        //     else if (cell_A_b1_expn != cell_A_b4_expn)
        //     {
        //         return 2.0*mHeterotypicSpringConstantMultiplier;
        //     }
        //     else
        //     {
        //         return 1.0;
        //     }
        // }
        // else
        // {
        //     // For homotypic interactions between cells, scale the spring constant by mHomotypicLabelledSpringConstantMultiplier
        //     if (cell_A_is_luminal)
        //     {
        //         return mHomotypicLabelledSpringConstantMultiplier;
        //     }
        //     else if (!cell_A_is_luminal)
        //     {
        //         // For homotypic interactions between myoepitehlial cells, leave the spring constant unchanged from its normal value
        //         return mHomotypicLabelledSpringConstantMultiplier;
        //     }
        //     else if (p_node_a->IsParticle())
        //     {
        //         // For homotypic interactions between myoepitehlial cells, leave the spring constant unchanged from its normal value
        //         return mHomotypicLabelledSpringConstantMultiplier;
        //     }
        // }
        return 1.0;
    }
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
double CellECMAdhesionForce<ELEMENT_DIM, SPACE_DIM>::GetHomotypicLabelledSpringConstantMultiplier()
{
    return mHomotypicLabelledSpringConstantMultiplier;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void CellECMAdhesionForce<ELEMENT_DIM, SPACE_DIM>::SetHomotypicLabelledSpringConstantMultiplier(double labelledSpringConstantMultiplier)
{
    assert(labelledSpringConstantMultiplier > 0.0);
    mHomotypicLabelledSpringConstantMultiplier = labelledSpringConstantMultiplier;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
double CellECMAdhesionForce<ELEMENT_DIM, SPACE_DIM>::GetHeterotypicSpringConstantMultiplier()
{
    return mHeterotypicSpringConstantMultiplier;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void CellECMAdhesionForce<ELEMENT_DIM, SPACE_DIM>::SetHeterotypicSpringConstantMultiplier(double heterotypicSpringConstantMultiplier)
{
    assert(heterotypicSpringConstantMultiplier > 0.0);
    mHeterotypicSpringConstantMultiplier = heterotypicSpringConstantMultiplier;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void CellECMAdhesionForce<ELEMENT_DIM, SPACE_DIM>::OutputForceParameters(out_stream& rParamsFile)
{
    *rParamsFile << "\t\t\t<HomotypicLabelledSpringConstantMultiplier>" << mHomotypicLabelledSpringConstantMultiplier << "</HomotypicLabelledSpringConstantMultiplier>\n";
    *rParamsFile << "\t\t\t<HeterotypicSpringConstantMultiplier>" << mHeterotypicSpringConstantMultiplier << "</HeterotypicSpringConstantMultiplier>\n";

    // Call direct parent class
    GeneralisedLinearSpringForce<ELEMENT_DIM, SPACE_DIM>::OutputForceParameters(rParamsFile);
}

// Explicit instantiation
template class CellECMAdhesionForce<1,1>;
template class CellECMAdhesionForce<1,2>;
template class CellECMAdhesionForce<2,2>;
template class CellECMAdhesionForce<1,3>;
template class CellECMAdhesionForce<2,3>;
template class CellECMAdhesionForce<3,3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_ALL_DIMS(CellECMAdhesionForce)