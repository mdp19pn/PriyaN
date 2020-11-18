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
        // Determine if cell A is luminal (if not, assume it is myoepithelial)
        CellPtr p_cell_A = rCellPopulation.GetCellUsingLocationIndex(nodeAGlobalIndex);
        bool cell_A_is_luminal = p_cell_A->template HasCellProperty<LuminalCellProperty>();

        // Determine if cell A expresses b1 and/or b4 integrin
        bool cell_A_b1_expn = true;
        bool cell_A_b4_expn = true;
        
        if (cell_A_is_luminal)
        {
            CellPropertyCollection collection_A = p_cell_A->rGetCellPropertyCollection().GetProperties<LuminalCellProperty>();
            boost::shared_ptr<LuminalCellProperty> p_prop_A = boost::static_pointer_cast<LuminalCellProperty>(collection_A.GetProperty());
            cell_A_b1_expn = p_prop_A->GetB1IntegrinExpression();
            cell_A_b4_expn = p_prop_A->GetB4IntegrinExpression();
        }
        else
        {
            CellPropertyCollection collection_A = p_cell_A->rGetCellPropertyCollection().GetProperties<MyoepithelialCellProperty>();
            boost::shared_ptr<MyoepithelialCellProperty> p_prop_A = boost::static_pointer_cast<MyoepithelialCellProperty>(collection_A.GetProperty());
            cell_A_b1_expn = p_prop_A->GetB1IntegrinExpression();
            cell_A_b4_expn = p_prop_A->GetB4IntegrinExpression();
        }

        // Determine if cell B is luminal (if not, assume it is myoepithelial)
        CellPtr p_cell_B = rCellPopulation.GetCellUsingLocationIndex(nodeBGlobalIndex);
        bool cell_B_is_luminal = p_cell_B->template HasCellProperty<LuminalCellProperty>();

        // Determine if cell B expresses b1 and/or b4 integrin
        bool cell_B_b1_expn = true;
        bool cell_B_b4_expn = true;
        
        if (cell_B_is_luminal)
        {
        CellPropertyCollection collection_B = p_cell_B->rGetCellPropertyCollection().GetProperties<LuminalCellProperty>();
        boost::shared_ptr<LuminalCellProperty> p_prop_B = boost::static_pointer_cast<LuminalCellProperty>(collection_B.GetProperty());
        cell_B_b1_expn = p_prop_B->GetB1IntegrinExpression();
        cell_B_b4_expn = p_prop_B->GetB4IntegrinExpression();
        }
        else
        {
        CellPropertyCollection collection_B = p_cell_B->rGetCellPropertyCollection().GetProperties<MyoepithelialCellProperty>();
        boost::shared_ptr<MyoepithelialCellProperty> p_prop_B = boost::static_pointer_cast<MyoepithelialCellProperty>(collection_B.GetProperty());
        cell_B_b1_expn = p_prop_B->GetB1IntegrinExpression();
        cell_B_b4_expn = p_prop_B->GetB4IntegrinExpression();
        }

        // For heterotypic interactions, scale the spring constant by mHeterotypicSpringConstantMultiplier
        if (cell_A_is_luminal != cell_B_is_luminal)
        {
            if (cell_A_b1_expn && cell_A_b4_expn)
            {
                return 1.0*mHeterotypicSpringConstantMultiplier;
            }
            else if (cell_A_b1_expn != cell_A_b4_expn)
            {
                return 0.5*mHeterotypicSpringConstantMultiplier;
            }
            else
            {
                return 1.0;
            }
        }
        else
        {
            // For homotypic interactions between cells, scale the spring constant by mHomotypicLabelledSpringConstantMultiplier
            if (cell_A_b1_expn && cell_A_b4_expn)
            {
                return 1.0*mHomotypicLabelledSpringConstantMultiplier;
            }
            else if (cell_A_b1_expn != cell_A_b4_expn)
            {
                return 0.5*mHomotypicLabelledSpringConstantMultiplier;
            }   
            else
            {
                return 1.0; 
            }
        }
         
         if (cell_A_is_luminal != cell_B_is_luminal)
        {
            if (cell_B_b1_expn && cell_B_b4_expn)
            {
                return 1.0*mHeterotypicSpringConstantMultiplier;
            }
            else if (cell_B_b1_expn != cell_B_b4_expn)
            {
                return 0.5*mHeterotypicSpringConstantMultiplier;
            }
            else
            {
                return 1.0;
            }
        }
        else
        {
            // For homotypic interactions between cells, scale the spring constant by mHomotypicLabelledSpringConstantMultiplier
            if (cell_B_b1_expn && cell_B_b4_expn)
            {
                return 1.0*mHomotypicLabelledSpringConstantMultiplier;
            }
            else if (cell_B_b1_expn != cell_B_b4_expn)
            {
                return 0.5*mHomotypicLabelledSpringConstantMultiplier;
            }   
            else
            {
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
