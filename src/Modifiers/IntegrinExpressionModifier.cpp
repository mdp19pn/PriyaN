#include "IntegrinExpressionModifier.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "AbstractMammaryCellProperty.hpp"
#include "LuminalCellProperty.hpp"
#include "MyoepithelialCellProperty.hpp"
#include "LuminalStemCellProperty.hpp"
#include "MyoepithelialStemCellProperty.hpp"
#include "Debug.hpp"

template<unsigned DIM>
IntegrinExpressionModifier<DIM>::IntegrinExpressionModifier()
    : AbstractCellBasedSimulationModifier<DIM>(),
	  mIntegrinExpressionModified(false),
	  mIntegrinExpressionModificationTime(0.0),
	  mLuminalCellsAffected(false),
	  mMyoepithelialCellsAffected(false),
	  mB1GainOfFunction(false),
	  mB1LossOfFunction(false),
	  mB4GainOfFunction(false),
	  mB4LossOfFunction(false)
{
}

template<unsigned DIM>
IntegrinExpressionModifier<DIM>::~IntegrinExpressionModifier()
{
}

template<unsigned DIM>
void IntegrinExpressionModifier<DIM>::UpdateAtEndOfTimeStep(AbstractCellPopulation<DIM,DIM>& rCellPopulation)
{
    UpdateCellData(rCellPopulation);
}

template<unsigned DIM>
void IntegrinExpressionModifier<DIM>::SetupSolve(AbstractCellPopulation<DIM,DIM>& rCellPopulation, std::string outputDirectory)
{
	// We must have at least one mammary cell lineage affected by the gain/loss of function
	if (mLuminalCellsAffected == false)
	{
		assert(mMyoepithelialCellsAffected == true);
	}
	if (mMyoepithelialCellsAffected == false)
	{
		assert(mLuminalCellsAffected == true);
	}
	
	// We cannot have both a gain and loss of function of a given integrin
    if (mB1GainOfFunction == true)
	{
		assert(mB1LossOfFunction == false);
	}
    if (mB1LossOfFunction == true)
	{
		assert(mB1GainOfFunction == false);
	}
    if (mB4GainOfFunction == true)
	{
		assert(mB4LossOfFunction == false);
	}
    if (mB4LossOfFunction == true)
	{
		assert(mB4GainOfFunction == false);
	}
}

template<unsigned DIM>
void IntegrinExpressionModifier<DIM>::UpdateCellData(AbstractCellPopulation<DIM,DIM>& rCellPopulation)
{
    // Make sure the cell population is updated
    rCellPopulation.Update();

	if (mIntegrinExpressionModified == false)
	{	
		if (SimulationTime::Instance()->GetTime() >= mIntegrinExpressionModificationTime)
		{
			// Iterate over cell population
			for (typename AbstractCellPopulation<DIM>::Iterator cell_iter = rCellPopulation.Begin();
				cell_iter != rCellPopulation.End();
				++cell_iter)
			{
				unsigned node_index = rCellPopulation.GetLocationIndexUsingCell(*cell_iter);
PRINT_VARIABLE("a");
				// Determine cell type
       			CellPtr p_cell = rCellPopulation.GetCellUsingLocationIndex(node_index);
        		bool cell_is_luminal = p_cell->template HasCellProperty<LuminalCellProperty>();
       			bool cell_is_myo = p_cell->template HasCellProperty<MyoepithelialCellProperty>();
        		bool cell_is_luminal_stem = p_cell->template HasCellProperty<LuminalStemCellProperty>();
        		bool cell_is_myo_stem = p_cell->template HasCellProperty<MyoepithelialStemCellProperty>();

				if (mLuminalCellsAffected == true)
				{
					PRINT_VARIABLE("b")
					if (cell_is_luminal || cell_is_luminal_stem)
					{
						CellPropertyCollection collection = p_cell->rGetCellPropertyCollection().GetProperties<LuminalCellProperty>();
            			boost::shared_ptr<LuminalCellProperty> p_luminal = boost::static_pointer_cast<LuminalCellProperty>(collection.GetProperty());
PRINT_VARIABLE("c")
						if (mB1GainOfFunction)
						{
							p_luminal->SetB1IntegrinExpression(true);
						}
						if (mB1LossOfFunction)
						{
							p_luminal->SetB1IntegrinExpression(false);
						}
						if (mB4GainOfFunction)
						{
							p_luminal->SetB4IntegrinExpression(true);
						}
						if (mB4LossOfFunction)
						{
							p_luminal->SetB4IntegrinExpression(false);
						}
					}
				}
				if (mMyoepithelialCellsAffected == true)
				{
					PRINT_VARIABLE("d")
					if (cell_is_myo || cell_is_myo_stem)
					{
						PRINT_VARIABLE("e")
						CellPropertyCollection collection = p_cell->rGetCellPropertyCollection().GetProperties<MyoepithelialCellProperty>();
						boost::shared_ptr<MyoepithelialCellProperty> p_myo = boost::static_pointer_cast<MyoepithelialCellProperty>(collection.GetProperty());
PRINT_VARIABLE("f")
						if (mB1GainOfFunction)
						{
							p_myo->SetB1IntegrinExpression(true);
						}
						if (mB1LossOfFunction)
						{
							p_myo->SetB1IntegrinExpression(false);
						}
						if (mB4GainOfFunction)
						{
							p_myo->SetB4IntegrinExpression(true);
						}
						if (mB4LossOfFunction)
						{
							p_myo->SetB4IntegrinExpression(false);
						}
					}
				}
			}
PRINT_VARIABLE("g")			
			mIntegrinExpressionModified = true;
			PRINT_VARIABLE("h")
		}
	}
}

template<unsigned DIM>
void IntegrinExpressionModifier<DIM>::OutputSimulationModifierParameters(out_stream& rParamsFile)
{
    // No parameters to output, so just call method on direct parent class
    AbstractCellBasedSimulationModifier<DIM>::OutputSimulationModifierParameters(rParamsFile);
}

template<unsigned DIM>
void IntegrinExpressionModifier<DIM>::SetIntegrinExpressionModificationTime(double integrinExpressionModificationTime)
{
    mIntegrinExpressionModificationTime = integrinExpressionModificationTime;
}

template<unsigned DIM>
void IntegrinExpressionModifier<DIM>::SetLuminalCellsAffected(bool luminalCellsAffected)
{
	mLuminalCellsAffected = luminalCellsAffected;
}

template<unsigned DIM>
void IntegrinExpressionModifier<DIM>::SetMyoepithelialCellsAffected(bool myoepithelialCellsAffected)
{
	mMyoepithelialCellsAffected = myoepithelialCellsAffected;
}

template<unsigned DIM>
void IntegrinExpressionModifier<DIM>::SetB1GainOfFunction(bool b1GainOfFunction)
{
    mB1GainOfFunction = b1GainOfFunction;
}

template<unsigned DIM>
void IntegrinExpressionModifier<DIM>::SetB1LossOfFunction(bool b1LossOfFunction)
{
    mB1LossOfFunction = b1LossOfFunction;
}

template<unsigned DIM>
void IntegrinExpressionModifier<DIM>::SetB4GainOfFunction(bool b4GainOfFunction)
{
    mB4GainOfFunction = b4GainOfFunction;
}

template<unsigned DIM>
void IntegrinExpressionModifier<DIM>::SetB4LossOfFunction(bool b4LossOfFunction)
{
    mB4LossOfFunction = b4LossOfFunction;
}

// Explicit instantiation
template class IntegrinExpressionModifier<1>;
template class IntegrinExpressionModifier<2>;
template class IntegrinExpressionModifier<3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(IntegrinExpressionModifier)