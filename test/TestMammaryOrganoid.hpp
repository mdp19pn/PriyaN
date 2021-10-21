#ifndef TESTMAMMARYORGANOID_HPP_
#define TESTMAMMARYORGANOID_HPP_

// Include necessary header files
#include <cxxtest/TestSuite.h>
#include "CheckpointArchiveTypes.hpp"
#include "AbstractCellBasedTestSuite.hpp"

#include "CellBasedSimulationArchiver.hpp"

#include "SmartPointers.hpp"
#include "PetscSetupAndFinalize.hpp"
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
#include "WildTypeCellMutationState.hpp"
#include "StemCellProliferativeType.hpp"
#include "OrientedDivisionRule.hpp"
#include "AnoikisCellKiller3D.hpp"
#include "LinearSpringForce.hpp"
#include "RepulsionForce.hpp"
#include "CellECMAdhesionForce.hpp"
#include "LumenExpansionForce.hpp"
#include "Debug.hpp"

/*
 * The results of each test below can be visualised using Paraview. 
 */
class TestMammaryOrganoid : public AbstractCellBasedTestSuite
{
public:

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

    void TestMammaryOrganoidWithParticles()
    {
        EXIT_IF_PARALLEL;
        MARK;
        // Create a simple 3D mesh with some particles
        std::vector<Node<3>*> nodes;
        nodes.push_back(new Node<3>(0,  false, -0.5, -0.5, 0.0));
        nodes.push_back(new Node<3>(1,  false, 0.5, 0.5, 0.0));
        nodes.push_back(new Node<3>(2,  false, 0.5, -0.5, -0.5));
        nodes.push_back(new Node<3>(3,  false, -0.5, 0.5, 0.5));
        nodes.push_back(new Node<3>(4,  false, 0.0, 0.0, 0.0));
        
        nodes.push_back(new Node<3>(5,  false, -1.0, -1.0, -1.0)); //corner
        nodes.push_back(new Node<3>(6,  false, 0.0, -1.0, -1.0));
        nodes.push_back(new Node<3>(7,  false, 1.0, -1.0, -1.0)); //corner
        nodes.push_back(new Node<3>(8,  false, 1.0, 0.0, -1.0));
        nodes.push_back(new Node<3>(9,  false, 1.0,  1.0, -1.0)); //corner
        nodes.push_back(new Node<3>(10,  false, 0.0,  1.0, -1.0));
        nodes.push_back(new Node<3>(11,  false, -1.0,  1.0, -1.0)); //corner
        nodes.push_back(new Node<3>(12,  false, -1.0,  0.0, -1.0));
        nodes.push_back(new Node<3>(13,  false, 0.0,  0.0, -1.0));

        nodes.push_back(new Node<3>(14,  false, -1.0, -1.0, 0.0)); //corner
        nodes.push_back(new Node<3>(15,  false, 0.0, -1.0, 0.0));
        nodes.push_back(new Node<3>(16,  false, 1.0, -1.0, 0.0)); //corner
        nodes.push_back(new Node<3>(17,  false, 1.0, 0.0, 0.0));
        nodes.push_back(new Node<3>(18,  false, 1.0,  1.0, 0.0)); //corner
        nodes.push_back(new Node<3>(19,  false, 0.0,  1.0, 0.0));
        nodes.push_back(new Node<3>(20,  false, -1.0,  1.0, 0.0)); //corner
        nodes.push_back(new Node<3>(21,  false, -1.0,  0.0, 0.0));

        nodes.push_back(new Node<3>(22,  false, -1.0, -1.0, 1.0)); //corner
        nodes.push_back(new Node<3>(23,  false, 0.0, -1.0, 1.0));
        nodes.push_back(new Node<3>(24,  false, 1.0, -1.0, 1.0)); //corner 
        nodes.push_back(new Node<3>(25,  false, 1.0, 0.0, 1.0));
        nodes.push_back(new Node<3>(26,  false, 1.0,  1.0, 1.0)); //corner
        nodes.push_back(new Node<3>(27,  false, 0.0,  1.0, 1.0));
        nodes.push_back(new Node<3>(28,  false, -1.0,  1.0, 1.0)); //corner
        nodes.push_back(new Node<3>(29,  false, -1.0,  0.0, 1.0));
        nodes.push_back(new Node<3>(30,  false, 0.0,  0.0, 1.0));
        MARK;
        // Convert this to a NodesOnlyMesh
        MAKE_PTR(NodesOnlyMesh<3>, p_mesh);
        p_mesh->ConstructNodesWithoutMesh(nodes, 1.5);
        MARK;
        // Specify the node indices corresponding to cells (the others correspond to particles)
        std::vector<unsigned> location_indices;
        for (unsigned index=0; index<5; index++)
        {
            location_indices.push_back(index);
        }
        MARK;
        // Set up cells
        std::vector<CellPtr> cells;
        CellsGenerator<UniformCellCycleModel, 3> cells_generator;
        cells_generator.GenerateGivenLocationIndices(cells, location_indices);
        MARK;
        // Create cell population
        NodeBasedCellPopulationWithParticles<3> cell_population(*p_mesh, cells, location_indices);
        MARK;
        /* 
        * Create the different cell types: luminal stem cells, myoepithelial stem differentiated luminal cells and 
        * differentiated myoepithelial cell, (we do it this way to make sure they're tracked correctly in the simulation)
        */
        boost::shared_ptr<AbstractCellProperty> p_luminal(cell_population.GetCellPropertyRegistry()->Get<LuminalCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_myo(cell_population.GetCellPropertyRegistry()->Get<MyoepithelialCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_luminal_stem(cell_population.GetCellPropertyRegistry()->Get<LuminalStemCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_myo_stem(cell_population.GetCellPropertyRegistry()->Get<MyoepithelialStemCellProperty>());
        MARK;
        // Assign these properties to cells
        cell_population.GetCellUsingLocationIndex(0)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(1)->AddCellProperty(p_myo_stem);
        cell_population.GetCellUsingLocationIndex(2)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(3)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(4)->AddCellProperty(p_myo);
        MARK;
        // Add a cell writer so that mammary cell types are written to file
        cell_population.AddCellWriter<MammaryCellTypeWriter>();
        MARK;
        // Add a cell writer so that the cell location is written to file
        cell_population.AddCellWriter<CellLocationWriter>();
        MARK;
        // Add a cell writer so that cell velocities are written to file
        cell_population.AddCellWriter<CellVelocityWriter>();
        MARK;
        // Pass the cell population to the simulation and specify duration and output parameters
        OffLatticeSimulation<3> simulator(cell_population);
        simulator.SetOutputDirectory("TestMammaryOrganoidWithParticles");
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(96.0); // Hours
        MARK;
        // Add linear spring force which has different spring stiffness constants, depending on the pair of nodes (cells, particles) it is connecting.
        MAKE_PTR(LinearSpringForce<3>, p_linear_force);
        p_linear_force->SetCutOffLength(1.5);
        p_linear_force->SetCellCellSpringStiffness(15.0);
        p_linear_force->SetCellECMSpringStiffness(15.0);
        p_linear_force->SetECMECMSpringStiffness(15.0);
        simulator.AddForce(p_linear_force);
        MARK;
        // Create a cell-ECM adhesion force law and pass it to the simulation
        MAKE_PTR(CellECMAdhesionForce<3>, p_cell_ECM_force);
        simulator.AddForce(p_cell_ECM_force);
        MARK;
        // Run the simulation
        simulator.Solve();
        MARK;
        // Since we created pointers to nodes, we delete them here to avoid memory leaks
        for (unsigned i=0; i<nodes.size(); i++)
        {
            delete nodes[i];
        }
    }
};
#endif /* TESTMAMMARYORGANOID_HPP_ */