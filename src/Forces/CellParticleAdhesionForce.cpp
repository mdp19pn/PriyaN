#include "CellParticleAdhesionForce.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "NodeBasedCellPopulationWithParticles.hpp"
#include "LuminalCellProperty.hpp"
#include "MyoepithelialCellProperty.hpp"
#include "LuminalStemCellProperty.hpp"
#include "MyoepithelialStemCellProperty.hpp"
#include "Debug.hpp"

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
CellParticleAdhesionForce<ELEMENT_DIM, SPACE_DIM>::CellParticleAdhesionForce()
   : GeneralisedLinearSpringForce<ELEMENT_DIM, SPACE_DIM>(),
     mHomotypicLabelledSpringConstantMultiplier(1.0),
     mHeterotypicSpringConstantMultiplier(1.0)
{
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
double CellParticleAdhesionForce<ELEMENT_DIM, SPACE_DIM>::VariableSpringConstantMultiplicationFactor(
    unsigned nodeAGlobalIndex,
    unsigned nodeBGlobalIndex,
    AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>& rCellPopulation,
    bool isCloserThanRestLength)
{
    if (!isCloserThanRestLength)
    {

        return 1.0;
    }
    else
    {

        Node<SPACE_DIM>* p_node_a = rCellPopulation.GetNode(nodeAGlobalIndex);
        Node<SPACE_DIM>* p_node_b = rCellPopulation.GetNode(nodeBGlobalIndex);
        
        // Create a vector to record the nodes corresponding only to particles (ECM Nodes)
        std::vector<unsigned> ECM_node;
        return 1.0;

        if (!p_node_a->IsParticle())
        {
            // Determine if cell A is luminal (if not, assume it is myoepithelial)
            CellPtr p_cell_A = rCellPopulation.GetCellUsingLocationIndex(nodeAGlobalIndex);
            bool cell_A_is_luminal = p_cell_A->template HasCellProperty<LuminalCellProperty>();
            bool cell_A_is_myoepithelial = p_cell_A->template HasCellProperty<MyoepithelialCellProperty>();
            bool cell_A_is_luminal_stem = p_cell_A->template HasCellProperty<LuminalStemCellProperty>();
            bool cell_A_is_myo_stem = p_cell_A->template HasCellProperty<MyoepithelialStemCellProperty>();

            // Determine if cell expresses b1 and/or b4 integrin
            bool cell_A_b1_expn = true;
            bool cell_A_b4_expn = true;

            if (cell_A_is_luminal)
            {
                CellPropertyCollection collection = p_cell_A->rGetCellPropertyCollection().GetProperties<LuminalCellProperty>();
                boost::shared_ptr<LuminalCellProperty> p_prop_A = boost::static_pointer_cast<LuminalCellProperty>(collection.GetProperty());
                cell_A_b1_expn = p_prop_A->GetB1IntegrinExpression();
                cell_A_b4_expn = p_prop_A->GetB4IntegrinExpression();
            }
            else if (cell_A_is_myoepithelial)
            {
                CellPropertyCollection collection =  p_cell_A->rGetCellPropertyCollection().GetProperties<MyoepithelialCellProperty>();
                boost::shared_ptr<MyoepithelialCellProperty> p_prop_A = boost::static_pointer_cast<MyoepithelialCellProperty>(collection.GetProperty());
                cell_A_b1_expn = p_prop_A->GetB1IntegrinExpression();
                cell_A_b4_expn = p_prop_A->GetB4IntegrinExpression();
            }
            else if (cell_A_is_luminal_stem)
            {
                CellPropertyCollection collection =  p_cell_A->rGetCellPropertyCollection().GetProperties<LuminalStemCellProperty>();
                boost::shared_ptr<LuminalStemCellProperty> p_prop_A = boost::static_pointer_cast<LuminalStemCellProperty>(collection.GetProperty());
                cell_A_b1_expn = p_prop_A->GetB1IntegrinExpression();
                cell_A_b4_expn = p_prop_A->GetB4IntegrinExpression();
            }
            else if (cell_A_is_myo_stem)
            {
                CellPropertyCollection collection =  p_cell_A->rGetCellPropertyCollection().GetProperties<MyoepithelialStemCellProperty>();
                boost::shared_ptr<MyoepithelialStemCellProperty> p_prop_A = boost::static_pointer_cast<MyoepithelialStemCellProperty>(collection.GetProperty());
                cell_A_b1_expn = p_prop_A->GetB1IntegrinExpression();
                cell_A_b4_expn = p_prop_A->GetB4IntegrinExpression();
            }

            // For heterotypic interactions, scale the spring constant by mHeterotypicSpringConstantMultiplier
            if (cell_A_is_luminal && p_node_b->IsParticle())
            {
                if (cell_A_b1_expn && cell_A_b4_expn)
                {
                    return 2.0*mHeterotypicSpringConstantMultiplier;
                }
                else if (cell_A_b1_expn != cell_A_b4_expn)
                {
                    return 1.0*mHeterotypicSpringConstantMultiplier;
                }
                else
                {
                    return 0.5;
                }
            }
            else if (cell_A_is_myoepithelial && p_node_b->IsParticle())
            {
                if (cell_A_b1_expn && cell_A_b4_expn)
                {
                    return 4.0*mHeterotypicSpringConstantMultiplier;
                }
                else if (cell_A_b1_expn != cell_A_b4_expn)
                {
                    return 2.0*mHeterotypicSpringConstantMultiplier;
                }
                else
                {
                    return 1.0;
                }
            }
            else if (cell_A_is_luminal_stem && p_node_b->IsParticle())
            {
                if (cell_A_b1_expn && cell_A_b4_expn)
                {
                    return 2.0*mHeterotypicSpringConstantMultiplier;
                }
                else if (cell_A_b1_expn != cell_A_b4_expn)
                {
                    return 1.0*mHeterotypicSpringConstantMultiplier;
                }
                else
                {
                    return 0.5;
                }
            }
            else if (cell_A_is_myo_stem && p_node_b->IsParticle())
            {
                if (cell_A_b1_expn && cell_A_b4_expn)
                {
                    return 4.0*mHeterotypicSpringConstantMultiplier;
                }
                else if (cell_A_b1_expn != cell_A_b4_expn)
                {
                    return 2.0*mHeterotypicSpringConstantMultiplier;
                }
                else
                {
                    return 1.0;
                }
            }
            else
            {
                // For homotypic interactions between cells, scale the spring constant by mHomotypicLabelledSpringConstantMultiplier
                if (cell_A_is_luminal)
                {
                    return mHomotypicLabelledSpringConstantMultiplier;
                }
                else if (cell_A_is_myoepithelial)
                {
                    return mHomotypicLabelledSpringConstantMultiplier;
                }
                else if (cell_A_is_luminal_stem)
                {
                    return mHomotypicLabelledSpringConstantMultiplier;
                }
                else if (cell_A_is_myo_stem)
                {
                    return mHomotypicLabelledSpringConstantMultiplier;
                }
                else 
                {
                    return 1.0;
                }
            }
        }
        else // node is particle
        {
            ECM_node.push_back(nodeAGlobalIndex);
            return 1.0;
        }

        if (!p_node_b->IsParticle())
        {
            // Determine if cell B is luminal (if not, assume it is myoepithelial)
            CellPtr p_cell_B = rCellPopulation.GetCellUsingLocationIndex(nodeBGlobalIndex);
            bool cell_B_is_luminal = p_cell_B->template HasCellProperty<LuminalCellProperty>();
            bool cell_B_is_myoepithelial = p_cell_B->template HasCellProperty<MyoepithelialCellProperty>();
            bool cell_B_is_luminal_stem = p_cell_B->template HasCellProperty<LuminalStemCellProperty>();
            bool cell_B_is_myo_stem = p_cell_B->template HasCellProperty<MyoepithelialStemCellProperty>();

            // Determine if cell expresses b1 and/or b4 integrin
            bool cell_B_b1_expn = true;
            bool cell_B_b4_expn = true;

            if (cell_B_is_luminal)
            {
                CellPropertyCollection collection = p_cell_B->rGetCellPropertyCollection().GetProperties<LuminalCellProperty>();
                boost::shared_ptr<LuminalCellProperty> p_prop_B = boost::static_pointer_cast<LuminalCellProperty>(collection.GetProperty());
                cell_B_b1_expn = p_prop_B->GetB1IntegrinExpression();
                cell_B_b4_expn = p_prop_B->GetB4IntegrinExpression();
            }
            else if (cell_B_is_myoepithelial)
            {
                CellPropertyCollection collection =  p_cell_B->rGetCellPropertyCollection().GetProperties<MyoepithelialCellProperty>();
                boost::shared_ptr<MyoepithelialCellProperty> p_prop_B = boost::static_pointer_cast<MyoepithelialCellProperty>(collection.GetProperty());
                cell_B_b1_expn = p_prop_B->GetB1IntegrinExpression();
                cell_B_b4_expn = p_prop_B->GetB4IntegrinExpression();
            }
            else if (cell_B_is_luminal_stem)
            {
                CellPropertyCollection collection =  p_cell_B->rGetCellPropertyCollection().GetProperties<LuminalStemCellProperty>();
                boost::shared_ptr<LuminalStemCellProperty> p_prop_B= boost::static_pointer_cast<LuminalStemCellProperty>(collection.GetProperty());
                cell_B_b1_expn = p_prop_B->GetB1IntegrinExpression();
                cell_B_b4_expn = p_prop_B->GetB4IntegrinExpression();
            }
            else if (cell_B_is_myo_stem)
            {
                CellPropertyCollection collection =  p_cell_B->rGetCellPropertyCollection().GetProperties<MyoepithelialStemCellProperty>();
                boost::shared_ptr<MyoepithelialStemCellProperty> p_prop_B = boost::static_pointer_cast<MyoepithelialStemCellProperty>(collection.GetProperty());
                cell_B_b1_expn = p_prop_B->GetB1IntegrinExpression();
                cell_B_b4_expn = p_prop_B->GetB4IntegrinExpression();
            }
            // For heterotypic interactions, scale the spring constant by mHeterotypicSpringConstantMultiplier
            if (cell_B_is_luminal && p_node_a->IsParticle())
            {
                if (cell_B_b1_expn && cell_B_b4_expn)
                {
                    return 2.0*mHeterotypicSpringConstantMultiplier;
                }
                else if (cell_B_b1_expn != cell_B_b4_expn)
                {
                    return 1.0*mHeterotypicSpringConstantMultiplier;
                }
                else
                {
                    return 0.5;
                }
            }
            else if (cell_B_is_myoepithelial && p_node_a->IsParticle())
            {
                if (cell_B_b1_expn && cell_B_b4_expn)
                {
                    return 4.0*mHeterotypicSpringConstantMultiplier;
                }
                else if (cell_B_b1_expn != cell_B_b4_expn)
                {
                    return 2.0*mHeterotypicSpringConstantMultiplier;
                }
                else
                {
                    return 1.0;
                }
            }
            else if (cell_B_is_luminal_stem && p_node_a->IsParticle())
            {
                if (cell_B_b1_expn && cell_B_b4_expn)
                {
                    return 2.0*mHeterotypicSpringConstantMultiplier;
                }
                else if (cell_B_b1_expn != cell_B_b4_expn)
                {
                    return 1.0*mHeterotypicSpringConstantMultiplier;
                }
                else
                {
                    return 0.5;
                }
            }
            else if (cell_B_is_myo_stem && p_node_a->IsParticle())
            {
                if (cell_B_b1_expn && cell_B_b4_expn)
                {
                    return 4.0*mHeterotypicSpringConstantMultiplier;
                }
                else if (cell_B_b1_expn != cell_B_b4_expn)
                {
                    return 2.0*mHeterotypicSpringConstantMultiplier;
                }
                else
                {
                    return 1.0;
                }
            }
            else
            {
                // For homotypic interactions between cells, scale the spring constant by mHomotypicLabelledSpringConstantMultiplier
                if (cell_B_is_luminal)
                {
                    return mHomotypicLabelledSpringConstantMultiplier;
                }
                else if (cell_B_is_myoepithelial)
                {
                    return mHomotypicLabelledSpringConstantMultiplier;
                }
                else if (cell_B_is_luminal_stem)
                {
                    return mHomotypicLabelledSpringConstantMultiplier;
                }
                else if (cell_B_is_myo_stem)
                {
                    return mHomotypicLabelledSpringConstantMultiplier;
                }
                else 
                {
                    return 1.0;
                }
            }
        }
        else // node is particle
        {
            ECM_node.push_back(nodeBGlobalIndex);
            return 1.0;
        }
    }
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void CellParticleAdhesionForce<ELEMENT_DIM, SPACE_DIM>::AddForceContribution(AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>& rCellPopulation)
{
  AbstractCentreBasedCellPopulation<ELEMENT_DIM,SPACE_DIM>* p_static_cast_cell_population = static_cast<AbstractCentreBasedCellPopulation<ELEMENT_DIM,SPACE_DIM>*>(&rCellPopulation);

  std::vector< std::pair<Node<SPACE_DIM>*, Node<SPACE_DIM>* > >& r_node_pairs = p_static_cast_cell_population->rGetNodePairs();
  for (typename std::vector< std::pair<Node<SPACE_DIM>*, Node<SPACE_DIM>* > >::iterator iter = r_node_pairs.begin();
      iter != r_node_pairs.end();
      iter++)
    {
        std::pair<Node<SPACE_DIM>*, Node<SPACE_DIM>* > pair = *iter;

        Node<SPACE_DIM>* p_node_a = pair.first;
        Node<SPACE_DIM>* p_node_b = pair.second;

        // Calculate the force between nodes
        c_vector<double, SPACE_DIM> force;
        force[0]=-0.5;
        force[1]=-0.5;
        force[2]=-0.5;
        
        // Add the force contribution to each node
        p_node_a->AddAppliedForceContribution(force);
        p_node_b->AddAppliedForceContribution(force);
    }
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
double CellParticleAdhesionForce<ELEMENT_DIM, SPACE_DIM>::GetHomotypicLabelledSpringConstantMultiplier()
{
    return mHomotypicLabelledSpringConstantMultiplier;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void CellParticleAdhesionForce<ELEMENT_DIM, SPACE_DIM>::SetHomotypicLabelledSpringConstantMultiplier(double labelledSpringConstantMultiplier)
{
    assert(labelledSpringConstantMultiplier > 0.0);
    mHomotypicLabelledSpringConstantMultiplier = labelledSpringConstantMultiplier;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
double CellParticleAdhesionForce<ELEMENT_DIM, SPACE_DIM>::GetHeterotypicSpringConstantMultiplier()
{
    return mHeterotypicSpringConstantMultiplier;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void CellParticleAdhesionForce<ELEMENT_DIM, SPACE_DIM>::SetHeterotypicSpringConstantMultiplier(double heterotypicSpringConstantMultiplier)
{
    assert(heterotypicSpringConstantMultiplier > 0.0);
    mHeterotypicSpringConstantMultiplier = heterotypicSpringConstantMultiplier;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void CellParticleAdhesionForce<ELEMENT_DIM, SPACE_DIM>::OutputForceParameters(out_stream& rParamsFile)
{
    *rParamsFile << "\t\t\t<HomotypicLabelledSpringConstantMultiplier>" << mHomotypicLabelledSpringConstantMultiplier << "</HomotypicLabelledSpringConstantMultiplier>\n";
    *rParamsFile << "\t\t\t<HeterotypicSpringConstantMultiplier>" << mHeterotypicSpringConstantMultiplier << "</HeterotypicSpringConstantMultiplier>\n";
    // Call direct parent class
    GeneralisedLinearSpringForce<ELEMENT_DIM, SPACE_DIM>::OutputForceParameters(rParamsFile);
}

// Explicit instantiation
template class CellParticleAdhesionForce<1,1>;
template class CellParticleAdhesionForce<1,2>;
template class CellParticleAdhesionForce<2,2>;
template class CellParticleAdhesionForce<1,3>;
template class CellParticleAdhesionForce<2,3>;
template class CellParticleAdhesionForce<3,3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_ALL_DIMS(CellParticleAdhesionForce)