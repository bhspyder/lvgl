#include "lv_xml_element.h"

lv_xml_attribute_t *lv_xml_element_get_attribute(lv_xml_element_t *element, char *name)
{   
    //Favor matching of element attributes first
    for (int i = 0; i < element->attributes_count; i++)
    {
        lv_xml_attribute_t *attribute = &element->attributes[i];
        if (!strcmp(name, attribute->name))
        { return attribute; }
    }

    //Search for match among the element base attributes
    if (element->base)
    { return lv_xml_element_get_attribute(element->base, name); }

    //No match found
    return NULL;
}
