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
#include "HoneycombMeshGenerator.hpp"

#include "LuminalCellProperty.hpp"
#include "MyoepithelialCellProperty.hpp"
#include "LuminalStemCellProperty.hpp"
#include "MyoepithelialStemCellProperty.hpp"

#include "CellIdWriter.hpp"
#include "CellPopulationAdjacencyWriter.hpp"
#include "CellVelocityWriter.hpp"
#include "CellLocationWriter.hpp"
#include "MammaryCellTypeWriter.hpp"

#include "MammaryCellCycleModel.hpp"
#include "UniformG1GenerationalCellCycleModel.hpp"
#include "DifferentiatedCellProliferativeType.hpp"
#include "WildTypeCellMutationState.hpp"
#include "StemCellProliferativeType.hpp"

#include "OrientedDivisionRule.hpp"
#include "AnoikisCellKiller3D.hpp"

#include "LinearSpringForce.hpp"
#include "CellECMAdhesionForce.hpp"
#include "DifferentialAdhesionLinearSpringForce.hpp"
#include "RandomMotionForce.hpp"

#include "Debug.hpp"

/*
 * The results of each test below can be visualised using Paraview. 
 */

class TestMammaryOrganoid : public AbstractCellBasedTestSuite
{
public:

    void xTestMammaryOrganoid2DWithoutProliferation()
    {
        // We use the HoneycombMeshGenerator to create a honeycomb mesh covering a circular domain of given radius
        HoneycombMeshGenerator generator(20, 20, 0);
        MutableMesh<2,2>* p_generating_mesh = generator.GetCircularMesh(10);

        // Convert this to a NodesOnlyMesh
        NodesOnlyMesh<2> mesh;

        // We construct the mesh using the generating mesh and a cut-off 1.5 which defines the connectivity in the mesh.
        mesh.ConstructNodesWithoutMesh(*p_generating_mesh, 1.5);

        // Set up cells, one for each Node
        std::vector<CellPtr> cells;
        MAKE_PTR(DifferentiatedCellProliferativeType, p_differentiated_type);
        CellsGenerator<UniformG1GenerationalCellCycleModel, 2> cells_generator;
        cells_generator.GenerateBasicRandom(cells, mesh.GetNumNodes(), p_differentiated_type);

        // Create cell population
        NodeBasedCellPopulation<2> cell_population(mesh, cells);

        // Set population to output all data to results files
        cell_population.AddCellWriter<CellIdWriter>();
        cell_population.AddCellWriter<MammaryCellTypeWriter>();
        cell_population.AddCellWriter<CellLocationWriter>();
        cell_population.AddPopulationWriter<CellPopulationAdjacencyWriter>();

        /* We randomly assign some cells luminal or myoepithelial using the cell property {{{LuminalCellProperty or MyoepithleilaCellproperty}}}. 
         * We begin by creating a shared pointer to this cell property using the helper singleton {{{CellPropertyRegistry}}}.  
         * We then loop over the cells and label each cell luminal independently with probability 0.6. 
        */
        boost::shared_ptr<AbstractCellProperty> p_luminal(cell_population.GetCellPropertyRegistry()->Get<LuminalCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_myo(cell_population.GetCellPropertyRegistry()->Get<MyoepithelialCellProperty>());

        for (AbstractCellPopulation<2>::Iterator cell_iter = cell_population.Begin();
             cell_iter != cell_population.End();
             ++cell_iter)
        {
            if (RandomNumberGenerator::Instance()->ranf() < 0.6)
            {
                cell_iter->AddCellProperty(p_luminal);
            }
            else
            {
                cell_iter->AddCellProperty(p_myo);
            }
        }

        // Set up cell-based simulation and output directory
        OffLatticeSimulation<2> simulator(cell_population);
        simulator.SetOutputDirectory("TestMammaryOrganoid2D/WithoutProliferation");
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(10.0);

        // We create a force law and pass it to the
        MAKE_PTR(GeneralisedLinearSpringForce<2>, p_linear_force);
        p_linear_force->SetCutOffLength(3);
        simulator.AddForce(p_linear_force);

        // Create a force law and pass it to the simulation
        MAKE_PTR(CellECMAdhesionForce<2>, p_force);
        simulator.AddForce(p_force);

        // Run simulation
        simulator.Solve();
    }

    void xTestMammaryOrganoid2CellTypeDifferentialAdhesion()    
    {
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
        CellsGenerator<UniformG1GenerationalCellCycleModel, 3> cells_generator;
        cells_generator.GenerateBasicRandom(cells, mesh.GetNumNodes());
      
        // Use the mesh and cells to create a cell population
        NodeBasedCellPopulation<3> cell_population(mesh, cells);

        // Set population to output all data to results files
        cell_population.AddCellWriter<CellIdWriter>();
        cell_population.AddCellWriter<MammaryCellTypeWriter>();
        cell_population.AddPopulationWriter<CellPopulationAdjacencyWriter>();

        /* We randomly assign some cells luminal or myoepithelial using the cell property {{{LuminalCellProperty or MyoepithleilaCellproperty}}}. 
         * We begin by creating a shared pointer to this cell property using the helper singleton {{{CellPropertyRegistry}}}.  
         * We then loop over the cells and label each cell luminal independently with probability 0.6. 
        */
        boost::shared_ptr<AbstractCellProperty> p_luminal(cell_population.GetCellPropertyRegistry()->Get<LuminalCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_myo(cell_population.GetCellPropertyRegistry()->Get<MyoepithelialCellProperty>());

        for (AbstractCellPopulation<3>::Iterator cell_iter = cell_population.Begin();
             cell_iter != cell_population.End();
             ++cell_iter)
        {
            if (RandomNumberGenerator::Instance()->ranf() < 0.6)
            {
                cell_iter->AddCellProperty(p_luminal);
            }
            else
            {
                cell_iter->AddCellProperty(p_myo);
            }
        }

        // Set up cell-based simulation and output directory
        OffLatticeSimulation<3> simulator(cell_population);
        simulator.SetOutputDirectory("TestMammaryOrganoid/DifferentialAdhesion");
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(120.0);

        // Create a force law and pass it to the simulation
        MAKE_PTR(DifferentialAdhesionLinearSpringForce<3>, p_differential_adhesion_force);
        p_differential_adhesion_force->SetHomotypicLabelledSpringConstantMultiplier(1.0);
        p_differential_adhesion_force->SetHeterotypicSpringConstantMultiplier(0.1);
        p_differential_adhesion_force->SetCutOffLength(3.0);
        simulator.AddForce(p_differential_adhesion_force);

        // Run simulation
        simulator.Solve();
    }

    void xTestMammaryOrganoid2CellType()
    {
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
        CellsGenerator<UniformG1GenerationalCellCycleModel, 3> cells_generator;
        cells_generator.GenerateBasicRandom(cells, mesh.GetNumNodes());
      
        // Use the mesh and cells to create a cell population
        NodeBasedCellPopulation<3> cell_population(mesh, cells);

        // Set population to output all data to results files
        cell_population.AddCellWriter<CellIdWriter>();
        cell_population.AddCellWriter<MammaryCellTypeWriter>();
        cell_population.AddCellWriter<CellLocationWriter>();
        cell_population.AddPopulationWriter<CellPopulationAdjacencyWriter>();

        /* We randomly assign some cells luminal or myoepithelial using the cell property {{{LuminalCellProperty or MyoepithleilaCellproperty}}}. 
         * We begin by creating a shared pointer to this cell property using the helper singleton {{{CellPropertyRegistry}}}.  
         * We then loop over the cells and label each cell luminal independently with probability 0.6. 
        */
        boost::shared_ptr<AbstractCellProperty> p_luminal(cell_population.GetCellPropertyRegistry()->Get<LuminalCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_myo(cell_population.GetCellPropertyRegistry()->Get<MyoepithelialCellProperty>());

        for (AbstractCellPopulation<3>::Iterator cell_iter = cell_population.Begin();
             cell_iter != cell_population.End();
             ++cell_iter)
        {
            if (RandomNumberGenerator::Instance()->ranf() < 0.6)
            {
                cell_iter->AddCellProperty(p_luminal);
            }
            else
            {
                cell_iter->AddCellProperty(p_myo);
            }
        }

        // Set up cell-based simulation and output directory
        OffLatticeSimulation<3> simulator(cell_population);
        simulator.SetOutputDirectory("TestMammaryOrganoid");
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(120.0);

        // We create a force law and pass it to the
        MAKE_PTR(GeneralisedLinearSpringForce<3>, p_linear_force);
        p_linear_force->SetCutOffLength(1.5);
        simulator.AddForce(p_linear_force);

        // Create a force law and pass it to the simulation
        MAKE_PTR(CellECMAdhesionForce<3>, p_force);
        simulator.AddForce(p_force);

        // Run simulation
        simulator.Solve();
    }
    
    void TestMammaryOrganoidMammaryCellCycleModel()
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
        
        // Create the different cell types: luminal stem cells, myoepithelial stem differentiated luminal cells and differentiated myoepithelial cell, 
        // (we do it this way to make sure they're tracked correctly in the simulation)
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
        cell_population.GetCellUsingLocationIndex(9)->AddCellProperty(p_myo_stem);
        cell_population.GetCellUsingLocationIndex(10)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(11)->AddCellProperty(p_luminal);

        // Set population to output all data to results files
        cell_population.AddCellWriter<CellIdWriter>();
        cell_population.AddCellWriter<MammaryCellTypeWriter>();
        cell_population.AddCellWriter<CellLocationWriter>();
        cell_population.AddCellWriter<CellVelocityWriter>();
        cell_population.AddPopulationWriter<CellPopulationAdjacencyWriter>();

        // Pass the cell population to the simulation and specify duration and output parameters
        OffLatticeSimulation<3> simulator(cell_population);
        simulator.SetOutputDirectory("TestMammaryOrganoid/MammaryCellCycleModel/WT/n=3");
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(120.0);

        // We create a force law and pass it to the
        MAKE_PTR(GeneralisedLinearSpringForce<3>, p_linear_force);
        p_linear_force->SetCutOffLength(1.5);
        simulator.AddForce(p_linear_force);

        // Create a force law and pass it to the simulation
        MAKE_PTR(CellECMAdhesionForce<3>, p_force);
        simulator.AddForce(p_force);

        // Add some noise to avoid local minimum
        MAKE_PTR(RandomMotionForce<3>, p_random_force);
        p_random_force->SetMovementParameter(0.05); //0.1 causes dissasociation, 0.001 is not enough
        simulator.AddForce(p_random_force);

        // Run simulation
        simulator.Solve();
    }

    void xTestMammaryOrganoidIndividualCellStart()
    {
        EXIT_IF_PARALLEL;
        
        // Create a 3D 'nodes only' mesh, specifying nodes manually
        std::vector<Node<3>*> nodes;
        nodes.push_back(new Node<3>(0,  false,  0.0, -0.5, 0.0));
        nodes.push_back(new Node<3>(1,  false,  1.25, -0.5, 1.0));
        nodes.push_back(new Node<3>(2,  false,  -1.0, -1.5, 0.0));
        nodes.push_back(new Node<3>(3,  false,  1.75, -1.5, 1.0));
        nodes.push_back(new Node<3>(4,  false,  0.5, 0.25, 1.0));
        nodes.push_back(new Node<3>(5,  false,  1.0, 2.0, 2.0));
        nodes.push_back(new Node<3>(6,  false,  1.5, 0.5, 1.0));
        nodes.push_back(new Node<3>(7,  false,  0.5, -1.5, 1.0));
        nodes.push_back(new Node<3>(8,  false,  -1.0, 0.0, 2.0));
        nodes.push_back(new Node<3>(9,  false,  1.75, 1.5, 0.0));
        nodes.push_back(new Node<3>(10,  false,  -0.5, 0.5, 0.0));
        nodes.push_back(new Node<3>(11,  false, 0.5, 1.25, 1.0));
        
        NodesOnlyMesh<3> mesh;
        mesh.ConstructNodesWithoutMesh(nodes, 1.5);
       
        // Create a vector of proliferative cells using the helper CellsGenerator
        std::vector<CellPtr> cells;
        CellsGenerator<MammaryCellCycleModel, 3> cells_generator;
        cells_generator.GenerateBasicRandom(cells, mesh.GetNumNodes());
      
        // Use the mesh and cells to create a cell population
        NodeBasedCellPopulation<3> cell_population(mesh, cells);
        
        // Create the different cell types: luminal stem cells, myoepithelial stem differentiated luminal cells and differentiated myoepithelial cell, 
        // (we do it this way to make sure they're tracked correctly in the simulation)
        boost::shared_ptr<AbstractCellProperty> p_luminal(cell_population.GetCellPropertyRegistry()->Get<LuminalCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_myo(cell_population.GetCellPropertyRegistry()->Get<MyoepithelialCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_luminal_stem(cell_population.GetCellPropertyRegistry()->Get<LuminalStemCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_myo_stem(cell_population.GetCellPropertyRegistry()->Get<MyoepithelialStemCellProperty>());
        
        // Assign these properties to cells
        cell_population.GetCellUsingLocationIndex(0)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(1)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(2)->AddCellProperty(p_myo_stem);
        cell_population.GetCellUsingLocationIndex(3)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(4)->AddCellProperty(p_myo);
        cell_population.GetCellUsingLocationIndex(5)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(6)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(7)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(8)->AddCellProperty(p_myo);
        cell_population.GetCellUsingLocationIndex(9)->AddCellProperty(p_myo_stem);
        cell_population.GetCellUsingLocationIndex(10)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(11)->AddCellProperty(p_luminal);

        // Set population to output all data to results files
        cell_population.AddCellWriter<CellIdWriter>();
        cell_population.AddCellWriter<MammaryCellTypeWriter>();
        cell_population.AddCellWriter<CellLocationWriter>();
        cell_population.AddCellWriter<CellVelocityWriter>();
        cell_population.AddPopulationWriter<CellPopulationAdjacencyWriter>();

        // Pass the cell population to the simulation and specify duration and output parameters
        OffLatticeSimulation<3> simulator(cell_population);
        simulator.SetOutputDirectory("TestMammaryOrganoid/MammaryCellCycleModel/IndividualCells");
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(120.0);

        // We create a force law and pass it to the
        MAKE_PTR(GeneralisedLinearSpringForce<3>, p_linear_force);
        p_linear_force->SetCutOffLength(1.5);
        simulator.AddForce(p_linear_force);

        // Create a force law and pass it to the simulation
        MAKE_PTR(CellECMAdhesionForce<3>, p_force);
        simulator.AddForce(p_force);

        // Run simulation
        simulator.Solve();
    }

    void xTestMammaryOrganoidWithAllMechanisms()
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
        
        // Create the different cell types: luminal stem cells, myoepithelial stem differentiated luminal cells and differentiated myoepithelial cell, 
        // (we do it this way to make sure they're tracked correctly in the simulation)
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

        // Set population to output all data to results files
        cell_population.AddCellWriter<CellIdWriter>();
        cell_population.AddCellWriter<MammaryCellTypeWriter>();
        cell_population.AddCellWriter<CellLocationWriter>();
        cell_population.AddPopulationWriter<CellPopulationAdjacencyWriter>();

        // Pass the cell population to the simulation and specify duration and output parameters
        OffLatticeSimulation<3> simulator(cell_population);
        simulator.SetOutputDirectory("TestMammaryOrganoidWT");
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(120.0);

        // We create a force law and pass it to the
        MAKE_PTR(GeneralisedLinearSpringForce<3>, p_linear_force);
        p_linear_force->SetCutOffLength(1.5);
        simulator.AddForce(p_linear_force);

        // Create a force law and pass it to the simulation
        MAKE_PTR(CellECMAdhesionForce<3>, p_force);
        simulator.AddForce(p_force);

        // // Add an anoikis-based cell killer and pass it to the simulation
        // MAKE_PTR_ARGS(AnoikisCellKiller3D<3>, p_anoikis_killer, (&cell_population));
        // simulator.AddCellKiller(p_anoikis_killer);

        // Run the simulation
        simulator.Solve();
    }
};

#endif /* TESTMAMMARYORGANOID_HPP_ */
