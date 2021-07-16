#include "MyoepithelialStemCellProperty.hpp"

MyoepithelialStemCellProperty::MyoepithelialStemCellProperty(bool b1IntegrinExpression, bool b4IntegrinExpression)
    : AbstractMammaryCellProperty(b1IntegrinExpression, b4IntegrinExpression)
{
}

MyoepithelialStemCellProperty::~MyoepithelialStemCellProperty()
{
}

#include "SerializationExportWrapperForCpp.hpp"
// Declare identifier for the serializer
CHASTE_CLASS_EXPORT(MyoepithelialStemCellProperty)
