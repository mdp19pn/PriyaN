#include "LuminalStemCellProperty.hpp"

LuminalStemCellProperty::LuminalStemCellProperty(bool b1IntegrinExpression, bool b4IntegrinExpression)
    : AbstractMammaryCellProperty(b1IntegrinExpression, b4IntegrinExpression)
{
}

LuminalStemCellProperty::~LuminalStemCellProperty()
{
}

#include "SerializationExportWrapperForCpp.hpp"
// Declare identifier for the serializer
CHASTE_CLASS_EXPORT(LuminalStemCellProperty)
