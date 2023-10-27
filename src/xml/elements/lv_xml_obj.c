#include <stdio.h>
#include <stdlib.h>

#include "lv_xml_obj.h"

static void apply_width(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value);
static void apply_height(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value);
static void apply_content_width(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value);
static void apply_content_height(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value);
static void apply_x(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value);
static void apply_y(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value);
static void apply_align(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value);
static void apply_layout(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value);
static void apply_ext_click_area(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value);
static void apply_scrollbar_mode(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value);
static void apply_scroll_direction(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value);
static void apply_scroll_snap_x(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value);
static void apply_scroll_snap_y(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value);
static void apply_flex_flow(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value);
static void apply_flex_align(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value);
static void apply_flex_grow(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value);
static void apply_flag(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value);

static const lv_xml_attribute_t attributes[] =
{
    //Size and position
    {.name = "width", .apply_cb = apply_width },
    {.name = "height", .apply_cb = apply_height },
    {.name = "content-width", .apply_cb = apply_content_width },
    {.name = "content-height", .apply_cb = apply_content_height },
    {.name = "x", .apply_cb = apply_x },
    {.name = "y", .apply_cb = apply_y },
    {.name = "align", .apply_cb = apply_align },
    {.name = "layout", .apply_cb = apply_layout },
    {.name = "ext-click-area", .apply_cb = apply_ext_click_area },

    //Scrolling
    {.name = "scrollbar-mode", .apply_cb = apply_scrollbar_mode },
    {.name = "scroll-direction", .apply_cb = apply_scroll_direction },
    {.name = "scroll-snap-x", .apply_cb = apply_scroll_snap_x },
    {.name = "scroll-snap-y", .apply_cb = apply_scroll_snap_y },

    //Flags
    {.name = "hidden", .apply_cb = apply_flag, .context = (void *)LV_OBJ_FLAG_HIDDEN },
    {.name = "clickable", .apply_cb = apply_flag, .context = (void *)LV_OBJ_FLAG_CLICKABLE },
    {.name = "click-focusable", .apply_cb = apply_flag, .context = (void *)LV_OBJ_FLAG_CLICK_FOCUSABLE },
    {.name = "checkable", .apply_cb = apply_flag, .context = (void *)LV_OBJ_FLAG_CHECKABLE },
    {.name = "scrollable", .apply_cb = apply_flag, .context = (void *)LV_OBJ_FLAG_SCROLLABLE },
    {.name = "scroll-elastic", .apply_cb = apply_flag, .context = (void *)LV_OBJ_FLAG_SCROLL_ELASTIC },
    {.name = "scroll-momentum", .apply_cb = apply_flag, .context = (void *)LV_OBJ_FLAG_SCROLL_MOMENTUM },
    {.name = "scroll-one", .apply_cb = apply_flag, .context = (void *)LV_OBJ_FLAG_SCROLL_ONE },
    {.name = "scroll-chain-x", .apply_cb = apply_flag, .context = (void *)LV_OBJ_FLAG_SCROLL_CHAIN_HOR },
    {.name = "scroll-chain-y", .apply_cb = apply_flag, .context = (void *)LV_OBJ_FLAG_SCROLL_CHAIN_VER },
    {.name = "scroll-chain", .apply_cb = apply_flag, .context = (void *)LV_OBJ_FLAG_SCROLL_CHAIN },
    {.name = "scroll-on-focus", .apply_cb = apply_flag, .context = (void *)LV_OBJ_FLAG_SCROLL_ON_FOCUS },
    {.name = "scroll-with-arrow", .apply_cb = apply_flag, .context = (void *)LV_OBJ_FLAG_SCROLL_WITH_ARROW },
    {.name = "snappable", .apply_cb = apply_flag, .context = (void *)LV_OBJ_FLAG_SNAPPABLE },
    {.name = "press-lock", .apply_cb = apply_flag, .context = (void *)LV_OBJ_FLAG_PRESS_LOCK },
    {.name = "event-bubble", .apply_cb = apply_flag, .context = (void *)LV_OBJ_FLAG_EVENT_BUBBLE },
    {.name = "gesture-bubble", .apply_cb = apply_flag, .context = (void *)LV_OBJ_FLAG_GESTURE_BUBBLE },
    {.name = "advanced-hit-test", .apply_cb = apply_flag, .context = (void *)LV_OBJ_FLAG_ADV_HITTEST },
    {.name = "ignore-layout", .apply_cb = apply_flag, .context = (void *)LV_OBJ_FLAG_IGNORE_LAYOUT },
    {.name = "floating", .apply_cb = apply_flag, .context = (void *)LV_OBJ_FLAG_FLOATING },
    {.name = "overflow-visible", .apply_cb = apply_flag, .context =(void *) LV_OBJ_FLAG_OVERFLOW_VISIBLE },
    {.name = "layout-1", .apply_cb = apply_flag, .context = (void *)LV_OBJ_FLAG_LAYOUT_1 },
    {.name = "layoyt-2", .apply_cb = apply_flag, .context = (void *)LV_OBJ_FLAG_LAYOUT_2 },
    {.name = "widget-1", .apply_cb = apply_flag, .context = (void *)LV_OBJ_FLAG_WIDGET_1 },
    {.name = "widget-2", .apply_cb = apply_flag, .context = (void *)LV_OBJ_FLAG_WIDGET_2 },
    {.name = "user-1", .apply_cb = apply_flag, .context = (void *)LV_OBJ_FLAG_USER_1 },
    {.name = "user-2", .apply_cb = apply_flag, .context = (void *)LV_OBJ_FLAG_USER_2 },
    {.name = "user-3", .apply_cb = apply_flag, .context = (void *)LV_OBJ_FLAG_USER_3 },
    {.name = "user-4", .apply_cb = apply_flag, .context = (void *)LV_OBJ_FLAG_USER_4 },

    //Flex
    {.name = "flex-flow", .apply_cb = apply_flex_flow },
    {.name = "flex-align", .apply_cb = apply_flex_align },
    {.name = "flex-grow", .apply_cb = apply_flex_grow }
};

const lv_xml_element_t lv_xml_obj_element =
{
    .base = NULL,
    .name = "object",
    .obj_class = &lv_obj_class,
    .attributes = attributes,
    .attributes_count = sizeof(attributes) / sizeof(lv_xml_attribute_t)
};

static void apply_width(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value)
{
    uint32_t width;
    char unit = 0;
    if (sscanf(value, "%d%c", &width, &unit))
    {
        if (unit == '%')
        { width = lv_pct(width); }
        
        lv_obj_set_width(obj, width);
    }
    else if (!strcmp(value, "content"))
    { lv_obj_set_width(obj, LV_SIZE_CONTENT); }
}

static void apply_height(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value)
{
    uint32_t height;
    char unit = 0;
    if (sscanf(value, "%d%c", &height, &unit))
    {
        if (unit == '%')
        { height = lv_pct(height); }

        lv_obj_set_height(obj, height);
    }
    else if (!strcmp(value, "content"))
    { lv_obj_set_height(obj, LV_SIZE_CONTENT); }
}

static void apply_content_width(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value)
{
    uint32_t width;
    char unit = 0;
    if (sscanf(value, "%d%c", &width, &unit))
    {
        if (unit == '%')
        { width = lv_pct(width); }

        lv_obj_set_content_width(obj, width);
    }
    else if (!strcmp(value, "content"))
    { lv_obj_set_content_width(obj, LV_SIZE_CONTENT); }
}

static void apply_content_height(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value)
{
    uint32_t height;
    char unit = 0;
    if (sscanf(value, "%d%c", &height, &unit))
    {
        if (unit == '%')
        { height = lv_pct(height); }

        lv_obj_set_content_height(obj, height);
    }
    else if (!strcmp(value, "content"))
    { lv_obj_set_content_height(obj, LV_SIZE_CONTENT); }
}

static void apply_x(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value)
{
    uint32_t x;
    char unit = 0;

    if (sscanf(value, "%d%c", &x, &unit))
    {
        if (unit == '%')
        { x = lv_pct(x); }

        lv_obj_set_x(obj, x);
    }
}

static void apply_y(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value)
{
    uint32_t y;
    char unit = 0;
    if (sscanf(value, "%d%c", &y, &unit))
    {
        if (unit == '%')
        { y = lv_pct(y); }

        lv_obj_set_y(obj, y);
    }
}

static void apply_align(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value)
{
    lv_align_t align = LV_ALIGN_DEFAULT;
    if (!strcmp(value, "top-left")) align = LV_ALIGN_TOP_LEFT;
    else if (!strcmp(value, "top-mid")) align = LV_ALIGN_TOP_MID;
    else if (!strcmp(value, "top-right")) align = LV_ALIGN_TOP_RIGHT;
    else if (!strcmp(value, "bottom-left")) align = LV_ALIGN_BOTTOM_LEFT;
    else if (!strcmp(value, "bottom-mid")) align = LV_ALIGN_BOTTOM_MID;
    else if (!strcmp(value, "bottom-right")) align = LV_ALIGN_BOTTOM_RIGHT;
    else if (!strcmp(value, "left-mid")) align = LV_ALIGN_LEFT_MID;
    else if (!strcmp(value, "right-mid")) align = LV_ALIGN_RIGHT_MID;
    else if (!strcmp(value, "center")) align = LV_ALIGN_CENTER;

    lv_obj_set_align(obj, align);
}

static void apply_layout(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value)
{
    lv_layout_t layout = LV_LAYOUT_NONE;
    if (!strcmp(value, "none")) layout = LV_LAYOUT_NONE;
    else if (!strcmp(value, "flex")) layout = LV_LAYOUT_FLEX;
    else if (!strcmp(value, "grid")) layout = LV_LAYOUT_GRID;

    lv_obj_set_layout(obj, layout);
}

static void apply_ext_click_area(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value)
{
    uint8_t size = atoi(value);
    lv_obj_set_ext_click_area(obj, size);
}

static void apply_scrollbar_mode(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value)
{
    lv_scrollbar_mode_t mode = LV_SCROLLBAR_MODE_OFF;
    if (!strcmp(value, "off")) mode = LV_SCROLLBAR_MODE_OFF;
    else if (!strcmp(value, "on")) mode = LV_SCROLLBAR_MODE_ON;
    else if (!strcmp(value, "active")) mode = LV_SCROLLBAR_MODE_ACTIVE;
    else if (!strcmp(value, "auto")) mode = LV_SCROLLBAR_MODE_AUTO;

    lv_obj_set_scrollbar_mode(obj, mode);
}

static void apply_scroll_direction(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value)
{
    lv_dir_t direction = LV_DIR_NONE;
    if (!strcmp(value, "none")) direction = LV_DIR_NONE;
    else if (!strcmp(value, "left")) direction = LV_DIR_LEFT;
    else if (!strcmp(value, "right")) direction = LV_DIR_RIGHT;
    else if (!strcmp(value, "top")) direction = LV_DIR_TOP;
    else if (!strcmp(value, "bottom")) direction = LV_DIR_BOTTOM;
    else if (!strcmp(value, "horizontal")) direction = LV_DIR_HOR;
    else if (!strcmp(value, "vertical")) direction = LV_DIR_VER;
    else if (!strcmp(value, "all")) direction = LV_DIR_ALL;

    lv_obj_set_scroll_dir(obj, direction);
}

static void apply_scroll_snap_x(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value)
{
    lv_scroll_snap_t snap = LV_SCROLL_SNAP_NONE;
    if (!strcmp(value, "none")) snap = LV_SCROLL_SNAP_NONE;
    else if (!strcmp(value, "start")) snap = LV_SCROLL_SNAP_START;
    else if (!strcmp(value, "end")) snap = LV_SCROLL_SNAP_END;
    else if (!strcmp(value, "center")) snap = LV_SCROLL_SNAP_CENTER;

    lv_obj_set_scroll_snap_x(obj, snap);
}

static void apply_scroll_snap_y(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value)
{
    lv_scroll_snap_t snap = LV_SCROLL_SNAP_NONE;
    if (!strcmp(value, "none")) snap = LV_SCROLL_SNAP_NONE;
    else if (!strcmp(value, "start")) snap = LV_SCROLL_SNAP_START;
    else if (!strcmp(value, "end")) snap = LV_SCROLL_SNAP_END;
    else if (!strcmp(value, "center")) snap = LV_SCROLL_SNAP_CENTER;

    lv_obj_set_scroll_snap_y(obj, snap);
}

static void apply_flag(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value)
{
    bool enabled = false;
    if (!strcmp(value, "true")) enabled = true;
    else if (!strcmp(value, "false")) enabled = false;

    lv_obj_flag_t flag = (lv_obj_flag_t)attribute->context;
    if (enabled)
    { lv_obj_add_flag(obj, flag); }
    else
    { lv_obj_remove_flag(obj, flag); }
}

static void apply_flex_flow(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value)
{
    lv_flex_flow_t flow = LV_FLEX_FLOW_ROW;
    if (!strcmp(value, "row")) flow = LV_FLEX_FLOW_ROW;
    else if (!strcmp(value, "column")) flow = LV_FLEX_FLOW_COLUMN;
    else if (!strcmp(value, "row-wrap")) flow = LV_FLEX_FLOW_ROW_WRAP;
    else if (!strcmp(value, "row-reverse")) flow = LV_FLEX_FLOW_ROW_REVERSE;
    else if (!strcmp(value, "row-wrap-reverse")) flow = LV_FLEX_FLOW_ROW_WRAP_REVERSE;
    else if (!strcmp(value, "column-wrap")) flow = LV_FLEX_FLOW_COLUMN_WRAP;
    else if (!strcmp(value, "column-reverse")) flow = LV_FLEX_FLOW_COLUMN_REVERSE;
    else if (!strcmp(value, "column-wrap-reverse")) flow = LV_FLEX_FLOW_COLUMN_WRAP_REVERSE;

    lv_obj_set_flex_flow(obj, flow);
}

static void apply_flex_align(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value)
{
    char values[3][15];
    if (sscanf(value, "%13s %13s %13s", values[0], values[1], values[2]) == 3)
    {
        lv_flex_align_t align[3] = { LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START };
        for (uint8_t i = 0; i < 3; i++)
        {
            if (!strcmp(values[i], "start")) align[i] = LV_FLEX_ALIGN_START;
            else if (!strcmp(values[i], "end")) align[i] = LV_FLEX_ALIGN_END;
            else if (!strcmp(values[i], "center")) align[i] = LV_FLEX_ALIGN_CENTER;
            else if (i != 1 && !strcmp(values[i], "space-evenly")) align[i] = LV_FLEX_ALIGN_SPACE_EVENLY;
            else if (i != 1 && !strcmp(values[i], "space-around")) align[i] = LV_FLEX_ALIGN_SPACE_AROUND;
            else if (i != 1 && !strcmp(values[i], "space-between")) align[i] = LV_FLEX_ALIGN_SPACE_BETWEEN;
        }
        lv_obj_set_flex_align(obj, align[0], align[1], align[2]);
    }
}

static void apply_flex_grow(lv_xml_attribute_t* attribute, lv_obj_t* obj, char* value)
{
    uint8_t grow = atoi(value);
    lv_obj_set_flex_grow(obj, grow);
}
