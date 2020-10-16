#include "LuminalCellProperty.hpp"

LuminalCellProperty::LuminalCellProperty(bool b1IntegrinExpression, bool b4IntegrinExpression)
    : AbstractMammaryCellProperty(b1IntegrinExpression, b4IntegrinExpression)
{
}

LuminalCellProperty::~LuminalCellProperty()
{
}

#include "SerializationExportWrapperForCpp.hpp"
// Declare identifier for the serializer
CHASTE_CLASS_EXPORT(LuminalCellProperty)
