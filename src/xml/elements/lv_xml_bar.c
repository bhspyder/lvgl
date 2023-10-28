#include "lv_xml_obj.h"
#include "lv_xml_bar.h"

const lv_xml_element_t lv_xml_bar_element =
{
    .base = &lv_xml_obj_element,
    .name = "bar",
    .obj_class = &lv_bar_class,
    .attributes = NULL,
    .attributes_count = 0
};
