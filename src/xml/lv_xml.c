#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../lvgl.h"
#include "lv_xml.h"

#include "elements/lv_xml_obj.h"
#include "elements/lv_xml_button.h"
#include "elements/lv_xml_label.h" 
#include "elements/lv_xml_switch.h"

#define YXML_BUFFER_SIZE    4096

// TODO:
// * Implement elements and attributes
// * Deal with const discard warnings
// * Figure out meta elements (lvgl, resources, include, etc) and validate content
// * Add support for variables
// * Benchmark parser

const lv_xml_element_t *elements[] =
{
    &lv_xml_obj_element,
    &lv_xml_button_element,
    &lv_xml_label_element,
    &lv_xml_switch_element
};

static const size_t elements_count = sizeof(elements) / sizeof(lv_xml_element_t *);

lv_obj_t *lv_xml_inflate(const char *xml)
{
    yxml_t *x = malloc(sizeof(yxml_t) + YXML_BUFFER_SIZE);
    yxml_init(x, x + 1, YXML_BUFFER_SIZE);

    lv_obj_t *current_obj = NULL;
    lv_ll_t element_stack;
    _lv_ll_init(&element_stack, sizeof(lv_xml_element_t *));

    lv_xml_attribute_t *current_attribute = NULL;
    char *attribute_value = malloc(sizeof(char) * 1024);
    size_t attribute_value_length = 0;

    size_t xml_len = strlen(xml);
    for(size_t i = 0; i < xml_len; i++)
    {
        yxml_ret_t r = yxml_parse(x, xml[i]);
        switch (r)
        {
            case YXML_ELEMSTART:
            {
                //Find element for tag
                for (uint16_t e = 0; e < elements_count; e++)
                {
                    //If element matches tag name
                    lv_xml_element_t *element = elements[e];
                    if (!strcmp(x->elem, element->name))
                    {
                        //Push onto stack
                        lv_xml_element_t **head = _lv_ll_ins_head(&element_stack);
                        *head = element;

                        //If element is associated with an LVGL object
                        if (element->obj_class)
                        {
                            //Create object to represent element
                            current_obj = lv_obj_class_create_obj(element->obj_class, current_obj);
                            lv_obj_class_init_obj(current_obj);
                        }
                        break;
                    }
                }
                break;
            }

            case YXML_ELEMEND:
            {
                //Get current element
                lv_xml_element_t **head = _lv_ll_get_head(&element_stack);
                if (head)
                {
                    //Set current object to parent (if any)
                    lv_obj_t *parent = lv_obj_get_parent(current_obj);
                    if (parent)
                    { current_obj = parent; }

                    //Pop element off the stack
                    _lv_ll_remove(&element_stack, head);
                }
                break;
            }

            case YXML_ATTRSTART:
            {
                //Reset attribute value length
                attribute_value_length = 0;

                //Get current element
                lv_xml_element_t **head = _lv_ll_get_head(&element_stack);
                if (head)
                {
                    //Attempt to get attribute for element
                    current_attribute = lv_xml_element_get_attribute(*head, x->attr);
                }
                break;
            }

            case YXML_ATTRVAL:
            {
                //Append next character to attribute value (only if attribute valid)
                if (current_attribute)
                { attribute_value[attribute_value_length++] = x->data[0]; }
                break;
            }

            case YXML_ATTREND:
            {
                //Valid attribute finished
                if (current_attribute)
                {
                    //Finalize attribute value and apply attribute
                    attribute_value[attribute_value_length++] = '\0';
                    current_attribute->apply_cb(current_attribute, current_obj, attribute_value);

                    //Reset current attribute
                    current_attribute = NULL;
                }
                break;
            }

            case YXML_EEOF:
            case YXML_EREF:
            case YXML_ECLOSE:
            case YXML_ESTACK:
            case YXML_ESYN:
            {
                lv_log("Failed to parse XML; Error: %d\n", r);
                return NULL;
            }
            default:
                break;
        }
    }

    //Ensure xml file valid
    yxml_ret_t r = yxml_eof(x);
    if(r < 0)
    { return NULL; }

    free(xml);
    free(attribute_value);
    free(x);

    //Return root object
    return current_obj;
}
