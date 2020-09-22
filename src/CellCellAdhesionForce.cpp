#include "CellCellAdhesionForce.hpp"
#include "LuminalCellProperty.hpp"
#include "MyoepithelialCellProperty.hpp"

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
CellCellAdhesionForce<ELEMENT_DIM, SPACE_DIM>::CellCellAdhesionForce()
   : GeneralisedLinearSpringForce<ELEMENT_DIM, SPACE_DIM>(),
     mHomotypicLabelledSpringConstantMultiplier(1.0),
     mHeterotypicSpringConstantMultiplier(1.0)
{
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
double CellCellAdhesionForce<ELEMENT_DIM, SPACE_DIM>::VariableSpringConstantMultiplicationFactor(
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
        // Determine which (if any) of the cells corresponding to these nodes are luminal or myoepithelial
        CellPtr p_cell_A = rCellPopulation.GetCellUsingLocationIndex(nodeAGlobalIndex);
        bool cell_A_is_luminal = p_cell_A->template HasCellProperty<LuminalCellProperty>();

        CellPtr p_cell_B = rCellPopulation.GetCellUsingLocationIndex(nodeBGlobalIndex);
        bool cell_B_is_myoepithelial = p_cell_B->template HasCellProperty<MyoepithelialCellProperty>();

        // For heterotypic interactions, scale the spring constant by mHeterotypicSpringConstantMultiplier
        if (cell_A_is_luminal != cell_B_is_myoepithelial)
        {
            return mHeterotypicSpringConstantMultiplier;
        }
        else
        {
            // For homotypic interactions between luminal cells, scale the spring constant by mHomotypicLabelledSpringConstantMultiplier
            if (cell_A_is_luminal)
            {
                return mHomotypicLabelledSpringConstantMultiplier;
            }
            else
            {
                // For homotypic interactions between myoepithelial cells, leave the spring constant unchanged from its normal value
                return 1.0;
            }
        }
    }
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
double CellCellAdhesionForce<ELEMENT_DIM, SPACE_DIM>::GetHomotypicLabelledSpringConstantMultiplier()
{
    return mHomotypicLabelledSpringConstantMultiplier;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void CellCellAdhesionForce<ELEMENT_DIM, SPACE_DIM>::SetHomotypicLabelledSpringConstantMultiplier(double labelledSpringConstantMultiplier)
{
    assert(labelledSpringConstantMultiplier > 0.0);
    mHomotypicLabelledSpringConstantMultiplier = labelledSpringConstantMultiplier;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
double CellCellAdhesionForce<ELEMENT_DIM, SPACE_DIM>::GetHeterotypicSpringConstantMultiplier()
{
    return mHeterotypicSpringConstantMultiplier;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void CellCellAdhesionForce<ELEMENT_DIM, SPACE_DIM>::SetHeterotypicSpringConstantMultiplier(double heterotypicSpringConstantMultiplier)
{
    assert(heterotypicSpringConstantMultiplier > 0.0);
    mHeterotypicSpringConstantMultiplier = heterotypicSpringConstantMultiplier;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void CellCellAdhesionForce<ELEMENT_DIM, SPACE_DIM>::OutputForceParameters(out_stream& rParamsFile)
{
    *rParamsFile << "\t\t\t<HomotypicLabelledSpringConstantMultiplier>" << mHomotypicLabelledSpringConstantMultiplier << "</HomotypicLabelledSpringConstantMultiplier>\n";
    *rParamsFile << "\t\t\t<HeterotypicSpringConstantMultiplier>" << mHeterotypicSpringConstantMultiplier << "</HeterotypicSpringConstantMultiplier>\n";

    // Call direct parent class
    GeneralisedLinearSpringForce<ELEMENT_DIM, SPACE_DIM>::OutputForceParameters(rParamsFile);
}

// Explicit instantiation
template class CellCellAdhesionForce<1,1>;
template class CellCellAdhesionForce<1,2>;
template class CellCellAdhesionForce<2,2>;
template class CellCellAdhesionForce<1,3>;
template class CellCellAdhesionForce<2,3>;
template class CellCellAdhesionForce<3,3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_ALL_DIMS(CellCellAdhesionForce)
