#include "DifferentialAdhesionLinearSpringForce.hpp"
#include "NodeBasedCellPopulationWithParticles.hpp"
#include "CellLabel.hpp"
#include "Debug.hpp"

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
DifferentialAdhesionLinearSpringForce<ELEMENT_DIM, SPACE_DIM>::DifferentialAdhesionLinearSpringForce()
   : GeneralisedLinearSpringForce<ELEMENT_DIM, SPACE_DIM>(),
     mHomotypicLabelledSpringConstantMultiplier(1.0),
     mHeterotypicSpringConstantMultiplier(1.0)
{
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
double DifferentialAdhesionLinearSpringForce<ELEMENT_DIM, SPACE_DIM>::VariableSpringConstantMultiplicationFactor(
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
        Node<SPACE_DIM>* p_node_a = rCellPopulation.GetNode(nodeAGlobalIndex);
        Node<SPACE_DIM>* p_node_b = rCellPopulation.GetNode(nodeBGlobalIndex);

        // Determine which (if any) of the cells corresponding to these nodes are labelled
        CellPtr p_cell_A = rCellPopulation.GetCellUsingLocationIndex(nodeAGlobalIndex);
        bool cell_A_is_labelled = p_cell_A->template HasCellProperty<CellLabel>();

        CellPtr p_cell_B = rCellPopulation.GetCellUsingLocationIndex(nodeBGlobalIndex);
        bool cell_B_is_labelled = p_cell_B->template HasCellProperty<CellLabel>();
        
        // For heterotypic interactions, scale the spring constant by mHeterotypicSpringConstantMultiplier
        if ((!p_node_a->IsParticle() && cell_A_is_labelled) != (!p_node_b->IsParticle() && cell_B_is_labelled))
        {
            return mHeterotypicSpringConstantMultiplier;
        }
        else
        {
            // For homotypic interactions between labelled cells, scale the spring constant by mHomotypicLabelledSpringConstantMultiplier
            if (!p_node_a->IsParticle() && cell_A_is_labelled)
            {
                return mHomotypicLabelledSpringConstantMultiplier;
            }
            else
            {
                // For homotypic interactions between unlabelled cells, leave the spring constant unchanged from its normal value
                return 1.0;
            }
        }
    }
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
double DifferentialAdhesionLinearSpringForce<ELEMENT_DIM, SPACE_DIM>::GetHomotypicLabelledSpringConstantMultiplier()
{
    return mHomotypicLabelledSpringConstantMultiplier;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void DifferentialAdhesionLinearSpringForce<ELEMENT_DIM, SPACE_DIM>::SetHomotypicLabelledSpringConstantMultiplier(double labelledSpringConstantMultiplier)
{
    assert(labelledSpringConstantMultiplier > 0.0);
    mHomotypicLabelledSpringConstantMultiplier = labelledSpringConstantMultiplier;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
double DifferentialAdhesionLinearSpringForce<ELEMENT_DIM, SPACE_DIM>::GetHeterotypicSpringConstantMultiplier()
{
    return mHeterotypicSpringConstantMultiplier;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void DifferentialAdhesionLinearSpringForce<ELEMENT_DIM, SPACE_DIM>::SetHeterotypicSpringConstantMultiplier(double heterotypicSpringConstantMultiplier)
{
    assert(heterotypicSpringConstantMultiplier > 0.0);
    mHeterotypicSpringConstantMultiplier = heterotypicSpringConstantMultiplier;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void DifferentialAdhesionLinearSpringForce<ELEMENT_DIM, SPACE_DIM>::OutputForceParameters(out_stream& rParamsFile)
{
    *rParamsFile << "\t\t\t<HomotypicLabelledSpringConstantMultiplier>" << mHomotypicLabelledSpringConstantMultiplier << "</HomotypicLabelledSpringConstantMultiplier>\n";
    *rParamsFile << "\t\t\t<HeterotypicSpringConstantMultiplier>" << mHeterotypicSpringConstantMultiplier << "</HeterotypicSpringConstantMultiplier>\n";

    // Call direct parent class
    GeneralisedLinearSpringForce<ELEMENT_DIM, SPACE_DIM>::OutputForceParameters(rParamsFile);
}

// Explicit instantiation
template class DifferentialAdhesionLinearSpringForce<1,1>;
template class DifferentialAdhesionLinearSpringForce<1,2>;
template class DifferentialAdhesionLinearSpringForce<2,2>;
template class DifferentialAdhesionLinearSpringForce<1,3>;
template class DifferentialAdhesionLinearSpringForce<2,3>;
template class DifferentialAdhesionLinearSpringForce<3,3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_ALL_DIMS(DifferentialAdhesionLinearSpringForce)
