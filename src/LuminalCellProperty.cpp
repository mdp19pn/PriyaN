#include "LuminalCellProperty.hpp"

LuminalCellProperty::LuminalCellProperty(bool b1IntegrinExpression, bool b4IntegrinExpression)
    : AbstractCellProperty(),
      mB1IntegrinExpression(b1IntegrinExpression),
      mB4IntegrinExpression(b4IntegrinExpression)
{
}

LuminalCellProperty::~LuminalCellProperty()
{
}

unsigned LuminalCellProperty::GetColour() const
{
    return 1.0*(mB1IntegrinExpression) + 2.0*(mB4IntegrinExpression);
}

bool LuminalCellProperty::GetB1IntegrinExpression() const
{
    return mB1IntegrinExpression;
}

bool LuminalCellProperty::GetB4IntegrinExpression() const
{
    return mB4IntegrinExpression;
}

#include "SerializationExportWrapperForCpp.hpp"
// Declare identifier for the serializer
CHASTE_CLASS_EXPORT(LuminalCellProperty)
