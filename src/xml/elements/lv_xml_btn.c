#include "lv_xml_obj.h"
#include "lv_xml_btn.h"

const lv_xml_element_t lv_xml_btn_element =
{
    .base = &lv_xml_obj_element,
    .name = "button",
    .obj_class = &lv_button_class,
    .attributes = NULL,
    .attributes_count = 0
};
