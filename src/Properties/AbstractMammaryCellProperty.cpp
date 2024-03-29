#include "AbstractMammaryCellProperty.hpp"
#include "Exception.hpp"

AbstractMammaryCellProperty::AbstractMammaryCellProperty(bool b1IntegrinExpression, bool b4IntegrinExpression)
    : AbstractCellProperty(),
      mB1IntegrinExpression(b1IntegrinExpression),
      mB4IntegrinExpression(b4IntegrinExpression)
{
}

AbstractMammaryCellProperty::~AbstractMammaryCellProperty()
{
}

unsigned AbstractMammaryCellProperty::GetColour() const
{
    return 1.0*(mB1IntegrinExpression) + 2.0*(mB4IntegrinExpression);
}

bool AbstractMammaryCellProperty::GetB1IntegrinExpression() const
{
    return mB1IntegrinExpression;
}

bool AbstractMammaryCellProperty::GetB4IntegrinExpression() const
{
    return mB4IntegrinExpression;
}

void AbstractMammaryCellProperty::SetB1IntegrinExpression(bool b1IntegrinExpression)
{
	mB1IntegrinExpression = b1IntegrinExpression;
}

void AbstractMammaryCellProperty::SetB4IntegrinExpression(bool b4IntegrinExpression)
{
	mB4IntegrinExpression = b4IntegrinExpression;
}