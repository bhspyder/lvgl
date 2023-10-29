#include <stdlib.h>

#include "lv_xml_obj.h"
#include "lv_xml_checkbox.h"

static void apply_text(lv_xml_attribute_t *attribute, lv_obj_t *obj, char *value);

static const lv_xml_attribute_t attributes[] =
{
    { .name = "text", .apply_cb = apply_text }
};

const lv_xml_element_t lv_xml_checkbox_element =
{
    .base = &lv_xml_obj_element,
    .name = "checkbox",
    .obj_class = &lv_checkbox_class,
    .attributes = attributes,
    .attributes_count = sizeof(attributes) / sizeof(lv_xml_attribute_t)
};

static void apply_text(lv_xml_attribute_t *attribute, lv_obj_t *obj, char *value)
{ lv_checkbox_set_text(obj, value); }
