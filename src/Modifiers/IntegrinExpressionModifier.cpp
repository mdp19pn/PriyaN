#include "IntegrinExpressionModifier.hpp"
#include "NodeBasedCellPopulation.hpp"

template<unsigned DIM>
IntegrinExpressionModifier<DIM>::IntegrinExpressionModifier()
    : AbstractCellBasedSimulationModifier<DIM>(),
	  mIntegrinExpressionModified(false),
	  mIntegrinExpressionModificationTime(0.0),
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
				// Get this cell's mammary cell property, so we can alter its mB1IntegrinExpression and/or mB4IntegrinExpression value(s)
				boost::shared_ptr<AbstractCellProperty> p_mammary_cell_property =
					mpCell->rGetCellPropertyCollection().GetCellPropertyRegistry()->Get<AbstractMammaryCellProperty >();

				if (mB1GainOfFunction)
				{
					p_mammary_cell_property->SetB1IntegrinExpression(true);
				}
				if (mB1LossOfFunction)
				{
					p_mammary_cell_property->SetB1IntegrinExpression(false);
				}
				if (mB4GainOfFunction)
				{
					p_mammary_cell_property->SetB4IntegrinExpression(true);
				}
				if (mB4LossOfFunction)
				{
					p_mammary_cell_property->SetB4IntegrinExpression(false);
				}
			}
			
			mIntegrinExpressionModified = true;
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

