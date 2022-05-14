#ifndef TESTCELLSORTING_HPP_
#define TESTCELLSORTING_HPP_

#include <cxxtest/TestSuite.h>

// Must be included before other cell_based headers
#include "CellBasedSimulationArchiver.hpp"

#include "AbstractCellBasedWithTimingsTestSuite.hpp"
#include "CellLabel.hpp"
#include "SmartPointers.hpp"
#include "CellsGenerator.hpp"
#include "UniformG1GenerationalCellCycleModel.hpp"
#include "DifferentiatedCellProliferativeType.hpp"
#include "NodeBasedCellPopulationWithParticles.hpp"

#include "HeterotypicBoundaryLengthWriter.hpp"
#include "CellLabelWriter.hpp"

#include "HoneycombMeshGenerator.hpp"
#include "DifferentialAdhesionGeneralisedLinearSpringForce.hpp"
#include "DifferentialAdhesionLinearSpringForce.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "Force.hpp"
#include "CellECMAdhesionForce.hpp"
#include "RandomMotionForce.hpp"

#include "OffLatticeSimulation.hpp"
#include "CellPopulationAdjacencyMatrixWriter.hpp"

#include "CellIdWriter.hpp"
#include "CellMutationStatesWriter.hpp"

#include "LuminalCellProperty.hpp"
#include "MyoepithelialCellProperty.hpp"
#include "MammaryCellTypeWriter.hpp"

#include "PetscSetupAndFinalize.hpp"

static const double M_TIME_TO_STEADY_STATE = 10; //10
static const double M_TIME_FOR_SIMULATION = 100; //100
static const double M_NUM_CELLS_ACROSS = 20; //20
static const double M_CELL_FLUCTUATION = 1.0;

class TestCellSorting : public AbstractCellBasedWithTimingsTestSuite
{

public:

    void xTestDifferentialAdhesionCellSorting()
    {
        HoneycombMeshGenerator generator(M_NUM_CELLS_ACROSS, M_NUM_CELLS_ACROSS, 0);
        TetrahedralMesh<2,2>* p_generating_mesh = generator.GetMesh();

        //Extended to allow sorting for longer distances
        double cut_off_length = 2.5;

        // Convert this to a NodesOnlyMesh
        NodesOnlyMesh<2> mesh;
        mesh.ConstructNodesWithoutMesh(*p_generating_mesh, cut_off_length);

        // Set up cells, one for each Node
        std::vector<CellPtr> cells;
        MAKE_PTR(DifferentiatedCellProliferativeType, p_differentiated_type);
        CellsGenerator<UniformG1GenerationalCellCycleModel, 2> cells_generator;
        cells_generator.GenerateBasicRandom(cells, mesh.GetNumNodes(), p_differentiated_type);

        // Create cell population
        NodeBasedCellPopulation<2> cell_population(mesh, cells);

        // Set population to output all data to results files
        cell_population.AddCellWriter<CellIdWriter>();
        cell_population.AddCellWriter<CellLabelWriter>();
        cell_population.AddCellWriter<CellMutationStatesWriter>();
        cell_population.AddPopulationWriter<HeterotypicBoundaryLengthWriter>();
        cell_population.AddPopulationWriter<CellPopulationAdjacencyMatrixWriter>();

        /* We randomly label some cells using the cell property {{{CellLabel}}}. We begin by creating a shared pointer to
         * this cell property using the helper singleton {{{CellPropertyRegistry}}}. We then loop over the cells and label
         * each cell independently with probability 0.5. 
        */
        boost::shared_ptr<AbstractCellProperty> p_label(CellPropertyRegistry::Instance()->Get<CellLabel>());
        for (AbstractCellPopulation<2>::Iterator cell_iter = cell_population.Begin();
             cell_iter != cell_population.End();
             ++cell_iter)
        {
            if (RandomNumberGenerator::Instance()->ranf() < 0.5)
            {
                cell_iter->AddCellProperty(p_label);
            }
        }

        // Set up cell-based simulation and output directory
        OffLatticeSimulation<2> simulator(cell_population);
        simulator.SetOutputDirectory("CellSorting/DifferentialAdhesion");

        // Set time step and end time for simulation
        simulator.SetDt(1.0/200.0);
        simulator.SetSamplingTimestepMultiple(200);
        simulator.SetEndTime(M_TIME_TO_STEADY_STATE);

        // Create a force law and pass it to the simulation
        MAKE_PTR(DifferentialAdhesionLinearSpringForce<2>, p_differential_adhesion_force);
        p_differential_adhesion_force->SetMeinekeSpringStiffness(50.0);
        p_differential_adhesion_force->SetHomotypicLabelledSpringConstantMultiplier(1.0);
        p_differential_adhesion_force->SetHeterotypicSpringConstantMultiplier(0.1);
        p_differential_adhesion_force->SetCutOffLength(cut_off_length);
        simulator.AddForce(p_differential_adhesion_force);

        // Add some noise to avoid local minimum
        MAKE_PTR(RandomMotionForce<2>, p_random_force);
        p_random_force->SetMovementParameter(0.05); //0.1 causes dissasociation, 0.001 is not enough
        simulator.AddForce(p_random_force);

        // Run simulation
        simulator.Solve();

        // Adjust parameters
        p_random_force->SetMovementParameter(0.05*M_CELL_FLUCTUATION); //0.1 causes dissasociation

        // Run simulation
        simulator.SetEndTime(M_TIME_TO_STEADY_STATE + M_TIME_FOR_SIMULATION);
        simulator.Solve();
    }
    
    void xTestDifferentialAdhesionCellSortingWithParticles()
    {
        // Create a simple mesh
        HoneycombMeshGenerator generator(10, 8);
        MutableMesh<2,2>* p_generating_mesh = generator.GetMesh();
        
        //Extended to allow sorting for longer distances
        double cut_off_length = 2.5;

        // Convert this to a NodesOnlyMesh
        NodesOnlyMesh<2> mesh;
        mesh.ConstructNodesWithoutMesh(*p_generating_mesh, cut_off_length);

        // Create vector of cell location indices
        std::vector<unsigned> cell_location_indices;
        for (unsigned i=10; i<mesh.GetNumNodes(); i++)
        {
            if (i != 80)
            {
                cell_location_indices.push_back(i);
            }
        }

        // Set up cells
        std::vector<CellPtr> cells;
        MAKE_PTR(DifferentiatedCellProliferativeType, p_differentiated_type);
        CellsGenerator<UniformG1GenerationalCellCycleModel, 2> cells_generator;
        cells_generator.GenerateBasicRandom(cells, cell_location_indices.size(), p_differentiated_type);

        // Create a cell population, with some random particles
        NodeBasedCellPopulationWithParticles<2> cell_population(mesh, cells, cell_location_indices);

        // Set population to output all data to results files
        cell_population.AddCellWriter<CellIdWriter>();
        cell_population.AddCellWriter<CellLabelWriter>();
        cell_population.AddCellWriter<CellMutationStatesWriter>();

        /* We randomly label some cells using the cell property {{{CellLabel}}}. We begin by creating a shared pointer to
         * this cell property using the helper singleton {{{CellPropertyRegistry}}}. We then loop over the cells and label
         * each cell independently with probability 0.5. 
         */
        boost::shared_ptr<AbstractCellProperty> p_label(CellPropertyRegistry::Instance()->Get<CellLabel>());
        for (AbstractCellPopulation<2>::Iterator cell_iter = cell_population.Begin();
             cell_iter != cell_population.End();
             ++cell_iter)
        {
            if (RandomNumberGenerator::Instance()->ranf() < 0.5)
            {
                cell_iter->AddCellProperty(p_label);
            }
        }

        // Set up cell-based simulation and output directory
        OffLatticeSimulation<2> simulator(cell_population);
        simulator.SetOutputDirectory("CellSorting/DifferentialAdhesion/WithParticles");

        // Set time step and end time for simulation
        simulator.SetDt(1.0/200.0);
        simulator.SetSamplingTimestepMultiple(200);
        simulator.SetEndTime(M_TIME_TO_STEADY_STATE);

        // Create a force law and pass it to the simulation
        MAKE_PTR(DifferentialAdhesionLinearSpringForce<2>, p_differential_adhesion_force);
        p_differential_adhesion_force->SetMeinekeSpringStiffness(50.0);
        p_differential_adhesion_force->SetHomotypicLabelledSpringConstantMultiplier(1.0);
        p_differential_adhesion_force->SetHeterotypicSpringConstantMultiplier(0.1);
        p_differential_adhesion_force->SetCutOffLength(cut_off_length);
        simulator.AddForce(p_differential_adhesion_force);

        // Add some noise to avoid local minimum
        MAKE_PTR(RandomMotionForce<2>, p_random_force);
        p_random_force->SetMovementParameter(0.05); //0.1 causes dissasociation, 0.001 is not enough
        simulator.AddForce(p_random_force);

        // Run simulation
        simulator.Solve();

        // Adjust parameters
        p_random_force->SetMovementParameter(0.05*M_CELL_FLUCTUATION); //0.1 causes dissasociation

        // Run simulation
        simulator.SetEndTime(M_TIME_TO_STEADY_STATE + M_TIME_FOR_SIMULATION);
        simulator.Solve();
    }

    void xTestCellECMAdhesionCellSorting()
    {
        // We use the HoneycombMeshGenerator to create a honeycomb mesh covering a circular domain of given radius
        HoneycombMeshGenerator generator(M_NUM_CELLS_ACROSS, M_NUM_CELLS_ACROSS);
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
        cell_population.AddCellWriter<CellLabelWriter>();
        cell_population.AddCellWriter<CellMutationStatesWriter>();
        cell_population.AddPopulationWriter<HeterotypicBoundaryLengthWriter>();
        cell_population.AddPopulationWriter<CellPopulationAdjacencyMatrixWriter>();

        /* We randomly label some cells using the cell property {{{CellLabel}}}. We begin by creating a shared pointer to
         * this cell property using the helper singleton {{{CellPropertyRegistry}}}. We then loop over the cells and label
         * each cell independently with probability 0.5. 
        */
        boost::shared_ptr<AbstractCellProperty> p_label(CellPropertyRegistry::Instance()->Get<CellLabel>());
        for (AbstractCellPopulation<2>::Iterator cell_iter = cell_population.Begin();
             cell_iter != cell_population.End();
             ++cell_iter)
        {
            if (RandomNumberGenerator::Instance()->ranf() < 0.5)
            {
                cell_iter->AddCellProperty(p_label);
            }
        }

        // Set up cell-based simulation and output directory
        OffLatticeSimulation<2> simulator(cell_population);
        simulator.SetOutputDirectory("CellSorting/CellECMAdhesion");
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(10.0);

        // We create a force law and pass it to the
        MAKE_PTR(GeneralisedLinearSpringForce<2>, p_linear_force);
        p_linear_force->SetCutOffLength(1.5);
        simulator.AddForce(p_linear_force);

        // Create a force law and pass it to the simulation
        MAKE_PTR(Force<2>, p_force);
        simulator.AddForce(p_force);

        // Run simulation
        simulator.Solve();

        // Run simulation
        simulator.Solve();
    }
    
    void TestMammaryCellECMAdhesionCellSorting()
    {
        // We use the HoneycombMeshGenerator to create a honeycomb mesh covering a circular domain of given radius
        HoneycombMeshGenerator generator(M_NUM_CELLS_ACROSS, M_NUM_CELLS_ACROSS);
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
        cell_population.AddCellWriter<CellMutationStatesWriter>();
        cell_population.AddPopulationWriter<HeterotypicBoundaryLengthWriter>();
        cell_population.AddPopulationWriter<CellPopulationAdjacencyMatrixWriter>();

        // Add a cell writer so that mammary cell types are written to file
        cell_population.AddCellWriter<MammaryCellTypeWriter>();

        /* We randomly assign some cells luminal or myoepithelial using the cell property {{{LuminalCellProperty or MyoepithleilaCellproperty}}}. 
         * We begin by creating a shared pointer to this cell property using the helper singleton {{{CellPropertyRegistry}}}.  
         * We then loop over the cells and label each cell independently with probability 0.5. 
        */
        boost::shared_ptr<AbstractCellProperty> p_luminal(cell_population.GetCellPropertyRegistry()->Get<LuminalCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_myo(cell_population.GetCellPropertyRegistry()->Get<MyoepithelialCellProperty>());

        for (AbstractCellPopulation<2>::Iterator cell_iter = cell_population.Begin();
             cell_iter != cell_population.End();
             ++cell_iter)
        {
            if (RandomNumberGenerator::Instance()->ranf() < 0.5)
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
        simulator.SetOutputDirectory("CellSorting/CellECMAdhesion/Mammary");
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(10.0);

        // We create a force law and pass it to the
        MAKE_PTR(GeneralisedLinearSpringForce<2>, p_linear_force);
        p_linear_force->SetCutOffLength(1.5);
        simulator.AddForce(p_linear_force);

        // Create a force law and pass it to the simulation
        MAKE_PTR(CellECMAdhesionForce<2>, p_force);
        simulator.AddForce(p_force);

        // Add some noise to avoid local minimum
        MAKE_PTR(RandomMotionForce<2>, p_random_force);
        p_random_force->SetMovementParameter(0.05); //0.1 causes dissasociation, 0.001 is not enough
        simulator.AddForce(p_random_force);

        // Run simulation
        simulator.Solve();

        // Adjust parameters
        p_random_force->SetMovementParameter(0.05*M_CELL_FLUCTUATION); //0.1 causes dissasociation
    }
};

#endif /*TESTCELLSORTING_HPP_*/