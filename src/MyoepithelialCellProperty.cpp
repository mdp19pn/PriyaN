#include "MyoepithelialCellProperty.hpp"

MyoepithelialCellProperty::MyoepithelialCellProperty(unsigned colour)
    : AbstractCellProperty(),
      mColour(colour)
{
}

MyoepithelialCellProperty::~MyoepithelialCellProperty()
{
}

unsigned MyoepithelialCellProperty::GetColour() const
{
    return mColour;
}

#include "SerializationExportWrapperForCpp.hpp"
// Declare identifier for the serializer
CHASTE_CLASS_EXPORT(MyoepithelialCellProperty)
