#include "MyoepithelialCellProperty.hpp"
#include "DifferentiatedCellProliferativeType.hpp"

MyoepithelialCellProperty::MyoepithelialCellProperty(bool b1IntegrinExpression, bool b4IntegrinExpression)
    : AbstractMammaryCellProperty(b1IntegrinExpression, b4IntegrinExpression)
{
}

MyoepithelialCellProperty::~MyoepithelialCellProperty()
{
}

#include "SerializationExportWrapperForCpp.hpp"
// Declare identifier for the serializer
CHASTE_CLASS_EXPORT(MyoepithelialCellProperty)
