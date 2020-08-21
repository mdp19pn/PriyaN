#ifndef TESTMAMMARYMONOLAYER_HPP_
#define TESTMAMMARYMONOLAYER_HPP_

// Include necessary header files
#include <cxxtest/TestSuite.h>
#include "CheckpointArchiveTypes.hpp"
#include "AbstractCellBasedTestSuite.hpp"
#include "PetscSetupAndFinalize.hpp"
#include "CellsGenerator.hpp"
#include "MammaryCellCycleModel.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "OffLatticeSimulation.hpp"
#include "SmartPointers.hpp"
#include "NodesOnlyMesh.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "LuminalCellProperty.hpp"
#include "MyoepithelialCellProperty.hpp"
#include "CellVelocityWriter.hpp"
#include "MammaryCellTypeWriter.hpp"
#include "CellCellAdhesionForce.hpp"
#include "Debug.hpp"

/*
 * The results of each test below can be visualised using Paraview. 
 */
class TestMammaryMonolayer : public AbstractCellBasedTestSuite
{
public:

    void TestMonolayer()
    {
        EXIT_IF_PARALLEL;
        
        // Create a 2D 'nodes only' mesh, specifying nodes manually
        std::vector<Node<2>*> nodes;
        nodes.push_back(new Node<2>(0, false, 0.5, 0.0));
        nodes.push_back(new Node<2>(1, false, 0.0, 0.5));
        nodes.push_back(new Node<2>(2, false, 1.0, 0.0));
        nodes.push_back(new Node<2>(3, false, 0.5, 1.0));
        nodes.push_back(new Node<2>(4, false, 1.0, 0.5));
        NodesOnlyMesh<2> mesh;
        mesh.ConstructNodesWithoutMesh(nodes, 1.5);
        
        // Create a vector of proliferative cells using the helper CellsGenerator
        std::vector<CellPtr> cells;
        CellsGenerator<MammaryCellCycleModel, 2> cells_generator;
        cells_generator.GenerateBasicRandom(cells, mesh.GetNumNodes());
        
        // Use the mesh and cells to create a cell population
        NodeBasedCellPopulation<2> cell_population(mesh, cells);
        
        // Create the luminal/myoepithelial cell properties (we do it this way
        // to make sure they're tracked correctly in the simulation)
        boost::shared_ptr<AbstractCellProperty> p_luminal(cell_population.GetCellPropertyRegistry()->Get<LuminalCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_myo(cell_population.GetCellPropertyRegistry()->Get<MyoepithelialCellProperty>());
        
        // Assign these properties to cells (change these lines if you want e.g. only luminal cells)
        cell_population.GetCellUsingLocationIndex(0)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(1)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(2)->AddCellProperty(p_myo);
        cell_population.GetCellUsingLocationIndex(3)->AddCellProperty(p_myo);
        cell_population.GetCellUsingLocationIndex(4)->AddCellProperty(p_myo);
        
        // Add a cell writer so that cell velocities are written to file
        cell_population.AddCellWriter<CellVelocityWriter>();
        
        // Add a cell writer so that mammary cell types are written to file
        cell_population.AddCellWriter<MammaryCellTypeWriter>();
        
        // Pass the cell population to the simulation and specify duration and output parameters
        OffLatticeSimulation<2> simulator(cell_population);
        simulator.SetOutputDirectory("TestMammaryMonolayer");
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(96.0); // Hours
        
        // Create an adhesion force law and pass it to the simulation
        MAKE_PTR(CellCellAdhesionForce<2>, p_differential_adhesion_force);
        p_differential_adhesion_force->SetMeinekeSpringStiffness(50.0);
        p_differential_adhesion_force->SetHomotypicLabelledSpringConstantMultiplier(1.0);
        p_differential_adhesion_force->SetHeterotypicSpringConstantMultiplier(0.1);
        simulator.AddForce(p_differential_adhesion_force);
        
        // Run the simulation
        simulator.Solve();
    }
};

#endif /* TESTMAMMARYMONOLAYER_HPP_ */
