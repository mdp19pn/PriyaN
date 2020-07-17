#include "LuminalCellProperty.hpp"

LuminalCellProperty::LuminalCellProperty(unsigned colour)
    : AbstractCellProperty(),
      mColour(colour)
{
}

LuminalCellProperty::~LuminalCellProperty()
{
}

unsigned LuminalCellProperty::GetColour() const
{
    return mColour;
}

#include "SerializationExportWrapperForCpp.hpp"
// Declare identifier for the serializer
CHASTE_CLASS_EXPORT(LuminalCellProperty)
