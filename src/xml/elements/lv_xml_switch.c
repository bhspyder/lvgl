#include "lv_xml_obj.h"
#include "lv_xml_switch.h"

const lv_xml_element_t lv_xml_switch_element =
{
    .base = &lv_xml_obj_element,
    .name = "switch",
    .obj_class = &lv_switch_class,
    .attributes = NULL,
    .attributes_count = 0
};