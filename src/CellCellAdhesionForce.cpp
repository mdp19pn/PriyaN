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
        double cell_type = 0.0;
        double b1_expn = 0.0;
        double b4_expn = 0.0;

        // Determine which (if any) of the cells corresponding to these nodes are luminal or myoepithelial
        CellPtr p_cell_A = rCellPopulation.GetCellUsingLocationIndex(nodeAGlobalIndex);
        bool cell_A_is_luminal = p_cell_A->template HasCellProperty<LuminalCellProperty>();

        CellPtr p_cell_B = rCellPopulation.GetCellUsingLocationIndex(nodeBGlobalIndex);
        bool cell_B_is_myoepithelial = p_cell_B->template HasCellProperty<MyoepithelialCellProperty>();

        if (p_cell_A->HasCellProperty<LuminalCellProperty>())
        {
            CellPropertyCollection collection = p_cell_A->rGetCellPropertyCollection().GetProperties<LuminalCellProperty>();
            boost::shared_ptr<LuminalCellProperty> p_prop = boost::static_pointer_cast<LuminalCellProperty>(collection.GetProperty());
            b1_expn = double(p_prop->GetB1IntegrinExpression());
            b4_expn = double(p_prop->GetB4IntegrinExpression());
        }
        else if (p_cell_B->HasCellProperty<MyoepithelialCellProperty>())
        {
            cell_type = 1.0;
            CellPropertyCollection collection = p_cell_B->rGetCellPropertyCollection().GetProperties<MyoepithelialCellProperty>();
            boost::shared_ptr<MyoepithelialCellProperty> p_prop = boost::static_pointer_cast<MyoepithelialCellProperty>(collection.GetProperty());
            b1_expn = double(p_prop->GetB1IntegrinExpression());
            b4_expn = double(p_prop->GetB4IntegrinExpression());
        }

        // For heterotypic interactions, scale the spring constant by mHeterotypicSpringConstantMultiplier
        if (cell_A_is_luminal && cell_B_is_myoepithelial)
        {
            if ((b1_expn = 1.0) && (b4_expn = 1.0))
            {
                return 1.0*mHeterotypicSpringConstantMultiplier;
            }

            else if (((b1_expn = 1.0) && (b4_expn = 0.0)) || ((b1_expn = 0.0) && (b4_expn = 1.0)))
            {
                return 0.5*mHeterotypicSpringConstantMultiplier;
            }   

            else if ((b1_expn = 0.0) && (b4_expn = 0.0))
            {
                return 0.0;
            }
        }
        else
        {
            // For homotypic interactions between luminal cells, scale the spring constant by mHomotypicLabelledSpringConstantMultiplier
            if (cell_A_is_luminal != cell_B_is_myoepithelial)
            {
                if ((b1_expn = 1.0) && (b4_expn = 1.0))
                {
                    return 1.0*mHomotypicLabelledSpringConstantMultiplier;
                }
                else if (((b1_expn = 1.0) && (b4_expn = 0.0)) || ((b1_expn = 0.0) && (b4_expn = 1.0)))
                {
                    return 0.5*mHomotypicLabelledSpringConstantMultiplier;
                }   
                else if ((b1_expn = 0.0) && (b4_expn = 0.0))
                {
                    return 0.0;
                }
            }
            else if (cell_B_is_myoepithelial != cell_A_is_luminal)
            {
                if ((b1_expn = 1.0) && (b4_expn = 1.0))
                {
                    return 1.0*mHomotypicLabelledSpringConstantMultiplier;
                }
                else if (((b1_expn = 1.0) && (b4_expn = 0.0)) || ((b1_expn = 0.0) && (b4_expn = 1.0)))
                {
                    return 0.5*mHomotypicLabelledSpringConstantMultiplier;
                }   
                else if ((b1_expn = 0.0) && (b4_expn = 0.0))
                {
                    return 0.0;
                }
                // For homotypic interactions between myoepithelial cells, leave the spring constant unchanged from its normal value
                // return 1.0;
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
