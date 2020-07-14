#include "LuminalCellProperty.hpp"

LuminalCellProperty::LuminalCellProperty()
    : AbstractCellMutationState(0)
{}

#include "SerializationExportWrapperForCpp.hpp"
// Declare identifier for the serializer
CHASTE_CLASS_EXPORT(LuminalCellProperty)
