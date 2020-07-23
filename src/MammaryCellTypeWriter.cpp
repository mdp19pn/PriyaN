#include "MammaryCellTypeWriter.hpp"
#include "AbstractCellPopulation.hpp"
#include "LuminalCellProperty.hpp"
#include "MyoepithelialCellProperty.hpp"

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
MammaryCellTypeWriter<ELEMENT_DIM, SPACE_DIM>::MammaryCellTypeWriter()
    : AbstractCellWriter<ELEMENT_DIM, SPACE_DIM>("results.viztypes")
{
    this->mVtkCellDataName = "Mammary cell types";
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
double MammaryCellTypeWriter<ELEMENT_DIM, SPACE_DIM>::GetCellDataForVtkOutput(CellPtr pCell, AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation)
{
    double colour = 0.0;
    if (pCell->HasCellProperty<LuminalCellProperty>())
    {
        CellPropertyCollection collection = pCell->rGetCellPropertyCollection().GetProperties<LuminalCellProperty>();
        boost::shared_ptr<LuminalCellProperty> p_prop = boost::static_pointer_cast<LuminalCellProperty>(collection.GetProperty());
        colour = double(p_prop->GetColour());
    }
    else if (pCell->HasCellProperty<MyoepithelialCellProperty>())
    {
        CellPropertyCollection collection = pCell->rGetCellPropertyCollection().GetProperties<MyoepithelialCellProperty>();
        boost::shared_ptr<MyoepithelialCellProperty> p_prop = boost::static_pointer_cast<MyoepithelialCellProperty>(collection.GetProperty());
        colour = 4.0 + double(p_prop->GetColour());
    }
    return colour;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void MammaryCellTypeWriter<ELEMENT_DIM, SPACE_DIM>::VisitCell(CellPtr pCell, AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation)
{
    double cell_type = 0.0;
    double b1_expn = 0.0;
    double b4_expn = 0.0;
    if (pCell->HasCellProperty<LuminalCellProperty>())
    {
        CellPropertyCollection collection = pCell->rGetCellPropertyCollection().GetProperties<LuminalCellProperty>();
        boost::shared_ptr<LuminalCellProperty> p_prop = boost::static_pointer_cast<LuminalCellProperty>(collection.GetProperty());
        b1_expn = double(p_prop->GetB1IntegrinExpression());
        b4_expn = double(p_prop->GetB4IntegrinExpression());
    }
    else if (pCell->HasCellProperty<MyoepithelialCellProperty>())
    {
        cell_type = 1.0;
        CellPropertyCollection collection = pCell->rGetCellPropertyCollection().GetProperties<MyoepithelialCellProperty>();
        boost::shared_ptr<MyoepithelialCellProperty> p_prop = boost::static_pointer_cast<MyoepithelialCellProperty>(collection.GetProperty());
        b1_expn = double(p_prop->GetB1IntegrinExpression());
        b4_expn = double(p_prop->GetB4IntegrinExpression());
    }

    *this->mpOutStream << " " << cell_type << " " << b1_expn << " " << b4_expn;

    unsigned location_index = pCellPopulation->GetLocationIndexUsingCell(pCell);
    *this->mpOutStream << " " << location_index;

    c_vector<double, SPACE_DIM> coords = pCellPopulation->GetLocationOfCellCentre(pCell);
    for (unsigned i=0; i<SPACE_DIM; i++)
    {
        *this->mpOutStream << " " << coords[i];
    }
}

// Explicit instantiation
template class MammaryCellTypeWriter<1,1>;
template class MammaryCellTypeWriter<1,2>;
template class MammaryCellTypeWriter<2,2>;
template class MammaryCellTypeWriter<1,3>;
template class MammaryCellTypeWriter<2,3>;
template class MammaryCellTypeWriter<3,3>;

#include "SerializationExportWrapperForCpp.hpp"
// Declare identifier for the serializer
EXPORT_TEMPLATE_CLASS_ALL_DIMS(MammaryCellTypeWriter)
