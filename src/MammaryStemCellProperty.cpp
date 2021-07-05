#include "MammaryStemCellProperty.hpp"

MammaryStemCellProperty::MammaryStemCellProperty(bool b1IntegrinExpression, bool b4IntegrinExpression)
    : AbstractMammaryCellProperty(b1IntegrinExpression, b4IntegrinExpression)
{
}

MammaryStemCellProperty::~MammaryStemCellProperty()
{
}

#include "SerializationExportWrapperForCpp.hpp"
// Declare identifier for the serializer
CHASTE_CLASS_EXPORT(MammaryStemCellProperty)
