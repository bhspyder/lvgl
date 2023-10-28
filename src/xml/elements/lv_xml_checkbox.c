#include "lv_xml_obj.h"
#include "lv_xml_checkbox.h"

const lv_xml_element_t lv_xml_checkbox_element =
{
    .base = &lv_xml_obj_element,
    .name = "checkbox",
    .obj_class = &lv_checkbox_class,
    .attributes = NULL,
    .attributes_count = 0
};