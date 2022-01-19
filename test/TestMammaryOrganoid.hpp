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
        
        // Create a simple 3D mesh with some particles
        std::vector<Node<3>*> nodes;
        nodes.push_back(new Node<3>(0,  false, -0.5, -0.5, 0.0));
        nodes.push_back(new Node<3>(1,  false, 0.5, 0.5, 0.0));
        nodes.push_back(new Node<3>(2,  false, 0.5, -0.5, -0.5));
        nodes.push_back(new Node<3>(3,  false, -0.5, 0.5, 0.5));
        nodes.push_back(new Node<3>(4,  false, 0.0, 0.0, 0.0));
        
        int counter = 5;

        for (unsigned i=0; i<5; i++) 
        {
            for (unsigned j=0; j<5; j++) 
            {
                for (unsigned k=0; k<5; k++)
                {
                    double spacing = 1.0; 
                    double L = 10;
                    double x = -L/2 + spacing*i; 
                    double y = -L/2 + spacing*j; 
                    double z = -L/2 + spacing*k;
                    nodes.push_back(new Node<3>(counter,  false, x, y, z));
                    counter += 1;
                }
            }
        }
        
        // Convert this to a NodesOnlyMesh
        MAKE_PTR(NodesOnlyMesh<3>, p_mesh);
        p_mesh->ConstructNodesWithoutMesh(nodes, 1.5);
        
        // Specify the node indices corresponding to cells (the others correspond to particles)
        std::vector<unsigned> location_indices;
        for (unsigned index=0; index<5; index++)
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
        cell_population.GetCellUsingLocationIndex(1)->AddCellProperty(p_myo_stem);
        cell_population.GetCellUsingLocationIndex(2)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(3)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(4)->AddCellProperty(p_myo);
        
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
        p_linear_force->SetCellECMSpringStiffness(15.0);
        p_linear_force->SetECMECMSpringStiffness(30.0);
        simulator.AddForce(p_linear_force);
        
        // // Create a cell-ECM adhesion force law and pass it to the simulation
        // MAKE_PTR(CellECMAdhesionForce<3>, p_cell_ECM_force);
        // simulator.AddForce(p_cell_ECM_force);
        
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