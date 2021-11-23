#include "LinearSpringForce.hpp"
#include "NodeBasedCellPopulationWithParticles.hpp"
#include "CellECMAdhesionForce.hpp"
#include "Debug.hpp"
#include "LuminalCellProperty.hpp"
#include "MyoepithelialCellProperty.hpp"
#include "LuminalStemCellProperty.hpp"
#include "MyoepithelialStemCellProperty.hpp"

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
LinearSpringForce<ELEMENT_DIM,SPACE_DIM>::LinearSpringForce()
   : AbstractTwoBodyInteractionForce<ELEMENT_DIM,SPACE_DIM>(),
     mCellCellSpringStiffness(15.0),
     mCellECMSpringStiffness(15.0),
     mECMECMSpringStiffness(15.0),          // denoted by mu in Meineke et al, 2001 (doi:10.1046/j.0960-7722.2001.00216.x)
     mMeinekeDivisionRestingSpringLength(0.5),
     mMeinekeSpringGrowthDuration(1.0),
     mHomotypicLabelledSpringConstantMultiplier(1.0),
     mHeterotypicSpringConstantMultiplier(1.0)
{
    if (SPACE_DIM == 1)
    {
        mCellCellSpringStiffness = 30.0;
        mCellECMSpringStiffness = 30.0;
        mECMECMSpringStiffness = 30.0;
    }
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
double LinearSpringForce<ELEMENT_DIM,SPACE_DIM>::VariableSpringConstantMultiplicationFactor(unsigned nodeAGlobalIndex,
                                                                                            unsigned nodeBGlobalIndex,
                                                                                            AbstractCellPopulation<ELEMENT_DIM,SPACE_DIM>& rCellPopulation,
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
                    // For homotypic interactions between myoepitehlial cells, leave the spring constant unchanged from its normal value
                    return mHomotypicLabelledSpringConstantMultiplier;
                }
                else if (cell_A_is_luminal_stem)
                {
                    // For homotypic interactions between myoepitehlial cells, leave the spring constant unchanged from its normal value
                    return mHomotypicLabelledSpringConstantMultiplier;
                }
                else if (cell_A_is_myo_stem)
                {
                    // For homotypic interactions between myoepitehlial cells, leave the spring constant unchanged from its normal value
                    return mHomotypicLabelledSpringConstantMultiplier;
                }
                else if (p_node_a->IsParticle())
                {
                    // For homotypic interactions between myoepitehlial cells, leave the spring constant unchanged from its normal value
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
                    // For homotypic interactions between myoepitehlial cells, leave the spring constant unchanged from its normal value
                    return mHomotypicLabelledSpringConstantMultiplier;
                }
                else if (cell_B_is_luminal_stem)
                {
                    // For homotypic interactions between myoepitehlial cells, leave the spring constant unchanged from its normal value
                    return mHomotypicLabelledSpringConstantMultiplier;
                }
                else if (cell_B_is_myo_stem)
                {
                    // For homotypic interactions between myoepitehlial cells, leave the spring constant unchanged from its normal value
                    return mHomotypicLabelledSpringConstantMultiplier;
                }
                else if (p_node_b->IsParticle())
                {
                    // For homotypic interactions between myoepitehlial cells, leave the spring constant unchanged from its normal value
                    return mHomotypicLabelledSpringConstantMultiplier;
                }
            }
        }
        else // node is particle
        {
            ECM_node.push_back(nodeBGlobalIndex);
            return 1.0;
        }
        
        return 1.0;
    }
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
LinearSpringForce<ELEMENT_DIM,SPACE_DIM>::~LinearSpringForce()
{
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
c_vector<double, SPACE_DIM> LinearSpringForce<ELEMENT_DIM,SPACE_DIM>::CalculateForceBetweenNodes(unsigned nodeAGlobalIndex,
                                                                                    unsigned nodeBGlobalIndex,
                                                                                    AbstractCellPopulation<ELEMENT_DIM,SPACE_DIM>& rCellPopulation)
{
    // We should only ever calculate the force between two distinct nodes
    assert(nodeAGlobalIndex != nodeBGlobalIndex);

    Node<SPACE_DIM>* p_node_a = rCellPopulation.GetNode(nodeAGlobalIndex);
    Node<SPACE_DIM>* p_node_b = rCellPopulation.GetNode(nodeBGlobalIndex);

    // Get the node locations
    const c_vector<double, SPACE_DIM>& r_node_a_location = p_node_a->rGetLocation();
    const c_vector<double, SPACE_DIM>& r_node_b_location = p_node_b->rGetLocation();

    // Get the node radii for a NodeBasedCellPopulation
    double node_a_radius = 0.0;
    double node_b_radius = 0.0;

    if (bool(dynamic_cast<NodeBasedCellPopulationWithParticles<SPACE_DIM>*>(&rCellPopulation)))
    {
        node_a_radius = p_node_a->GetRadius();
        node_b_radius = p_node_b->GetRadius();
    }

    // Get the unit vector parallel to the line joining the two nodes
    c_vector<double, SPACE_DIM> unit_difference;

    /*
     * We use the mesh method GetVectorFromAtoB() to compute the direction of the
     * unit vector along the line joining the two nodes, rather than simply subtract
     * their positions, because this method can be overloaded (e.g. to enforce a
     * periodic boundary in Cylindrical2dMesh).
     */
    unit_difference = rCellPopulation.rGetMesh().GetVectorFromAtoB(r_node_a_location, r_node_b_location);

    // Calculate the distance between the two nodes
    double distance_between_nodes = norm_2(unit_difference);
    assert(distance_between_nodes > 0);
    assert(!std::isnan(distance_between_nodes));

    unit_difference /= distance_between_nodes;

    /*
     * If mUseCutOffLength has been set, then there is zero force between
     * two nodes located a distance apart greater than mMechanicsCutOffLength in AbstractTwoBodyInteractionForce.
     */
    if (this->mUseCutOffLength)
    {
        if (distance_between_nodes >= this->GetCutOffLength())
        {
            return zero_vector<double>(SPACE_DIM); // c_vector<double,SPACE_DIM>() is not guaranteed to be fresh memory
        }
    }

    /*
     * Calculate the rest length of the spring connecting the two nodes with a default
     * value of 1.0.
     */
    double rest_length_final = 1.0;

    if (bool(dynamic_cast<MeshBasedCellPopulation<ELEMENT_DIM,SPACE_DIM>*>(&rCellPopulation)))
    {
        rest_length_final = static_cast<MeshBasedCellPopulation<ELEMENT_DIM,SPACE_DIM>*>(&rCellPopulation)->GetRestLength(nodeAGlobalIndex, nodeBGlobalIndex);
    }
    else if (bool(dynamic_cast<NodeBasedCellPopulationWithParticles<SPACE_DIM>*>(&rCellPopulation)))
    {
        assert(node_a_radius > 0 && node_b_radius > 0);
        rest_length_final = node_a_radius+node_b_radius;
    }

    double rest_length = rest_length_final;

    if (!p_node_a->IsParticle() && !p_node_b->IsParticle()) // if we have a cell-cell pair
    {
        CellPtr p_cell_A = rCellPopulation.GetCellUsingLocationIndex(nodeAGlobalIndex);
        CellPtr p_cell_B = rCellPopulation.GetCellUsingLocationIndex(nodeBGlobalIndex);

        double ageA = p_cell_A->GetAge();
        double ageB = p_cell_B->GetAge();

        assert(!std::isnan(ageA));
        assert(!std::isnan(ageB));

        /*
        * If the cells are both newly divided, then the rest length of the spring
        * connecting them grows linearly with time, until 1 hour after division.
        */
        if (ageA < mMeinekeSpringGrowthDuration && ageB < mMeinekeSpringGrowthDuration)
        {
            AbstractCentreBasedCellPopulation<ELEMENT_DIM,SPACE_DIM>* p_static_cast_cell_population = static_cast<AbstractCentreBasedCellPopulation<ELEMENT_DIM,SPACE_DIM>*>(&rCellPopulation);

            std::pair<CellPtr,CellPtr> cell_pair = p_static_cast_cell_population->CreateCellPair(p_cell_A, p_cell_B);

            if (p_static_cast_cell_population->IsMarkedSpring(cell_pair))
            {
                // Spring rest length increases from a small value to the normal rest length over 1 hour
                double lambda = mMeinekeDivisionRestingSpringLength;
                rest_length = lambda + (rest_length_final - lambda) * ageA/mMeinekeSpringGrowthDuration;
            }
            if (ageA + SimulationTime::Instance()->GetTimeStep() >= mMeinekeSpringGrowthDuration)
            {
                // This spring is about to go out of scope
                p_static_cast_cell_population->UnmarkSpring(cell_pair);
            }
        }

        // For apoptosis, progressively reduce the radius of the cell
        double a_rest_length = rest_length*0.5;
        double b_rest_length = a_rest_length;

        if (bool(dynamic_cast<NodeBasedCellPopulationWithParticles<SPACE_DIM>*>(&rCellPopulation)))
        {
            assert(node_a_radius > 0 && node_b_radius > 0);
            a_rest_length = (node_a_radius/(node_a_radius+node_b_radius))*rest_length;
            b_rest_length = (node_b_radius/(node_a_radius+node_b_radius))*rest_length;
        }

        /*
        * If either of the cells has begun apoptosis, then the length of the spring
        * connecting them decreases linearly with time.
        */
        if (p_cell_A->HasApoptosisBegun())
        {
            double time_until_death_a = p_cell_A->GetTimeUntilDeath();
            a_rest_length = a_rest_length * time_until_death_a / p_cell_A->GetApoptosisTime();
        }
        if (p_cell_B->HasApoptosisBegun())
        {
            double time_until_death_b = p_cell_B->GetTimeUntilDeath();
            b_rest_length = b_rest_length * time_until_death_b / p_cell_B->GetApoptosisTime();
        }

        rest_length = a_rest_length + b_rest_length;
        //assert(rest_length <= 1.0+1e-12); ///\todo #1884 Magic number: would "<= 1.0" do?

        // Although in this class the 'spring constant' is a constant parameter, in
        // subclasses it can depend on properties of each of the cells
        double overlap = distance_between_nodes - rest_length;
        bool is_closer_than_rest_length = (overlap <= 0);
        double multiplication_factor = VariableSpringConstantMultiplicationFactor(nodeAGlobalIndex, nodeBGlobalIndex, rCellPopulation, is_closer_than_rest_length);
        double spring_cell_cell_stiffness = mCellCellSpringStiffness;

        if (bool(dynamic_cast<MeshBasedCellPopulation<ELEMENT_DIM,SPACE_DIM>*>(&rCellPopulation)))
        {
            return multiplication_factor * spring_cell_cell_stiffness * unit_difference * overlap;
        }
        else
        {
            // A reasonably stable simple force law
            if (is_closer_than_rest_length) //overlap is negative
            {
                //log(x+1) is undefined for x<=-1
                assert(overlap > -rest_length_final);
                c_vector<double, SPACE_DIM> temp = multiplication_factor*spring_cell_cell_stiffness * unit_difference * rest_length_final* log(1.0 + overlap/rest_length_final);
                return temp;
            }
            else
            {
                double alpha = 5.0;
                c_vector<double, SPACE_DIM> temp = multiplication_factor*spring_cell_cell_stiffness * unit_difference * overlap * exp(-alpha * overlap/rest_length_final);
                return temp;
            }
        }
    }
    else if (p_node_a->IsParticle() && p_node_b->IsParticle()) // if we have ECM-ECM pair
    {
        rest_length = 1;

        double overlap = distance_between_nodes - rest_length;
        bool is_closer_than_rest_length = (overlap <= 0);
        double multiplication_factor = VariableSpringConstantMultiplicationFactor(nodeAGlobalIndex, nodeBGlobalIndex, rCellPopulation, is_closer_than_rest_length);
        double spring_ecm_ecm_stiffness = mECMECMSpringStiffness;

        if (bool(dynamic_cast<MeshBasedCellPopulation<ELEMENT_DIM,SPACE_DIM>*>(&rCellPopulation)))
        {
            return multiplication_factor * spring_ecm_ecm_stiffness * unit_difference * overlap;
        }
        else
        {
            // A reasonably stable simple force law
            if (is_closer_than_rest_length) //overlap is negative
            {
                //log(x+1) is undefined for x<=-1
                assert(overlap > -rest_length_final);
                c_vector<double, SPACE_DIM> temp = multiplication_factor*spring_ecm_ecm_stiffness * unit_difference * rest_length_final* log(1.0 + overlap/rest_length_final);
                return temp;
            }
            else
            {
                double alpha = 5.0;
                c_vector<double, SPACE_DIM> temp = multiplication_factor*spring_ecm_ecm_stiffness * unit_difference * overlap * exp(-alpha * overlap/rest_length_final);
                return temp;
            }
        }
    }
    else // if we have cell-ECM pair
    {
        rest_length = 1;

        double overlap = distance_between_nodes - rest_length;
        bool is_closer_than_rest_length = (overlap <= 0);
        double multiplication_factor = VariableSpringConstantMultiplicationFactor(nodeAGlobalIndex, nodeBGlobalIndex, rCellPopulation, is_closer_than_rest_length);
        double spring_cell_ecm_stiffness = mCellECMSpringStiffness;

        if (bool(dynamic_cast<MeshBasedCellPopulation<ELEMENT_DIM,SPACE_DIM>*>(&rCellPopulation)))
        {
            return multiplication_factor * spring_cell_ecm_stiffness * unit_difference * overlap;
        }
        else
        {
            // A reasonably stable simple force law
            if (is_closer_than_rest_length) //overlap is negative
            {
                //log(x+1) is undefined for x<=-1
                assert(overlap > -rest_length_final);
                c_vector<double, SPACE_DIM> temp = multiplication_factor*spring_cell_ecm_stiffness * unit_difference * rest_length_final* log(1.0 + overlap/rest_length_final);
                return temp;
            }
            else
            {
                double alpha = 5.0;
                c_vector<double, SPACE_DIM> temp = multiplication_factor*spring_cell_ecm_stiffness * unit_difference * overlap * exp(-alpha * overlap/rest_length_final);
                return temp;
            }
        }
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
double LinearSpringForce<ELEMENT_DIM,SPACE_DIM>::GetCellCellSpringStiffness()
{
    return mCellCellSpringStiffness;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
double LinearSpringForce<ELEMENT_DIM,SPACE_DIM>::GetCellECMSpringStiffness()
{
    return mCellECMSpringStiffness;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
double LinearSpringForce<ELEMENT_DIM,SPACE_DIM>::GetECMECMSpringStiffness()
{
    return mECMECMSpringStiffness;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
double LinearSpringForce<ELEMENT_DIM,SPACE_DIM>::GetMeinekeDivisionRestingSpringLength()
{
    return mMeinekeDivisionRestingSpringLength;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
double LinearSpringForce<ELEMENT_DIM,SPACE_DIM>::GetMeinekeSpringGrowthDuration()
{
    return mMeinekeSpringGrowthDuration;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void LinearSpringForce<ELEMENT_DIM,SPACE_DIM>::SetCellCellSpringStiffness(double cellcellSpringStiffness)
{
    assert(springStiffness > 0.0);
    mCellCellSpringStiffness = cellcellSpringStiffness;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void LinearSpringForce<ELEMENT_DIM,SPACE_DIM>::SetCellECMSpringStiffness(double cellECMSpringStiffness)
{
    assert(springStiffness > 0.0);
    mCellECMSpringStiffness = cellECMSpringStiffness;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void LinearSpringForce<ELEMENT_DIM,SPACE_DIM>::SetECMECMSpringStiffness(double eCMECMSpringStiffness)
{
    assert(springStiffness > 0.0);
    mECMECMSpringStiffness = eCMECMSpringStiffness;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void LinearSpringForce<ELEMENT_DIM,SPACE_DIM>::SetMeinekeDivisionRestingSpringLength(double divisionRestingSpringLength)
{
    assert(divisionRestingSpringLength <= 1.0);
    assert(divisionRestingSpringLength >= 0.0);

    mMeinekeDivisionRestingSpringLength = divisionRestingSpringLength;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void LinearSpringForce<ELEMENT_DIM,SPACE_DIM>::SetMeinekeSpringGrowthDuration(double springGrowthDuration)
{
    assert(springGrowthDuration >= 0.0);

    mMeinekeSpringGrowthDuration = springGrowthDuration;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void LinearSpringForce<ELEMENT_DIM,SPACE_DIM>::OutputForceParameters(out_stream& rParamsFile)
{
    *rParamsFile << "\t\t\t<CellECMSpringStiffness>" << mCellCellSpringStiffness << "</CellCellSpringStiffness>\n";
    *rParamsFile << "\t\t\t<ECMECMSpringStiffness>" << mCellECMSpringStiffness << "</CellECMSpringStiffness>\n";
    *rParamsFile << "\t\t\t<ECMECMSpringStiffness>" << mECMECMSpringStiffness << "</ECMECMSpringStiffness>\n";
    *rParamsFile << "\t\t\t<MeinekeDivisionRestingSpringLength>" << mMeinekeDivisionRestingSpringLength << "</MeinekeDivisionRestingSpringLength>\n";
    *rParamsFile << "\t\t\t<MeinekeSpringGrowthDuration>" << mMeinekeSpringGrowthDuration << "</MeinekeSpringGrowthDuration>\n";

    // Call method on direct parent class
    AbstractTwoBodyInteractionForce<ELEMENT_DIM,SPACE_DIM>::OutputForceParameters(rParamsFile);
}

// Explicit instantiation
template class LinearSpringForce<1,1>;
template class LinearSpringForce<1,2>;
template class LinearSpringForce<2,2>;
template class LinearSpringForce<1,3>;
template class LinearSpringForce<2,3>;
template class LinearSpringForce<3,3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_ALL_DIMS(LinearSpringForce)

