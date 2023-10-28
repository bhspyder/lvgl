#include "lv_xml_obj.h"
#include "lv_xml_dropdown.h"

const lv_xml_element_t lv_xml_dropdown_element =
{
    .base = &lv_xml_obj_element,
    .name = "dropdown",
    .obj_class = &lv_dropdown_class,
    .attributes = NULL,
    .attributes_count = 0
};