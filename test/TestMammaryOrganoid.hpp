#ifndef TESTMAMMARYORGANOID_HPP_
#define TESTMAMMARYORGANOID_HPP_

// Include necessary header files
#include <cxxtest/TestSuite.h>
#include "CheckpointArchiveTypes.hpp"
#include "AbstractCellBasedTestSuite.hpp"

#include "CellBasedSimulationArchiver.hpp"

#include "SmartPointers.hpp"
#include "PetscSetupAndFinalize.hpp"
#include "UblasCustomFunctions.hpp" // required for 'Create_c_vector'
#include "CellsGenerator.hpp"
#include "OffLatticeSimulation.hpp"
#include "NodesOnlyMesh.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "NodeBasedCellPopulationWithParticles.hpp"
#include "LuminalCellProperty.hpp"
#include "MyoepithelialCellProperty.hpp"
#include "LuminalStemCellProperty.hpp"
#include "MyoepithelialStemCellProperty.hpp"
#include "CellHeightTrackingModifier.hpp"
#include "CellVelocityWriter.hpp"
#include "CellLocationWriter.hpp"
#include "BoundaryLengthWriter.hpp"
#include "MammaryCellTypeWriter.hpp"
#include "MammaryCellCycleModel.hpp"
#include "UniformCellCycleModel.hpp"
#include "NoCellCycleModel.hpp"
#include "WildTypeCellMutationState.hpp"
#include "StemCellProliferativeType.hpp"
#include "OrientedDivisionRule.hpp"
#include "AnoikisCellKiller3D.hpp"
#include "LinearSpringForce.hpp"
#include "LinearSpringForceCopy.hpp"
#include "RepulsionForce.hpp"
#include "CellECMAdhesionForce.hpp"
#include "LumenExpansionForce.hpp"
#include "RandomMotionForce.hpp"
#include "Debug.hpp"

/*
 * The results of each test below can be visualised using Paraview. 
 */

static const double M_TIME_TO_STEADY_STATE = 10; //10
static const double M_TIME_FOR_SIMULATION = 100; //100
static const double M_CELL_FLUCTUATION = 1.0;

class TestMammaryOrganoid : public AbstractCellBasedTestSuite
{
public:

    void TestMammaryOrganoid2D()
    {
        EXIT_IF_PARALLEL;
        
        // Create a 2D 'nodes only' mesh, specifying nodes manually
        std::vector<Node<2>*> nodes;
        nodes.push_back(new Node<2>(0,false, 0.0, 1.0));
        nodes.push_back(new Node<2>(1,false, 1.0, 1.0));
        nodes.push_back(new Node<2>(2,false, 2.0, 1.0));
        nodes.push_back(new Node<2>(3,false, 3.0, 1.0));
        nodes.push_back(new Node<2>(4,false, 1.0, 2.0));
        nodes.push_back(new Node<2>(5,false, 2.0, 2.0));
        nodes.push_back(new Node<2>(6,false, 3.0, 2.0));
        nodes.push_back(new Node<2>(7,false, 1.0, 0.0));
        nodes.push_back(new Node<2>(8,false, 2.0, 0.0));
        nodes.push_back(new Node<2>(9,false, 1.0, 3.0));
        nodes.push_back(new Node<2>(10,false, 2.0, 3.0));
        nodes.push_back(new Node<2>(11,false, 0.0, 2.0));
        nodes.push_back(new Node<2>(12,false, 4.0, 2.0));

        nodes.push_back(new Node<2>(13,false, 4.0, 1.0));
        nodes.push_back(new Node<2>(14,false, 5.0, 1.0));
        nodes.push_back(new Node<2>(15,false, 1.0, 4.0));
        nodes.push_back(new Node<2>(16,false, 2.0, 4.0));
        nodes.push_back(new Node<2>(17,false, 5.0, 2.0));
        nodes.push_back(new Node<2>(18,false, 6.0, 2.0));
        nodes.push_back(new Node<2>(19,false, 7.0, 2.0));
        nodes.push_back(new Node<2>(20,false, 3.0, 0.0));
        nodes.push_back(new Node<2>(21,false, 4.0, 0.0));
        nodes.push_back(new Node<2>(22,false, 3.0, 3.0));
        nodes.push_back(new Node<2>(23,false, 4.0, 3.0));
        nodes.push_back(new Node<2>(24,false, 3.0, 4.0));
        nodes.push_back(new Node<2>(25,false, 4.0, 4.0));
        nodes.push_back(new Node<2>(26,false, 5.0, 3.0));
        nodes.push_back(new Node<2>(27,false, 5.0, 4.0));

        c_vector< double, 2 > Node0Pos = Create_c_vector(0.0, 1.0);
        c_vector< double, 2 > Node1Pos = Create_c_vector(1.0, 1.0);
        c_vector< double, 2 > Node2Pos = Create_c_vector(2.0, 1.0);
        c_vector< double, 2 > Node3Pos = Create_c_vector(3.0, 1.0);
        c_vector< double, 2 > Node4Pos = Create_c_vector(1.0, 2.0);
        c_vector< double, 2 > Node5Pos = Create_c_vector(2.0, 2.0);
        c_vector< double, 2 > Node6Pos = Create_c_vector(3.0, 2.0);
        c_vector< double, 2 > Node7Pos = Create_c_vector(1.0, 0.0);
        c_vector< double, 2 > Node8Pos = Create_c_vector(2.0, 0.0);
        c_vector< double, 2 > Node9Pos = Create_c_vector(1.0, 3.0);
        c_vector< double, 2 > Node10Pos = Create_c_vector(2.0, 3.0);
        c_vector< double, 2 > Node11Pos = Create_c_vector(0.0, 2.0);
        c_vector< double, 2 > Node12Pos = Create_c_vector(4.0, 2.0);

        c_vector< double, 2 > Node13Pos = Create_c_vector(4.0, 1.0);
        c_vector< double, 2 > Node14Pos = Create_c_vector(5.0, 1.0);
        c_vector< double, 2 > Node15Pos = Create_c_vector(1.0, 4.0);
        c_vector< double, 2 > Node16Pos = Create_c_vector(2.0, 4.0);
        c_vector< double, 2 > Node17Pos = Create_c_vector(5.0, 2.0);
        c_vector< double, 2 > Node18Pos = Create_c_vector(6.0, 2.0);
        c_vector< double, 2 > Node19Pos = Create_c_vector(7.0, 2.0);
        c_vector< double, 2 > Node20Pos = Create_c_vector(3.0, 0.0);
        c_vector< double, 2 > Node21Pos = Create_c_vector(4.0, 0.0);
        c_vector< double, 2 > Node22Pos = Create_c_vector(3.0, 3.0);
        c_vector< double, 2 > Node23Pos = Create_c_vector(4.0, 3.0);
        c_vector< double, 2 > Node24Pos = Create_c_vector(3.0, 4.0);
        c_vector< double, 2 > Node25Pos = Create_c_vector(4.0, 4.0);
        c_vector< double, 2 > Node26Pos = Create_c_vector(5.0, 3.0);
        c_vector< double, 2 > Node27Pos = Create_c_vector(5.0, 4.0);

        int counter = 28; // the node count number starts from 14

        for (unsigned i=0; i<20; i++) 
        {
            for (unsigned j=0; j<20; j++) 
            {
                double spacing = 1.0; 
                double L = 20;
                
                double x = -L/2 + spacing*i; // "-L/x" ensures the particle mesh is offset from the origin
                double y = -L/2 + spacing*j; // "-L/x" ensures the particle mesh is offset from the origin
                
                c_vector< double, 2 > CurrentLocation = Create_c_vector(x,y);

                if ((CurrentLocation[0] == Node0Pos[0]) & (CurrentLocation[1] == Node0Pos[1]))
                {
                    continue; // skips the rest of this iteration
                }
                else if ((CurrentLocation[0] == Node1Pos[0]) & (CurrentLocation[1] == Node1Pos[1]))
                {
                    continue; // skips the rest of this iteration
                }
                else if ((CurrentLocation[0] == Node2Pos[0]) & (CurrentLocation[1] == Node2Pos[1]))
                {
                    continue; // skips the rest of this iteration
                }
                else if ((CurrentLocation[0] == Node3Pos[0]) & (CurrentLocation[1] == Node3Pos[1]))
                {
                    continue; // skips the rest of this iteration
                }
                else if ((CurrentLocation[0] == Node4Pos[0]) & (CurrentLocation[1] == Node4Pos[1]))
                {
                    continue; // skips the rest of this iteration
                }
                else if ((CurrentLocation[0] == Node5Pos[0]) & (CurrentLocation[1] == Node5Pos[1]))
                {
                    continue; // skips the rest of this iteration
                }
                else if ((CurrentLocation[0] == Node6Pos[0]) & (CurrentLocation[1] == Node6Pos[1]))
                {
                    continue; // skips the rest of this iteration
                }
                else if ((CurrentLocation[0] == Node7Pos[0]) & (CurrentLocation[1] == Node7Pos[1]))
                {
                    continue; // skips the rest of this iteration
                }
                else if ((CurrentLocation[0] == Node8Pos[0]) & (CurrentLocation[1] == Node8Pos[1]))
                {
                    continue; // skips the rest of this iteration
                }
                else if ((CurrentLocation[0] == Node9Pos[0]) & (CurrentLocation[1] == Node9Pos[1]))
                {
                    continue; // skips the rest of this iteration
                }
                else if ((CurrentLocation[0] == Node10Pos[0]) & (CurrentLocation[1] == Node10Pos[1]))
                {
                    continue; // skips the rest of this iteration
                }
                else if ((CurrentLocation[0] == Node11Pos[0]) & (CurrentLocation[1] == Node11Pos[1]))
                {
                    continue; // skips the rest of this iteration
                }
                else if ((CurrentLocation[0] == Node12Pos[0]) & (CurrentLocation[1] == Node12Pos[1]))
                {
                    continue; // skips the rest of this iteration
                }
                else if ((CurrentLocation[0] == Node13Pos[0]) & (CurrentLocation[1] == Node13Pos[1]))
                {
                    continue; // skips the rest of this iteration
                }
                else if ((CurrentLocation[0] == Node14Pos[0]) & (CurrentLocation[1] == Node14Pos[1]))
                {
                    continue; // skips the rest of this iteration
                }
                else if ((CurrentLocation[0] == Node15Pos[0]) & (CurrentLocation[1] == Node15Pos[1]))
                {
                    continue; // skips the rest of this iteration
                }
                else if ((CurrentLocation[0] == Node16Pos[0]) & (CurrentLocation[1] == Node16Pos[1]))
                {
                    continue; // skips the rest of this iteration
                }
                else if ((CurrentLocation[0] == Node17Pos[0]) & (CurrentLocation[1] == Node17Pos[1]))
                {
                    continue; // skips the rest of this iteration
                }
                else if ((CurrentLocation[0] == Node18Pos[0]) & (CurrentLocation[1] == Node18Pos[1]))
                {
                    continue; // skips the rest of this iteration
                }
                else if ((CurrentLocation[0] == Node19Pos[0]) & (CurrentLocation[1] == Node19Pos[1]))
                {
                    continue; // skips the rest of this iteration
                }
                else if ((CurrentLocation[0] == Node20Pos[0]) & (CurrentLocation[1] == Node20Pos[1]))
                {
                    continue; // skips the rest of this iteration
                }
                else if ((CurrentLocation[0] == Node21Pos[0]) & (CurrentLocation[1] == Node21Pos[1]))
                {
                    continue; // skips the rest of this iteration
                }
                else if ((CurrentLocation[0] == Node22Pos[0]) & (CurrentLocation[1] == Node22Pos[1]))
                {
                    continue; // skips the rest of this iteration
                }
                else if ((CurrentLocation[0] == Node23Pos[0]) & (CurrentLocation[1] == Node23Pos[1]))
                {
                    continue; // skips the rest of this iteration
                }
                else if ((CurrentLocation[0] == Node24Pos[0]) & (CurrentLocation[1] == Node24Pos[1]))
                {
                    continue; // skips the rest of this iteration
                }
                else if ((CurrentLocation[0] == Node25Pos[0]) & (CurrentLocation[1] == Node25Pos[1]))
                {
                    continue; // skips the rest of this iteration
                }
                else if ((CurrentLocation[0] == Node26Pos[0]) & (CurrentLocation[1] == Node26Pos[1]))
                {
                    continue; // skips the rest of this iteration
                }
                else if ((CurrentLocation[0] == Node27Pos[0]) & (CurrentLocation[1] == Node27Pos[1]))
                {
                    continue; // skips the rest of this iteration
                }
                
                nodes.push_back(new Node<2>(counter,  false, x, y));
                counter += 1; // the number of nodes increases by 1 each time
            }
        }

        // Convert this to a NodesOnlyMesh
        MAKE_PTR(NodesOnlyMesh<2>, p_mesh);
        p_mesh->ConstructNodesWithoutMesh(nodes, 1.5);
        
        // Specify the node indices corresponding to cells (the others correspond to particles)
        std::vector<unsigned> location_indices;
        for (unsigned index=0; index<28; index++)
        {
            location_indices.push_back(index);
        }
        
        // Set up cells
        std::vector<CellPtr> cells;
        CellsGenerator<NoCellCycleModel, 2> cells_generator;
        cells_generator.GenerateGivenLocationIndices(cells, location_indices);
        
        // Use the mesh and cells to create a cell population
        //NodeBasedCellPopulation<2> cell_population(p_mesh, cells);
               
        // Create cell population
        NodeBasedCellPopulationWithParticles<2> cell_population(*p_mesh, cells, location_indices);
        
        // Create the different cell types: luminal stem cells, myoepithelial stem differentiated luminal cells and differentiated myoepithelial cell, (we do it this way to make sure they're tracked correctly in the simulation)
        boost::shared_ptr<AbstractCellProperty> p_luminal(cell_population.GetCellPropertyRegistry()->Get<LuminalCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_myo(cell_population.GetCellPropertyRegistry()->Get<MyoepithelialCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_luminal_stem(cell_population.GetCellPropertyRegistry()->Get<LuminalStemCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_myo_stem(cell_population.GetCellPropertyRegistry()->Get<MyoepithelialStemCellProperty>());
        
        // Assign these properties to cells
        // cell_population.GetCellUsingLocationIndex(0)->AddCellProperty(p_luminal);
        // cell_population.GetCellUsingLocationIndex(1)->AddCellProperty(p_luminal);
        // cell_population.GetCellUsingLocationIndex(2)->AddCellProperty(p_luminal_stem);
        // cell_population.GetCellUsingLocationIndex(3)->AddCellProperty(p_luminal_stem);
        // cell_population.GetCellUsingLocationIndex(4)->AddCellProperty(p_luminal);
        // cell_population.GetCellUsingLocationIndex(5)->AddCellProperty(p_luminal_stem);
        // cell_population.GetCellUsingLocationIndex(6)->AddCellProperty(p_luminal);
        // cell_population.GetCellUsingLocationIndex(7)->AddCellProperty(p_myo);
        // cell_population.GetCellUsingLocationIndex(8)->AddCellProperty(p_myo_stem);
        // cell_population.GetCellUsingLocationIndex(9)->AddCellProperty(p_myo);
        // cell_population.GetCellUsingLocationIndex(10)->AddCellProperty(p_myo_stem);
        // cell_population.GetCellUsingLocationIndex(11)->AddCellProperty(p_myo);
        // cell_population.GetCellUsingLocationIndex(12)->AddCellProperty(p_myo_stem);

        cell_population.GetCellUsingLocationIndex(0)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(1)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(2)->AddCellProperty(p_myo_stem);
        cell_population.GetCellUsingLocationIndex(3)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(4)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(5)->AddCellProperty(p_myo);
        cell_population.GetCellUsingLocationIndex(6)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(7)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(8)->AddCellProperty(p_myo);
        cell_population.GetCellUsingLocationIndex(9)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(10)->AddCellProperty(p_myo_stem);
        cell_population.GetCellUsingLocationIndex(11)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(12)->AddCellProperty(p_myo_stem);
        cell_population.GetCellUsingLocationIndex(13)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(14)->AddCellProperty(p_myo);
        cell_population.GetCellUsingLocationIndex(15)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(16)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(17)->AddCellProperty(p_myo);
        cell_population.GetCellUsingLocationIndex(18)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(19)->AddCellProperty(p_myo_stem);
        cell_population.GetCellUsingLocationIndex(20)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(21)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(22)->AddCellProperty(p_myo_stem);
        cell_population.GetCellUsingLocationIndex(23)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(24)->AddCellProperty(p_myo);
        cell_population.GetCellUsingLocationIndex(25)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(26)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(27)->AddCellProperty(p_myo);
        
        // Add a cell writer so that mammary cell types are written to file
        cell_population.AddCellWriter<MammaryCellTypeWriter>();

        // Add a cell writer so that the cell location is written to file
        cell_population.AddCellWriter<CellLocationWriter>();

        // Add a population writer so that cell sorting is written to file
        //cell_population.AddPopulationWriter<BoundaryLengthWriter>();
        
        // Pass the cell population to the simulation and specify duration and output parameters
        OffLatticeSimulation<2> simulator(cell_population);
        simulator.SetOutputDirectory("TestMammaryOrganoid2D");
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(120.0); // Hours
        // simulator.SetDt(1.0/200.0);
        // simulator.SetSamplingTimestepMultiple(200);
        // simulator.SetEndTime(M_TIME_TO_STEADY_STATE);
        
        // Add linear spring force which has different spring stiffness constants, depending on the pair of nodes (cells, particles) it is connecting.
        MAKE_PTR(LinearSpringForce<2>, p_linear_force);
        p_linear_force->SetCutOffLength(2.5);
        p_linear_force->SetCellCellSpringStiffness(15.0);
        p_linear_force->SetCellECMSpringStiffness(15.0);
        p_linear_force->SetECMECMSpringStiffness(5.0);
	    p_linear_force->SetHomotypicSpringConstantMultiplier(1.0);
	    p_linear_force->SetHeterotypicSpringConstantMultiplier(0.1);
        simulator.AddForce(p_linear_force);

        // Add some noise to avoid local minimum
        MAKE_PTR(RandomMotionForce<2>, p_random_force);
        p_random_force->SetMovementParameter(0.05); //0.1 causes dissasociation, 0.001 is not enough
        simulator.AddForce(p_random_force);

        // Adjust parameters
        p_random_force->SetMovementParameter(0.05*M_CELL_FLUCTUATION); //0.1 causes dissasociation
        
        // Run the simulation
        //simulator.SetEndTime(M_TIME_TO_STEADY_STATE + M_TIME_FOR_SIMULATION);
        simulator.Solve();
    }
    
    void xTestMammaryOrganoidMammaryCellCycleModel()
    {
        EXIT_IF_PARALLEL;
        
        // Create a 3D 'nodes only' mesh, specifying nodes manually
        std::vector<Node<3>*> nodes;
        nodes.push_back(new Node<3>(0,  false,  0.0, 0.0, 0.0));
        nodes.push_back(new Node<3>(1,  false,  0.75, 0.0, 0.0));
        nodes.push_back(new Node<3>(2,  false,  -0.5, -0.5, 0.0));
        nodes.push_back(new Node<3>(3,  false,  1.25, -0.5, 0.0));
        nodes.push_back(new Node<3>(4,  false,  0.0, 0.75, 0.0));
        nodes.push_back(new Node<3>(5,  false,  0.75, 0.75, 0.0));
        nodes.push_back(new Node<3>(6,  false,  0.0, 0.0, 1.0));
        nodes.push_back(new Node<3>(7,  false,  -0.5, -0.5, 1.0));
        nodes.push_back(new Node<3>(8,  false,  -0.5, 0.5, 1.0));
        nodes.push_back(new Node<3>(9,  false,  1.25, 0.0, 0.0));
        nodes.push_back(new Node<3>(10,  false,  0.0, 1.0, 0.0));
        nodes.push_back(new Node<3>(11,  false,  0.0, 0.75, 1.0));
        
        NodesOnlyMesh<3> mesh;
        mesh.ConstructNodesWithoutMesh(nodes, 1.5);
       
        // Create a vector of proliferative cells using the helper CellsGenerator
        std::vector<CellPtr> cells;
        CellsGenerator<MammaryCellCycleModel, 3> cells_generator;
        cells_generator.GenerateBasicRandom(cells, mesh.GetNumNodes());
      
        // Use the mesh and cells to create a cell population
        NodeBasedCellPopulation<3> cell_population(mesh, cells);
        
        // Create the different cell types: luminal stem cells, myoepithelial stem differentiated luminal cells and differentiated myoepithelial cell, (we do it this way to make sure they're tracked correctly in the simulation)
        boost::shared_ptr<AbstractCellProperty> p_luminal(cell_population.GetCellPropertyRegistry()->Get<LuminalCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_myo(cell_population.GetCellPropertyRegistry()->Get<MyoepithelialCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_luminal_stem(cell_population.GetCellPropertyRegistry()->Get<LuminalStemCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_myo_stem(cell_population.GetCellPropertyRegistry()->Get<MyoepithelialStemCellProperty>());
        
        // Assign these properties to cells
        cell_population.GetCellUsingLocationIndex(0)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(1)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(2)->AddCellProperty(p_myo_stem);
        cell_population.GetCellUsingLocationIndex(3)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(4)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(5)->AddCellProperty(p_myo);
        cell_population.GetCellUsingLocationIndex(6)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(7)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(8)->AddCellProperty(p_myo);
        cell_population.GetCellUsingLocationIndex(9)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(10)->AddCellProperty(p_myo_stem);
        cell_population.GetCellUsingLocationIndex(11)->AddCellProperty(p_luminal);

        // Set the division rule for our population to be the oriented division rule
        boost::shared_ptr<AbstractCentreBasedDivisionRule<3,3> > p_division_rule_to_set(new OrientedDivisionRule<3,3>());
        cell_population.SetCentreBasedDivisionRule(p_division_rule_to_set);

        // Add a cell writer so that mammary cell types are written to file
        cell_population.AddCellWriter<MammaryCellTypeWriter>();

        // Add a cell writer so that the cell location is written to file
        cell_population.AddCellWriter<CellLocationWriter>();

        // Add a cell writer so that cell velocities are written to file
        cell_population.AddCellWriter<CellVelocityWriter>();

        // Pass the cell population to the simulation and specify duration and output parameters
        OffLatticeSimulation<3> simulator(cell_population);
        simulator.SetOutputDirectory("TestMammaryOrganoidWT");
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(96.0); // Hours

        MAKE_PTR(GeneralisedLinearSpringForce<3>, p_linear_force);
        p_linear_force->SetCutOffLength(3);
        simulator.AddForce(p_linear_force);

        // // Add an anoikis-based cell killer and pass it to the simulation
		// MAKE_PTR_ARGS(AnoikisCellKiller3D<3>, p_anoikis_killer, (&cell_population));
		// simulator.AddCellKiller(p_anoikis_killer);

        // Run the simulation
        simulator.Solve();
      
        // Since we created pointers to nodes, we delete them here to avoid memory leaks
        for (unsigned i=0; i<nodes.size(); i++)
        {
            delete nodes[i];
        }
    }

    void xTestMammaryOrganoidWithParticles()
    {
        EXIT_IF_PARALLEL;
        
        // Create a simple 3D mesh with some particles
        std::vector<Node<3>*> nodes;
        nodes.push_back(new Node<3>(0,  false,  0.0, 0.0, 0.0));
        nodes.push_back(new Node<3>(1,  false,  0.75, 0.0, 0.0));
        nodes.push_back(new Node<3>(2,  false,  -0.5, -0.5, 0.0));
        nodes.push_back(new Node<3>(3,  false,  1.25, -0.5, 0.0));
        nodes.push_back(new Node<3>(4,  false,  0.0, 0.75, 0.0));
        nodes.push_back(new Node<3>(5,  false,  0.75, 0.75, 0.0));
        nodes.push_back(new Node<3>(6,  false,  0.0, 1.0, 0.0));
        
        c_vector< double, 3 > Node0Pos = Create_c_vector(-0.5, -0.5, 0.0);
        c_vector< double, 3 > Node1Pos = Create_c_vector(0.5, 0.5, 0.0);
        c_vector< double, 3 > Node2Pos = Create_c_vector(0.5, -0.5, -0.5);
        c_vector< double, 3 > Node3Pos = Create_c_vector(-0.5, 0.5, 0.0);
        c_vector< double, 3 > Node4Pos = Create_c_vector(0.0, 0.0, 0.0);
        c_vector< double, 3 > Node5Pos = Create_c_vector(0.75, 0.75, 0.0);
        c_vector< double, 3 > Node6Pos = Create_c_vector(0.0, 1.0, 0.0);

        int counter = 7; // the node count number starts from 11

        for (unsigned i=0; i<10; i++) 
        {
            for (unsigned j=0; j<10; j++) 
            {
                for (unsigned k=0; k<10; k++)
                {
                    double spacing = 1.0; 
                    double L = 20;
                    
                    double x = -L/5 + spacing*i; // "-L/x" ensures the particle mesh is offset from the origin
                    double y = -L/5 + spacing*j; // "-L/x" ensures the particle mesh is offset from the origin
                    double z = -L/5 + spacing*k; // "-L/x" ensures the particle mesh is offset from the origin
                    
                    c_vector< double, 3 > CurrentLocation = Create_c_vector(x,y,z);

                    if ((CurrentLocation[0] == Node0Pos[0]) & (CurrentLocation[1] == Node0Pos[1]) & (CurrentLocation[2] == Node0Pos[2]))
                    {
                        continue; // skips the rest of this iteration
                    }
                    else if ((CurrentLocation[0] == Node1Pos[0]) & (CurrentLocation[1] == Node1Pos[1]) & (CurrentLocation[2] == Node1Pos[2]))
                    {
                        continue; // skips the rest of this iteration
                    }
                    else if ((CurrentLocation[0] == Node2Pos[0]) & (CurrentLocation[1] == Node2Pos[1]) & (CurrentLocation[2] == Node2Pos[2]))
                    {
                        continue; // skips the rest of this iteration
                    }
                    else if ((CurrentLocation[0] == Node3Pos[0]) & (CurrentLocation[1] == Node3Pos[1]) & (CurrentLocation[2] == Node3Pos[2]))
                    {
                        continue; // skips the rest of this iteration
                    }
                    else if ((CurrentLocation[0] == Node4Pos[0]) & (CurrentLocation[1] == Node4Pos[1]) & (CurrentLocation[2] == Node4Pos[2]))
                    {
                        continue; // skips the rest of this iteration
                    }
                    else if ((CurrentLocation[0] == Node5Pos[0]) & (CurrentLocation[1] == Node5Pos[1]) & (CurrentLocation[2] == Node5Pos[2]))
                    {
                        continue; // skips the rest of this iteration
                    }
                    else if ((CurrentLocation[0] == Node6Pos[0]) & (CurrentLocation[1] == Node6Pos[1]) & (CurrentLocation[2] == Node6Pos[2]))
                    {
                        continue; // skips the rest of this iteration
                    }
                    
                    nodes.push_back(new Node<3>(counter,  false, x, y, z));
                    counter += 1; // the number of nodes increases by 1 each time
                }
            }
        }
        
        // Convert this to a NodesOnlyMesh
        MAKE_PTR(NodesOnlyMesh<3>, p_mesh);
        p_mesh->ConstructNodesWithoutMesh(nodes, 1.5);
        
        // Specify the node indices corresponding to cells (the others correspond to particles)
        std::vector<unsigned> location_indices;
        for (unsigned index=0; index<7; index++)
        {
            location_indices.push_back(index);
        }
        
        // Set up cells
        std::vector<CellPtr> cells;
        CellsGenerator<UniformCellCycleModel, 3> cells_generator;
        cells_generator.GenerateGivenLocationIndices(cells, location_indices);
        
        // Create cell population
        NodeBasedCellPopulationWithParticles<3> cell_population(*p_mesh, cells, location_indices);
        
        /* 
        * Create the different cell types: luminal stem cells, myoepithelial stem differentiated luminal cells and 
        * differentiated myoepithelial cell, (we do it this way to make sure they're tracked correctly in the simulation)
        */
        boost::shared_ptr<AbstractCellProperty> p_luminal(cell_population.GetCellPropertyRegistry()->Get<LuminalCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_myo(cell_population.GetCellPropertyRegistry()->Get<MyoepithelialCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_luminal_stem(cell_population.GetCellPropertyRegistry()->Get<LuminalStemCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_myo_stem(cell_population.GetCellPropertyRegistry()->Get<MyoepithelialStemCellProperty>());
        
        // Assign these properties to cells
        cell_population.GetCellUsingLocationIndex(0)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(1)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(2)->AddCellProperty(p_myo_stem);
        cell_population.GetCellUsingLocationIndex(3)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(4)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(5)->AddCellProperty(p_myo);
        cell_population.GetCellUsingLocationIndex(6)->AddCellProperty(p_myo_stem);

        // Add a cell writer so that mammary cell types are written to file
        cell_population.AddCellWriter<MammaryCellTypeWriter>();
        
        // Add a cell writer so that the cell location is written to file
        cell_population.AddCellWriter<CellLocationWriter>();
        
        // Add a cell writer so that cell velocities are written to file
        cell_population.AddCellWriter<CellVelocityWriter>();
        
        // Pass the cell population to the simulation and specify duration and output parameters
        OffLatticeSimulation<3> simulator(cell_population);
        simulator.SetOutputDirectory("TestMammaryOrganoidWithParticles");
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(96.0); // Hours
        
        // Add linear spring force which has different spring stiffness constants, depending on the pair of nodes (cells, particles) it is connecting.
        MAKE_PTR(LinearSpringForce<3>, p_linear_force);
        p_linear_force->SetCutOffLength(1.5);
        p_linear_force->SetCellCellSpringStiffness(15.0);
        p_linear_force->SetCellECMSpringStiffness(5.0);
        p_linear_force->SetECMECMSpringStiffness(5.0);
	    p_linear_force->SetHomotypicSpringConstantMultiplier(1.0);
	    p_linear_force->SetHeterotypicSpringConstantMultiplier(2.0);
        simulator.AddForce(p_linear_force);

        // MAKE_PTR(LinearSpringForceCopy<3>, p_linear_force);
        // p_linear_force->SetCutOffLength(1.5);
        // p_linear_force->SetCellCellSpringStiffness(15.0);
        // p_linear_force->SetCellECMSpringStiffness(5.0);
        // p_linear_force->SetECMECMSpringStiffness(5.0);
        // simulator.AddForce(p_linear_force);
        
        // Run the simulation
        simulator.Solve();
        
        // Since we created pointers to nodes, we delete them here to avoid memory leaks
        for (unsigned i=0; i<nodes.size(); i++)
        {
            delete nodes[i];
        }
    }
};

#endif /* TESTMAMMARYORGANOID_HPP_ */