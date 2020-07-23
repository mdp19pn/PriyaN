#include "MyoepithelialCellProperty.hpp"

MyoepithelialCellProperty::MyoepithelialCellProperty(bool b1IntegrinExpression, bool b4IntegrinExpression)
    : AbstractCellProperty(),
      mB1IntegrinExpression(b1IntegrinExpression),
      mB4IntegrinExpression(b4IntegrinExpression)
{
}

MyoepithelialCellProperty::~MyoepithelialCellProperty()
{
}

unsigned MyoepithelialCellProperty::GetColour() const
{
    return 1.0*(mB1IntegrinExpression) + 2.0*(mB4IntegrinExpression);
}

bool MyoepithelialCellProperty::GetB1IntegrinExpression() const
{
    return mB1IntegrinExpression;
}

bool MyoepithelialCellProperty::GetB4IntegrinExpression() const
{
    return mB4IntegrinExpression;
}

#include "SerializationExportWrapperForCpp.hpp"
// Declare identifier for the serializer
CHASTE_CLASS_EXPORT(MyoepithelialCellProperty)
