#include "MyoepithelialCellProperty.hpp"

MyoepithelialCellProperty::MyoepithelialCellProperty()
    : AbstractCellMutationState(0)
{}

#include "SerializationExportWrapperForCpp.hpp"
// Declare identifier for the serializer
CHASTE_CLASS_EXPORT(MyoepithelialCellProperty)
