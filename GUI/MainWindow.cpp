#include "MainWindow.h"

namespace cagd
{
    MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
    {
        setupUi(this);

    /*

      the structure of the main window's central widget

     *---------------------------------------------------*
     |                 central widget                    |
     |                                                   |
     |  *---------------------------*-----------------*  |
     |  |     rendering context     |   scroll area   |  |
     |  |       OpenGL widget       | *-------------* |  |
     |  |                           | | side widget | |  |
     |  |                           | |             | |  |
     |  |                           | |             | |  |
     |  |                           | *-------------* |  |
     |  *---------------------------*-----------------*  |
     |                                                   |
     *---------------------------------------------------*

    */
        _side_widget = new SideWidget(this);

        _scroll_area = new QScrollArea(this);
        _scroll_area->setWidget(_side_widget);
        _scroll_area->setSizePolicy(_side_widget->sizePolicy());
        _scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        _gl_widget = new GLWidget(this);

        centralWidget()->setLayout(new QHBoxLayout());
        centralWidget()->layout()->addWidget(_gl_widget);
        centralWidget()->layout()->addWidget(_scroll_area);


        connect(_side_widget->tabWidget, SIGNAL(currentChanged(int)), _gl_widget, SLOT(set_tab(int)));

        // creating a signal slot mechanism between the rendering context and the side widget
        connect(_side_widget->rotate_x_slider, SIGNAL(valueChanged(int)), _gl_widget, SLOT(set_angle_x(int)));
        connect(_side_widget->rotate_y_slider, SIGNAL(valueChanged(int)), _gl_widget, SLOT(set_angle_y(int)));
        connect(_side_widget->rotate_z_slider, SIGNAL(valueChanged(int)), _gl_widget, SLOT(set_angle_z(int)));

        connect(_side_widget->derivatives_scale_slider, SIGNAL(valueChanged(int)), _gl_widget, SLOT(set_derivatives_scale(int)));

        connect(_side_widget->zoom_factor_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_zoom_factor(double)));

        connect(_side_widget->trans_x_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trans_x(double)));
        connect(_side_widget->trans_y_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trans_y(double)));
        connect(_side_widget->trans_z_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trans_z(double)));

        connect(_side_widget->chb_first_der, SIGNAL(clicked(bool)), _gl_widget, SLOT(set_parametric_first_derivatives(bool)));
        connect(_side_widget->chb_second_der, SIGNAL(clicked(bool)), _gl_widget, SLOT(set_parametric_second_derivatives(bool)));

        connect(_side_widget->trig_sb_x, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trig_point_x(double)));
        connect(_side_widget->trig_sb_y, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trig_point_y(double)));
        connect(_side_widget->trig_sb_z, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trig_point_z(double)));

        connect(_gl_widget, SIGNAL(update_trig_point_x(double)), _side_widget->trig_sb_x, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(update_trig_point_y(double)), _side_widget->trig_sb_y, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(update_trig_point_z(double)), _side_widget->trig_sb_z, SLOT(setValue(double)));

        connect(_side_widget->trig_point_cb, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_trig_point(int)));

        connect(_side_widget->alpha_sb, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_alpha(double)));
        connect(_gl_widget, SIGNAL(update_alpha(double)), _side_widget->alpha_sb, SLOT(setValue(double)));

        connect(_side_widget->patch_row_cb, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_patch_row(int)));
        connect(_side_widget->patch_col_cb, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_patch_col(int)));

        connect(_gl_widget, SIGNAL(update_patch_point_x(double)), _side_widget->patch_x_sb, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(update_patch_point_y(double)), _side_widget->patch_y_sb, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(update_patch_point_z(double)), _side_widget->patch_z_sb, SLOT(setValue(double)));

        connect(_gl_widget, SIGNAL(update_alpha_u(double)), _side_widget->patch_a_u, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(update_alpha_v(double)), _side_widget->patch_a_v, SLOT(setValue(double)));

        connect(_gl_widget, SIGNAL(update_translated_x(double)), _side_widget->patch_translate_x, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(update_translated_y(double)), _side_widget->patch_translate_y, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(update_translated_z(double)), _side_widget->patch_translate_z, SLOT(setValue(double)));

        connect(_side_widget->patch_x_sb, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_patch_point_x(double)));
        connect(_side_widget->patch_y_sb, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_patch_point_y(double)));
        connect(_side_widget->patch_z_sb, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_patch_point_z(double)));

        connect(_side_widget->patch_a_u, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_patch_alpha_u(double)));
        connect(_side_widget->patch_a_v, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_patch_alpha_v(double)));

        connect(_side_widget->trig_control_points_chb, SIGNAL(clicked(bool)), _gl_widget, SLOT(set_show_control_points(bool)));
        connect(_side_widget->trig_control_net_chb, SIGNAL(clicked(bool)), _gl_widget, SLOT(set_show_control_net(bool)));

        connect(_side_widget->patch_uip_chb, SIGNAL(clicked(bool)), _gl_widget, SLOT(show_patch_uip(bool)));
        connect(_side_widget->patch_vip_chb, SIGNAL(clicked(bool)), _gl_widget, SLOT(show_patch_vip(bool)));


        // curves for project


        connect(_gl_widget, SIGNAL(add_curve_to_list(int)), this, SLOT(add_to_curve_list(int)));

        connect(_side_widget->add_curve_button, SIGNAL(clicked()), _gl_widget, SLOT(add_trig_curve()));

        connect(_side_widget->extend_curve_button, SIGNAL(clicked()), _gl_widget, SLOT(curve_extend()));
        connect(_side_widget->join_curve_button, SIGNAL(clicked()), _gl_widget, SLOT(curve_join()));
        connect(_side_widget->merge_curve_button, SIGNAL(clicked()), _gl_widget, SLOT(curve_merge()));

        connect(_side_widget->trig_arc_sb, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_selected_curve(int)));

        connect(_side_widget->operations_curve1, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_curve1(int)));
        connect(_side_widget->operations_curve1_point, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_curve1_point(int)));
        connect(_side_widget->operations_curve2, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_curve2(int)));
        connect(_side_widget->operations_curve2_point, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_curve2_point(int)));

        // patches for project

        connect(_side_widget->patch_add, SIGNAL(clicked()), _gl_widget, SLOT(add_trig_patch()));

        connect(_side_widget->selected_shader, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_selected_shader(int)));
        connect(_side_widget->smoothing_sb, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_smoothing(double)));
        connect(_side_widget->scaling_sb, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_scaling(double)));
        connect(_side_widget->shading_sb, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_shading(double)));



        connect(_gl_widget, SIGNAL(add_patch_to_list(int)), this, SLOT(add_to_patch_list(int)));

        connect(_side_widget->patch_translate_x, SIGNAL(valueChanged(double)), _gl_widget, SLOT(patch_translate_x(double)));
        connect(_side_widget->patch_translate_y, SIGNAL(valueChanged(double)), _gl_widget, SLOT(patch_translate_y(double)));
        connect(_side_widget->patch_translate_z, SIGNAL(valueChanged(double)), _gl_widget, SLOT(patch_translate_z(double)));

        connect(_side_widget->selected_patch_cb, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_selected_patch(int)));

        connect(_side_widget->patch_extend_b, SIGNAL(clicked()), _gl_widget, SLOT(patch_extend()));
        connect(_side_widget->patch_join_b, SIGNAL(clicked()), _gl_widget, SLOT(patch_join()));
        connect(_side_widget->patch_merge_b, SIGNAL(clicked()), _gl_widget, SLOT(patch_merge()));

        connect(_side_widget->patch_extend_cb, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_extend_direction(int)));

        connect(_side_widget->patch_func_1, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_patch_func_1(int)));
        connect(_side_widget->patch_func_2, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_patch_func_2(int)));

        connect(_side_widget->patch_side_1, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_patch_side_1(int)));
        connect(_side_widget->patch_side_2, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_patch_side_2(int)));


    }

    //--------------------------------
    // implementation of private slots
    //--------------------------------
    void MainWindow::on_action_Quit_triggered()
    {
        qApp->exit(0);
    }

    void MainWindow::add_to_curve_list(int value) {
        _side_widget->trig_arc_sb->addItem("Curve " + QString::number(value));
        _side_widget->operations_curve1->addItem("Curve " + QString::number(value));
        _side_widget->operations_curve2->addItem("Curve " + QString::number(value));
    }

    void MainWindow::add_to_patch_list(int value) {
        _side_widget->selected_patch_cb->addItem("Patch " + QString::number(value));
        _side_widget->patch_func_1->addItem("Patch " + QString::number(value));
        _side_widget->patch_func_2->addItem("Patch " + QString::number(value));
    }
}
