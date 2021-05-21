#pragma once

#include <GL/glew.h>
#include <QOpenGLWidget>
#include "../Parametric/ParametricCurves3.h"
#include "../Parametric/ParametricSurfaces3.h"
#include "../Core/GenericCurves3.h"
#include "../Core/Materials.h"
#include "../Core/TriangulatedMeshes3.h"
#include "../Core/Lights.h"
#include "../Cyclic/CyclicCurves3.h"
#include "../Trigonometric/SecondOrderTrigonometricArc.h"
#include "../Trigonometric/SecondOrderTrigonometricPatch.h"
#include "../Core/ShaderPrograms.h"
#include <QTimer>
#include <QImage>
#include <QOpenGLTexture>

namespace cagd
{
    class GLWidget: public QOpenGLWidget
    {
        Q_OBJECT

    private:
        int _current_tab;

        // variables defining the projection matrix
        double       _aspect;            // aspect ratio of the rendering window
        double       _fovy;              // field of view in direction y
        double       _z_near, _z_far;    // distance of near and far clipping planes

        // variables defining the model-view matrix
        double       _eye[3], _center[3], _up[3];

        // variables needed by transformations
        int         _angle_x, _angle_y, _angle_z;
        double      _zoom;
        double      _trans_x, _trans_y, _trans_z;
        double      _derivatives_scale;
        bool        _parametric_first_derivatives;
        bool        _parametric_second_derivatives;
        bool        _cyclic_first_derivatives;
        bool        _cyclic_second_derivatives;
        bool        _apply_texture;


        int         _selected_material;
        int         _selected_texture;
        int         _selected_trig_curve;
        int         _selected_patch;
        int         _trig_point;

        int         _curve1;
        int         _curve2;
        int         _curve1_point;
        int         _curve2_point;

        RowMatrix<SecondOrderTrigArc*> _trig_curve;
        RowMatrix<GenericCurve3*> _image_of_trig_curve;

        QOpenGLTexture *texture[4];
        Material _materials[7];
        DirectionalLight *_dl = nullptr;

        RowMatrix<SecTrigPatch3*>       _patch;

        int                 _patch_row;
        int                 _patch_col;
        bool                _show_control_net;
        bool                _show_control_points;
        bool                _show_uip;
        bool                _show_vip;

        RowMatrix<ShaderProgram>    _shaders;
        float                       _scale_parameter;
        float                       _smoothing_parameter;
        float                       _shading_parameter;
        int                         _selected_shader;


        int _patch_extend_direction;
        int _patch_func_1;
        int _patch_func_2;
        int _patch_side_1;
        int _patch_side_2;


    public:
        // special and default constructor
        // the format specifies the properties of the rendering window
        GLWidget(QWidget* parent = 0);

        // redeclared virtual functions
        void initializeGL();
        void paintGL();
        void resizeGL(int w, int h);

        void paintTrigonometricCurves();
        void paintTrigonometricCurveOperations();
        void paintSecondTrigPatch();
        void paintSecondTrigPatchOperations();

        void initializeSecondTrigPatch();
        void initializeShaders();

        void enableShader();
        void disableShader();

        void addTrigCurve();
        void addTrigPatch();

        void deleteCyclicCurves();

        virtual ~GLWidget();

    signals:
        void update_trig_point_x(double value);
        void update_trig_point_y(double value);
        void update_trig_point_z(double value);

        void update_alpha(double value);

        void update_patch_point_x(double value);
        void update_patch_point_y(double value);
        void update_patch_point_z(double value);

        void update_alpha_u(double value);
        void update_alpha_v(double value);

        void update_translated_x(double value);
        void update_translated_y(double value);
        void update_translated_z(double value);

        void add_curve_to_list(int value);

        void add_patch_to_list(int value);

    public slots:
        // public event handling methods/slots

        void set_tab(int value);

        void set_angle_x(int value);
        void set_angle_y(int value);
        void set_angle_z(int value);

        void set_zoom_factor(double value);

        void set_trans_x(double value);
        void set_trans_y(double value);
        void set_trans_z(double value);

        void set_parametric_first_derivatives(bool value);
        void set_parametric_second_derivatives(bool value);

        void set_trig_point(int value);
        void set_trig_point_x(double value);
        void set_trig_point_y(double value);
        void set_trig_point_z(double value);

        void set_selected_material(int value);

        void set_apply_texture(bool value);
        void set_selected_texture(int value);

        void add_trig_curve();
        void add_trig_patch();

        void set_curve1(int value);
        void set_curve2(int value);
        void set_curve1_point(int value);
        void set_curve2_point(int value);

        void regenerate_curve(int n);

        void curve_extend();
        void curve_join();
        void curve_merge();

        void set_alpha(double value);
        void set_derivatives_scale(int value);

        void set_patch_alpha_u(double value);
        void set_patch_alpha_v(double value);
        void set_patch_row(int value);
        void set_patch_col(int value);

        void set_patch_point_x(double value);
        void set_patch_point_y(double value);
        void set_patch_point_z(double value);

        void show_patch_uip(bool value);
        void show_patch_vip(bool value);

        void set_show_control_points(bool value);
        void set_show_control_net(bool value);

        void set_selected_shader(int value);
        void set_smoothing(double value);
        void set_scaling(double value);
        void set_shading(double value);


        void set_selected_patch(int value);
        void set_selected_curve(int value);

        void patch_translate_x(double value);
        void patch_translate_y(double value);
        void patch_translate_z(double value);

        void patch_extend();
        void patch_join();
        void patch_merge();

        void set_extend_direction(int value);

        void set_patch_func_1(int value);
        void set_patch_func_2(int value);

        void set_patch_side_1(int value);
        void set_patch_side_2(int value);

    };
}
