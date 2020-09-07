set(ATLAS_WIDGETS_ROOT ${ATLAS_SOURCE_ROOT}/atlas/gui/widgets)

set(ATLAS_INCLUDE_WIDGETS_LIST
    ${ATLAS_WIDGETS_ROOT}/widget.hpp
    ${ATLAS_WIDGETS_ROOT}/app_properties_widget.hpp
    ${ATLAS_WIDGETS_ROOT}/fps_widget.hpp
    PARENT_SCOPE)

set(ATLAS_SOURCE_WIDGETS_LIST
    ${ATLAS_WIDGETS_ROOT}/app_properties_widget.cpp
    ${ATLAS_WIDGETS_ROOT}/fps_widget.cpp
    PARENT_SCOPE)
