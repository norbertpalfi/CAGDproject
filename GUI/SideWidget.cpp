#include "SideWidget.h"
#include <string>

namespace cagd
{
    SideWidget::SideWidget(QWidget *parent): QWidget(parent)
    {
        setupUi(this);

        QPalette p = rotate_x_slider->palette();

        p.setColor(QPalette::Highlight, QColor(255,50,10));

        rotate_x_slider->setPalette(p);

        // y

        p = rotate_y_slider->palette();

        p.setColor(QPalette::Highlight, QColor(50,255,10));

        rotate_y_slider->setPalette(p);

        // z

        p = rotate_z_slider->palette();

        p.setColor(QPalette::Highlight, QColor(50,10,255));

        rotate_z_slider->setPalette(p);

        // derivatives

        p = derivatives_scale_slider->palette();

        p.setColor(QPalette::Highlight, QColor(255,255,10));

        derivatives_scale_slider->setPalette(p);



        trig_point_cb->addItem("1 - Pink");
        trig_point_cb->addItem("2 - Yellow");
        trig_point_cb->addItem("3 - Green");
        trig_point_cb->addItem("4 - Blue");


        operations_curve1_point->addItem("1 - Pink");
        operations_curve1_point->addItem("4 - Blue");


        operations_curve2_point->addItem("1 - Pink");
        operations_curve2_point->addItem("4 - Blue");

        // Patches

        patch_row_cb->addItem("1");
        patch_row_cb->addItem("2");
        patch_row_cb->addItem("3");
        patch_row_cb->addItem("4");

        patch_col_cb->addItem("1");
        patch_col_cb->addItem("2");
        patch_col_cb->addItem("3");
        patch_col_cb->addItem("4");

        selected_shader->addItem("Directional Light");
        selected_shader->addItem("2SidedLighting");
        selected_shader->addItem("Toon");
        selected_shader->addItem("Reflection");

        patch_extend_cb->addItem("N");
        patch_extend_cb->addItem("NE");
        patch_extend_cb->addItem("E");
        patch_extend_cb->addItem("SE");
        patch_extend_cb->addItem("S");
        patch_extend_cb->addItem("SW");
        patch_extend_cb->addItem("W");
        patch_extend_cb->addItem("NW");

        patch_side_1->addItem("N - White");
        patch_side_1->addItem("E - Cyan");
        patch_side_1->addItem("S - Blue");
        patch_side_1->addItem("W - Yellow");

        patch_side_2->addItem("N - White");
        patch_side_2->addItem("E - Cyan");
        patch_side_2->addItem("S - Blue");
        patch_side_2->addItem("W - Yellow");




    }
}
