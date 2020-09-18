#ifndef TESTMAMMARYORGANOID_HPP_
#define TESTMAMMARYORGANOID_HPP_

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
#include "HeterotypicBoundaryLengthWriter.hpp"
#include "MammaryCellTypeWriter.hpp"
#include "CellCellAdhesionForce.hpp"
#include "Debug.hpp"

/*
 * The results of each test below can be visualised using Paraview. 
 */
class TestMammaryOrganoid : public AbstractCellBasedTestSuite
{
public:

void TestOrganoid()
    {
        EXIT_IF_PARALLEL;

        // Create a 3D 'nodes only' mesh, specifying nodes manually
        std::vector<Node<3>*> nodes;
        nodes.push_back(new Node<3>(0,  false,  0.5, 0.0, 0.0));
        nodes.push_back(new Node<3>(1,  false,  -0.5, 0.0, 0.0));
        nodes.push_back(new Node<3>(2,  false,  0.0, 0.5, 0.0));
        nodes.push_back(new Node<3>(3,  false,  0.0, -0.5, 0.0));
        NodesOnlyMesh<3> mesh;
        mesh.ConstructNodesWithoutMesh(nodes, 1.5);
       
        // Create a vector of proliferative cells using the helper CellsGenerator
        std::vector<CellPtr> cells;
        CellsGenerator<MammaryCellCycleModel, 3> cells_generator;
        cells_generator.GenerateBasicRandom(cells, mesh.GetNumNodes());
      
        // Use the mesh and cells to create a cell population
        NodeBasedCellPopulation<3> cell_population(mesh, cells);
        
        // Create the luminal/myoepithelial cell properties (we do it this way
        // to make sure they're tracked correctly in the simulation)
        boost::shared_ptr<AbstractCellProperty> p_luminal(cell_population.GetCellPropertyRegistry()->Get<LuminalCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_myo(cell_population.GetCellPropertyRegistry()->Get<MyoepithelialCellProperty>());
        
        // Assign these properties to cells (change these lines if you want e.g. only luminal cells)
        cell_population.GetCellUsingLocationIndex(0)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(1)->AddCellProperty(p_myo);
        cell_population.GetCellUsingLocationIndex(2)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(3)->AddCellProperty(p_myo);
        
        // Add a cell writer so that cell velocities are written to file
        cell_population.AddCellWriter<CellVelocityWriter>();
       
        // Add a cell writer so that mammary cell types are written to file
        cell_population.AddCellWriter<MammaryCellTypeWriter>();

        // Add a cell writer so that cell sorting (bilayer formation) is written to file
        cell_population.AddCellWriter<HeterotypicBoundaryLengthWriter>();
      
        // Pass the cell population to the simulation and specify duration and output parameters
        OffLatticeSimulation<3> simulator(cell_population);
        simulator.SetOutputDirectory("TestMammaryOrganoid");
        simulator.SetSamplingTimestepMultiple(1);
        simulator.SetEndTime(60.0); // Hours
       
        // Create a force law and pass it to the simulation
        MAKE_PTR(CellCellAdhesionForce<3>, p_differential_adhesion_force);
        p_differential_adhesion_force->SetMeinekeSpringStiffness(50.0);
        p_differential_adhesion_force->SetHomotypicLabelledSpringConstantMultiplier(1.0);
        p_differential_adhesion_force->SetHeterotypicSpringConstantMultiplier(0.1);
        simulator.AddForce(p_differential_adhesion_force);
       
        // Run the simulation
        simulator.Solve();
      
        // Since we created pointers to nodes, we delete them here to avoid memory leaks
        for (unsigned i=0; i<nodes.size(); i++)
        {
            delete nodes[i];
        }
    };

#endif /* TESTMAMMARYORGANOID_HPP_ */