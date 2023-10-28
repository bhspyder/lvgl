#include "lv_xml_obj.h"
#include "lv_xml_slider.h"

const lv_xml_element_t lv_xml_slider_element =
{
    .base = &lv_xml_obj_element,
    .name = "slider",
    .obj_class = &lv_slider_class,
    .attributes = NULL,
    .attributes_count = 0
};