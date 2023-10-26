#ifndef LV_XML_ELEMENT_H_
#define LV_XML_ELEMENT_H_

#include "../../lvgl.h"

typedef struct lv_xml_attribute_t
{
    char *name;
    void *context;
    void (*apply_cb)(struct lv_xml_attribute_t *attribute, lv_obj_t *obj, char *value);
} lv_xml_attribute_t;

typedef struct lv_xml_element_t
{
    struct lv_xml_element_t *base;
    char *name;
    const lv_obj_class_t *obj_class;
    lv_xml_attribute_t *attributes;
    size_t attributes_count;
} lv_xml_element_t;

lv_xml_attribute_t *lv_xml_element_get_attribute(lv_xml_element_t *element, char *name);

#endif
