#include "DifferentialAdhesionLinearSpringForce.hpp"
#include "NodeBasedCellPopulationWithParticles.hpp"

#include "LuminalCellProperty.hpp"
#include "MyoepithelialCellProperty.hpp"
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
        // Determine if cell A is luminal (if not, assume it is myoepithelial)
        CellPtr p_cell_A = rCellPopulation.GetCellUsingLocationIndex(nodeAGlobalIndex);
        bool cell_A_is_luminal = p_cell_A->template HasCellProperty<LuminalCellProperty>();

        // Determine if cell B is luminal (if not, assume it is myoepithelial)
        CellPtr p_cell_B = rCellPopulation.GetCellUsingLocationIndex(nodeBGlobalIndex);
        bool cell_B_is_luminal = p_cell_B->template HasCellProperty<LuminalCellProperty>();
        
        // For heterotypic interactions, scale the spring constant by mHeterotypicSpringConstantMultiplier
        if (cell_A_is_luminal != cell_B_is_luminal)
        {
            return mHeterotypicSpringConstantMultiplier;
        }
        else
        {
            // For homotypic interactions between luminal and myoepithlelial cells, scale the spring constant by mHomotypicLabelledSpringConstantMultiplier
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
