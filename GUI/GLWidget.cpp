#include "GLWidget.h"

#if !defined(__APPLE__)
#include <GL/glu.h>
#endif

#include <iostream>
#include "../Core/Matrices.h"
#include "../Test/TestFunctions.h"
#include "../Core/Materials.h"
#include "../Core/Constants.h"

using namespace std;

#include <Core/Exceptions.h>

namespace cagd
{
//--------------------------------
// special and default constructor
//--------------------------------
GLWidget::GLWidget(QWidget *parent): QOpenGLWidget(parent)
{
    _current_tab = 0;
    _derivatives_scale = 1.0;
    _selected_material = 0;
    _apply_texture = false;
    _selected_texture = 0;

    _parametric_first_derivatives = false;
    _parametric_second_derivatives = false;

    _cyclic_first_derivatives = true;
    _cyclic_second_derivatives = true;

    _trig_point = 0;
    _show_uip = false;
    _show_vip = false;

    _curve1 = 0;
    _curve2 = 0;
    _curve1_point = 0;
    _curve2_point = 0;

    _materials[0] = MatFBBrass;
    _materials[1] = MatFBGold;
    _materials[2] = MatFBSilver;
    _materials[3] = MatFBEmerald;
    _materials[4] = MatFBPearl;
    _materials[5] = MatFBRuby;
    _materials[6] = MatFBTurquoise;

    _patch_row = 0;
    _patch_col = 0;
    _show_control_net = true;
    _show_control_points = true;

    _scale_parameter = 30.0f;
    _smoothing_parameter = 1.0f;
    _shading_parameter = 1.0f;
    _selected_shader = 1;

    _selected_trig_curve = 0;

    _selected_patch = 0;
    _patch_extend_direction = 0;
    _patch_func_1 = 0;
    _patch_func_2 = 0;
    _patch_side_1 = 0;
    _patch_side_2 = 0;
}

GLWidget:: ~GLWidget() {
    if (_dl) {
        delete _dl; _dl = nullptr;
    }
}

void GLWidget::initializeSecondTrigPatch() {
    int n = _patch.GetColumnCount();
    _patch.ResizeColumns(n+1);
    n = n-1;
    _patch[n] = new (nothrow) SecTrigPatch3();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            _patch[n]->SetData(i, j, j - 1.5, 1.5 - i, 0.0);
        }
    }

    _patch[n]->UpdateVertexBufferObjectsOfData();
    _patch[n]->SetImage(_patch[n]->GenerateImage(30, 30, GL_STATIC_DRAW));

    DCoordinate3 p;
    _patch[n]->GetData(_patch_row, _patch_col, p);

    emit update_patch_point_x(p.x());
    emit update_patch_point_y(p.y());
    emit update_patch_point_z(p.z());

    emit update_alpha_u(_patch[n]->GetUAlpha());
    emit update_alpha_v(_patch[n]->GetVAlpha());

    emit add_patch_to_list(n);

    _patch[n]->SetUip(_patch[n]->GenerateUIsoparametricLines(30, 2, 100));
    _patch[n]->SetVip(_patch[n]->GenerateVIsoparametricLines(30, 2, 100));
}

void GLWidget::add_trig_patch() {
    int n = _patch.GetColumnCount();
    _patch.ResizeColumns(n+1);
    n = n-1;
    _patch[n] = new (nothrow) SecTrigPatch3();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            _patch[n]->SetData(i, j, j - 1.5, 1.5 - i, 0.0);
        }
    }

    _patch[n]->UpdateVertexBufferObjectsOfData();
    _patch[n]->SetImage(_patch[n]->GenerateImage(30, 30, GL_STATIC_DRAW));

    DCoordinate3 p;
    _patch[n]->GetData(_patch_row, _patch_col, p);

    emit add_patch_to_list(n);
    _patch[n]->SetUip(_patch[n]->GenerateUIsoparametricLines(30, 2, 100));
    _patch[n]->SetVip(_patch[n]->GenerateVIsoparametricLines(30, 2, 100));
    update();
}

void GLWidget::initializeShaders() {
    try {
        _shaders.ResizeColumns(4);
        if (!_shaders[0].InstallShaders("Shaders/directional_light.vert","Shaders/directional_light.frag"))
        {
            throw Exception("Directional light creation error");
        }

        if (!_shaders[1].InstallShaders("Shaders/two_sided_lighting.vert","Shaders/two_sided_lighting.frag"))
        {
            throw Exception("Two sided lighting creation error");
        }

        if (!_shaders[2].InstallShaders("Shaders/toon.vert","Shaders/toon.frag"))
        {
            throw Exception("Toon creation error.");
        } else {
            _shaders[2].Enable();
            _shaders[2].SetUniformVariable4f("default_outline_color", 60.0f, 160.0f, 60.0f, 1.0f);
            _shaders[2].Disable();
        }

        if (!_shaders[3].InstallShaders("Shaders/reflection_lines.vert","Shaders/reflection_lines.frag"))
        {
            throw Exception("Reflection lines creation error");
        } else {
            _shaders[3].Enable();
            _shaders[3].SetUniformVariable1f("scale_factor", _scale_parameter);
            _shaders[3].SetUniformVariable1f("smoothing", _smoothing_parameter);
            _shaders[3].SetUniformVariable1f("shading", _shading_parameter);
            _shaders[3].Disable();
        }

    } catch (Exception& e) {
        cout << e << endl;
    }
}

//--------------------------------------------------------------------------------------
// this virtual function is called once before the first call to paintGL() or resizeGL()
//--------------------------------------------------------------------------------------
void GLWidget::initializeGL()
{
    // creating a perspective projection matrix
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    _aspect = static_cast<double>(width()) / static_cast<double>(height());
    _z_near = 1.0;
    _z_far  = 1000.0;
    _fovy   = 45.0;

    gluPerspective(_fovy, _aspect, _z_near, _z_far);

    // setting the model view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    _eye[0] = _eye[1] = 0.0; _eye[2] = 6.0;
    _center[0] = _center[1] = _center[2] = 0.0;
    _up[0] = _up[2] = 0.0; _up[1] = 1.0;

    gluLookAt(_eye[0], _eye[1], _eye[2], _center[0], _center[1], _center[2], _up[0], _up[1], _up[2]);

    // enabling the depth test
    glEnable(GL_DEPTH_TEST);

    // setting the background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // initial values of transformation parameters
    _angle_x = _angle_y = _angle_z = 0.0;
    _trans_x = _trans_y = _trans_z = 0.0;
    _zoom = 1.0;

    // ...

    try
    {
        // initializing the OpenGL Extension Wrangler library
        GLenum error = glewInit();

        if (error != GLEW_OK)
        {
            throw Exception("Could not initialize the OpenGL Extension Wrangler Library!");
        }

        if (!glewIsSupported("GL_VERSION_2_0"))
        {
            throw Exception("Your graphics card is not compatible with OpenGL 2.0+! "
                                "Try to update your driver or buy a new graphics adapter!");
        }

        // create and store your geometry in display lists or vertex buffer objects
        // ...

        texture[0] = new QOpenGLTexture(QImage("Textures/periodic_texture_05.jpg").mirrored());
        texture[0]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        texture[0]->setMagnificationFilter(QOpenGLTexture::Linear);

        texture[1] = new QOpenGLTexture(QImage("Textures/periodic_texture_01.jpg").mirrored());
        texture[1]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        texture[1]->setMagnificationFilter(QOpenGLTexture::Linear);

        texture[2] = new QOpenGLTexture(QImage("Textures/periodic_texture_03.jpg").mirrored());
        texture[2]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        texture[2]->setMagnificationFilter(QOpenGLTexture::Linear);

        texture[3] = new QOpenGLTexture(QImage("Textures/periodic_texture_07.jpg").mirrored());
        texture[3]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        texture[3]->setMagnificationFilter(QOpenGLTexture::Linear);

        initializeSecondTrigPatch();
        initializeShaders();

        addTrigCurve();
        emit update_alpha(PI / 2.0);

        HCoordinate3    direction(0.0, 0.0, 1.0, 0.0);
        Color4          ambient(0.4, 0.4, 0.4, 1.0);
        Color4          diffuse(0.8, 0.8, 0.8, 1.0);
        Color4          specular(1.0, 1.0, 1.0, 1.0);

        _dl = new (nothrow) DirectionalLight(GL_LIGHT0, direction, ambient, diffuse, specular);
        if (!_dl) {
            throw("Exception: Could Not Create The Directional Light!");
        }

        glEnable(GL_POINT_SMOOTH);
        glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

        glEnable(GL_POLYGON_SMOOTH);
        glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

        glEnable(GL_DEPTH_TEST);
    }
    catch (Exception &e)
    {
        cout << e << endl;
    }
}

void GLWidget::paintTrigonometricCurves() {
    int n = _image_of_trig_curve.GetColumnCount();
    for (int i = 1; i < n; i++) {
        glPointSize(5.0f);
        if (_parametric_first_derivatives) {
            glColor3f(0.0f, 0.5f, 0.0f);
            _image_of_trig_curve[i]->RenderDerivatives(1, GL_LINES);
            _image_of_trig_curve[i]->RenderDerivatives(1, GL_POINTS);
        }

        if (_parametric_second_derivatives) {
            glColor3f(1.0f, 0.5f, 0.9f);
            _image_of_trig_curve[i]->RenderDerivatives(2, GL_LINES);
            _image_of_trig_curve[i]->RenderDerivatives(2, GL_POINTS);
        }


        if(i==_selected_trig_curve) {
            if(_show_control_points) {

                glPointSize(10.0f);
                glColor3f(0.0f, 1.0f, 1.0f);
                _trig_curve[i]->RenderDataCustomized(GL_POINTS,true,_trig_point);
                _trig_curve[i]->RenderData();
            }
            glColor3f(1.0f, 0.0f, 0.0f);
            _image_of_trig_curve[i]->RenderDerivatives(0, GL_LINE_STRIP);
        } else {
            if(_show_control_points) {

                glPointSize(10.0f);
                glColor3f(0.0f, 1.0f, 1.0f);
                _trig_curve[i]->RenderDataCustomized(GL_POINTS,false,_trig_point);
                _trig_curve[i]->RenderData();
            }
            glColor3f(0.0f, 1.0f, 1.0f);
            _image_of_trig_curve[i]->RenderDerivatives(0, GL_LINE_STRIP);
        }



        glPointSize(1.0f);
    }
}

void GLWidget::paintTrigonometricCurveOperations() {
    int n = _image_of_trig_curve.GetColumnCount();

    glColor3f(1.0f, 0.0f, 0.0f);
    _image_of_trig_curve[_curve1]->RenderDerivatives(0, GL_LINE_STRIP);
    glPointSize(10.0f);
    glColor3f(0.0f, 1.0f, 1.0f);
    _trig_curve[_curve1]->RenderDataCustomized(GL_POINTS,true,_curve1_point);
    _trig_curve[_curve1]->RenderData();

    glColor3f(1.0f, 0.0f, 0.0f);
    _image_of_trig_curve[_curve2]->RenderDerivatives(0, GL_LINE_STRIP);
    glPointSize(10.0f);
    glColor3f(0.0f, 1.0f, 1.0f);
    _trig_curve[_curve2]->RenderDataCustomized(GL_POINTS,true,_curve2_point);
    _trig_curve[_curve2]->RenderData();

    for (int i = 1; i < n; i++) {
        glPointSize(5.0f);
        if (_parametric_first_derivatives) {
            glColor3f(0.0f, 0.5f, 0.0f);
            _image_of_trig_curve[i]->RenderDerivatives(1, GL_LINES);
            _image_of_trig_curve[i]->RenderDerivatives(1, GL_POINTS);
        }

        if (_parametric_second_derivatives) {
            glColor3f(1.0f, 0.5f, 0.9f);
            _image_of_trig_curve[i]->RenderDerivatives(2, GL_LINES);
            _image_of_trig_curve[i]->RenderDerivatives(2, GL_POINTS);
        }

        glColor3f(0.0f, 1.0f, 1.0f);
        _image_of_trig_curve[i]->RenderDerivatives(0, GL_LINE_STRIP);
        glPointSize(10.0f);
        glColor3f(0.0f, 1.0f, 1.0f);
        _trig_curve[i]->RenderDataCustomized(GL_POINTS,false,_trig_point);
        _trig_curve[i]->RenderData();

    }



    glPointSize(1.0f);
}



void GLWidget::paintSecondTrigPatch() {
    int n = _patch.GetColumnCount() - 1;
    if (_dl)
    {
        _dl->Enable();
        glEnable(GL_LIGHTING);
        glEnable(GL_NORMALIZE);
        glEnable(GL_TEXTURE_2D);

        enableShader();
        for (int i = 0; i < n; i++) {
            MatFBEmerald.Apply();
            _patch[_selected_patch]->GetImage()->Render();
            // @TODO Let every patch have it's own material
            _materials[_selected_material].Apply();
            // @TODO Let every patch have it's own texture
            if (_apply_texture) texture[_selected_texture]->bind();
            else texture[_selected_texture]->release();

            _patch[i]->GetImage()->Render();
        }
        disableShader();
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
        glDisable(GL_NORMALIZE);
        _dl->Disable();

        if(_show_control_net) {
            glPointSize(10.0f);
            glColor3f(0.0f, 1.0f, 1.0f);
            _patch[_selected_patch]->RenderDataCustomized(GL_POINTS,_patch_col,_patch_row);
            _patch[_selected_patch]->RenderData(GL_LINE_STRIP);
        }

        for (int i = 0; i < n; i++) {
            glColor3f(1.0f, 0.0f, 0.0f);
            if(_show_uip) {
                for (GLuint j = 0; j < _patch[i]->GetUip()->GetColumnCount(); j++) {
                    GenericCurve3 *curve = (*_patch[i]->GetUip())[j];
                    if (curve) {
                        curve->RenderDerivatives(0,  GL_LINE_STRIP);
                    }
                }
            }
            glColor3f(0.0f, 0.5f, 0.0f);
            if(_show_vip) {
                for (GLuint j = 0; j < _patch[i]->GetVip()->GetColumnCount(); j++) {
                    GenericCurve3 *curve = (*_patch[i]->GetVip())[j];
                    if (curve) {
                        curve->RenderDerivatives(0,  GL_LINE_STRIP);
                    }
                }
            }
        }
    }
}

void GLWidget::paintSecondTrigPatchOperations() {
    int n = _patch.GetColumnCount() - 1;
    if (_dl)
    {
        _dl->Enable();
        glEnable(GL_LIGHTING);
        glEnable(GL_NORMALIZE);
        glEnable(GL_TEXTURE_2D);

        enableShader();
        for (int i = 0; i < n; i++) {
            MatFBRuby.Apply();
            _patch[_patch_func_1]->GetImage()->Render();
            MatFBBrass.Apply();
            _patch[_patch_func_2]->GetImage()->Render();
            // @TODO Let every patch have it's own material
            //_materials[_selected_material].Apply();
            MatFBSilver.Apply();
            // @TODO Let every patch have it's own texture
            if (_apply_texture) texture[_selected_texture]->bind();
            else texture[_selected_texture]->release();

            _patch[i]->GetImage()->Render();
        }
        disableShader();
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
        glDisable(GL_NORMALIZE);
        _dl->Disable();

        if(_show_control_net) {
            glPointSize(10.0f);
            glColor3f(0.0f, 1.0f, 1.0f);
            _patch[_patch_func_1]->RenderDataSides(GL_POINTS,_patch_side_1);
            _patch[_patch_func_1]->RenderDataSides(GL_LINE_STRIP,_patch_side_1);
            _patch[_patch_func_2]->RenderDataSides(GL_POINTS,_patch_side_2);
            _patch[_patch_func_2]->RenderDataSides(GL_LINE_STRIP,_patch_side_2);
        }

        for (int i = 0; i < n; i++) {
            glColor3f(1.0f, 0.0f, 0.0f);
            if(_show_uip) {
                for (GLuint j = 0; j < _patch[i]->GetUip()->GetColumnCount(); j++) {
                    GenericCurve3 *curve = (*_patch[i]->GetUip())[j];
                    if (curve) {
                        curve->RenderDerivatives(0,  GL_LINE_STRIP);
                    }
                }
            }
            glColor3f(0.0f, 0.5f, 0.0f);
            if(_show_vip) {
                for (GLuint j = 0; j < _patch[i]->GetVip()->GetColumnCount(); j++) {
                    GenericCurve3 *curve = (*_patch[i]->GetVip())[j];
                    if (curve) {
                        curve->RenderDerivatives(0,  GL_LINE_STRIP);
                    }
                }
            }
        }
    }
}

void GLWidget::enableShader() {
    _shaders[_selected_shader].Enable();
}
void GLWidget::disableShader() {
    _shaders[_selected_shader].Disable();
}

//-----------------------
// the rendering function
//-----------------------
void GLWidget::paintGL()
{
    // clears the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // stores/duplicates the original model view matrix
    glPushMatrix();

    // applying transformations
    glRotatef(_angle_x, 1.0, 0.0, 0.0);
    glRotatef(_angle_y, 0.0, 1.0, 0.0);
    glRotatef(_angle_z, 0.0, 0.0, 1.0);
    glTranslated(_trans_x, _trans_y, _trans_z);
    glScaled(_zoom, _zoom, _zoom);

    // render your geometry (this is oldest OpenGL rendering technique, later we will use some advanced methods)

    switch(_current_tab) {
    case 0: {
        paintTrigonometricCurves();
        break;
    }
    case 1: {
        paintTrigonometricCurveOperations();
        break;
    }
    case 2: {
        paintSecondTrigPatch();
        break;
    }
    default: {
        paintSecondTrigPatchOperations();
        break;
    }
    }

    // pops the current matrix stack, replacing the current matrix with the one below it on the stack,
    // i.e., the original model view matrix is restored
    glPopMatrix();
}

//----------------------------------------------------------------------------
// when the main window is resized one needs to redefine the projection matrix
//----------------------------------------------------------------------------
void GLWidget::resizeGL(int w, int h)
{
    // setting the new size of the rendering context
    glViewport(0, 0, w, h);

    // redefining the projection matrix
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    _aspect = static_cast<double>(w) / static_cast<double>(h);

    gluPerspective(_fovy, _aspect, _z_near, _z_far);

    // switching back to the model view matrix
    glMatrixMode(GL_MODELVIEW);

    update();
}

void GLWidget::addTrigCurve() {

    int n = _trig_curve.GetColumnCount();
    if (!_trig_curve.ResizeColumns(n+1)) {
        //@TODO Error
        return;
    }

    //@TODO add a curve to the UI combo box
    _trig_curve[n] = new (nothrow) SecondOrderTrigArc();

    if(!_trig_curve[n])
    {
        cout << "Couldn't generate CyclicCurve" << endl;
        return;
    }

    SecondOrderTrigArc& curve= *_trig_curve[n];

    DCoordinate3 &cp = curve[0];
    DCoordinate3 &cp1 = curve[1];
    DCoordinate3 &cp2 = curve[2];
    DCoordinate3 &cp3 = curve[3];

    cp[0] = -2.0; //x
    cp[1] = -1.0; //y
    cp[2] = 0.0;  //z

    cp1[0] = -1.0; //x
    cp1[1] = 1.0; //y
    cp1[2] = 0.0;  //z

    cp2[0] = 1.0; //x
    cp2[1] = 1.0; //y
    cp2[2] = 0.0;  //z

    cp3[0] = 2.0; //x
    cp3[1] = -1.0; //y
    cp3[2] = 0.0;  //z

    if(!_trig_curve[n]->UpdateVertexBufferObjectsOfData(GL_STATIC_DRAW))
    {
        cout << "Could_not_create_the_vertex_buffer_object_of_the_cyclic_curve!" << endl;
    }

    _image_of_trig_curve.ResizeColumns(n+1);
    _image_of_trig_curve[n] = _trig_curve[n]->GenerateImage(2, 100, GL_STATIC_DRAW);

    if(!_image_of_trig_curve[n])
    {
        cout << "Couldn't generate Second Order Trig Curve" << endl;
        return;
    }

    if(!_image_of_trig_curve[n]->UpdateVertexBufferObjects(_derivatives_scale, GL_STATIC_DRAW))
    {
        cout << "Could_not_create_the_vertex_buffer_object_of_the_cyclic_curve!" << endl;
    }

    emit add_curve_to_list(n);
    update();
}

void GLWidget::add_trig_curve() {
    addTrigCurve();
}

//-----------------------------------
// implementation of the public slots
//-----------------------------------

void GLWidget::set_angle_x(int value)
{
    if (_angle_x != value)
    {
        _angle_x = value;
        update();
    }
}

void GLWidget::set_angle_y(int value)
{
    if (_angle_y != value)
    {
        _angle_y = value;
        update();
    }
}

void GLWidget::set_angle_z(int value)
{
    if (_angle_z != value)
    {
        _angle_z = value;
        update();
    }
}

void GLWidget::set_zoom_factor(double value)
{
    if (_zoom != value)
    {
        _zoom = value;
        update();
    }
}

void GLWidget::set_trans_x(double value)
{
    if (_trans_x != value)
    {
        _trans_x = value;
        update();
    }
}

void GLWidget::set_trans_y(double value)
{
    if (_trans_y != value)
    {
        _trans_y = value;
        update();
    }
}

void GLWidget::set_trans_z(double value)
{
    if (_trans_z != value)
    {
        _trans_z = value;
        update();
    }
}


void GLWidget::set_parametric_first_derivatives(bool value) {
    if (_parametric_first_derivatives != value) {
        _parametric_first_derivatives = value;
        update();
    }
}

void GLWidget::set_parametric_second_derivatives(bool value) {
    if (_parametric_second_derivatives != value) {
        _parametric_second_derivatives = value;
        update();
    }
}

void GLWidget::set_derivatives_scale(int value) {
    if (_derivatives_scale == value) return;
    _derivatives_scale = value;
    for (GLuint i = 1; i < _image_of_trig_curve.GetColumnCount(); i++) {
        _image_of_trig_curve[i]->DeleteVertexBufferObjects();
        _image_of_trig_curve[i]->UpdateVertexBufferObjects(_derivatives_scale / 100, GL_STATIC_DRAW);
    }
    update();
}

void GLWidget::show_patch_uip(bool value) {
    if(_show_uip == value) return;
    _show_uip = value;
    if (!_show_uip) {update(); return;}
    for (GLuint i = 0; i < _patch.GetColumnCount()-1; i++) {
        _patch[i]->SetUip(_patch[i]->GenerateUIsoparametricLines(30, 1, 100));
    }

    update();
}

void GLWidget::show_patch_vip(bool value) {
    if(_show_vip == value) return;
    _show_vip = value;
    if (!_show_vip) {update(); return;}

    for (GLuint i = 0; i < _patch.GetColumnCount()-1; i++) {
        _patch[i]->SetVip(_patch[i]->GenerateVIsoparametricLines(30, 1, 100));
    }

    update();
}

void GLWidget::set_trig_point(int value) {
    if (_trig_point != value) {
        _trig_point = value;
        SecondOrderTrigArc& curve= *_trig_curve[_selected_trig_curve];
        DCoordinate3 &cp = curve[_trig_point];
        emit update_trig_point_x(cp[0]);
        emit update_trig_point_y(cp[1]);
        emit update_trig_point_z(cp[2]);

        update();
    }
}
void GLWidget::set_trig_point_x(double value) {
    SecondOrderTrigArc& curve= *_trig_curve[_selected_trig_curve];
    DCoordinate3 &cp = curve[_trig_point];

    if (cp[0] == value) return;
    double difference = cp[0]-value;
    cp[0] = value;
    SecondOrderTrigArc* joinedCurve =  _trig_curve[_selected_trig_curve]->preserveContinuity(_trig_point,0,difference);

    if (joinedCurve != NULL){
        for(int k=1; k<=_trig_curve.GetColumnCount() ;k++){
            if (joinedCurve == _trig_curve[k]){
                regenerate_curve(k);
                break;
            }
        }
    }
    regenerate_curve(_selected_trig_curve);

    update();
}
void GLWidget::set_trig_point_y(double value) {
    SecondOrderTrigArc& curve= *_trig_curve[_selected_trig_curve];
    DCoordinate3 &cp = curve[_trig_point];

    if (cp[1] == value) return;
    double difference = cp[1]-value;
    cp[1] = value;

    SecondOrderTrigArc* joinedCurve =  _trig_curve[_selected_trig_curve]->preserveContinuity(_trig_point,1,difference);

    if (joinedCurve != NULL){
        for(int k=1; k<=_trig_curve.GetColumnCount() ;k++){
            if (joinedCurve == _trig_curve[k]){
                regenerate_curve(k);
                break;
            }
        }
    }

    regenerate_curve(_selected_trig_curve);

    update();
}
void GLWidget::set_trig_point_z(double value) {
    SecondOrderTrigArc& curve= *_trig_curve[_selected_trig_curve];
    DCoordinate3 &cp = curve[_trig_point];

    if (cp[2] == value) return;
    double difference = cp[2]-value;
    cp[2] = value;
    SecondOrderTrigArc* joinedCurve =  _trig_curve[_selected_trig_curve]->preserveContinuity(_trig_point,2,difference);

    if (joinedCurve != NULL){
        for(int k=1; k<=_trig_curve.GetColumnCount() ;k++){
            if (joinedCurve == _trig_curve[k]){
                regenerate_curve(k);
                break;
            }
        }
    }

    regenerate_curve(_selected_trig_curve);
    update();
}

void GLWidget::set_tab(int value) {
    _current_tab = value;
    update();
}

void GLWidget::set_apply_texture(bool value) {
    if (value != _apply_texture) {
        _apply_texture = value;
        update();
    }
}

void GLWidget::set_selected_material(int value) {
    if (value != _selected_material) {
        _selected_material = value;
        update();
    }
}

void GLWidget::set_selected_texture(int value) {
    if(_selected_texture != value) {
        if(_apply_texture) texture[_selected_texture]->release();
        _selected_texture = value;
        update();
    }
}

void GLWidget::set_alpha(double value) {
    if (_trig_curve[_selected_trig_curve]->GetAlpha() == value) {
        return;
    }
    _trig_curve[_selected_trig_curve]->DeleteVertexBufferObjectsOfData();
    _trig_curve[_selected_trig_curve]->SetAlpha(value);
    if(!_trig_curve[_selected_trig_curve]->UpdateVertexBufferObjectsOfData(GL_STATIC_DRAW))
    {
        cout << "Could_not_create_the_vertex_buffer_object_of_the_cyclic_curve!" << endl;
    }
    _image_of_trig_curve[_selected_trig_curve]->DeleteVertexBufferObjects();
    _image_of_trig_curve[_selected_trig_curve] = _trig_curve[_selected_trig_curve]->GenerateImage(2, 100, GL_STATIC_DRAW);

    if(!_image_of_trig_curve[_selected_trig_curve])
    {
        cout << "Couldn't generate Second Order Trig Curve" << endl;
        return;
    }

    if(!_image_of_trig_curve[_selected_trig_curve]->UpdateVertexBufferObjects(_derivatives_scale, GL_STATIC_DRAW))
    {
        cout << "Could_not_create_the_vertex_buffer_object_of_the_cyclic_curve!" << endl;
    }

    update();
}

void GLWidget::set_patch_alpha_u(double value) {
    if (value == _patch[_selected_patch]->GetUAlpha()) return;
    _patch[_selected_patch]->SetUAlpha(value);

    _patch[_selected_patch]->DeleteVertexBufferObjectsOfData();

    _patch[_selected_patch]->UpdateVertexBufferObjectsOfData();
    _patch[_selected_patch]->SetImage(_patch[_selected_patch]->GenerateImage(30, 30, GL_STATIC_DRAW));
    update();
}

void GLWidget::set_patch_alpha_v(double value) {
    if (value == _patch[_selected_patch]->GetVAlpha()) return;
    _patch[_selected_patch]->SetVAlpha(value);

    _patch[_selected_patch]->DeleteVertexBufferObjectsOfData();

    _patch[_selected_patch]->UpdateVertexBufferObjectsOfData();
    _patch[_selected_patch]->SetImage(_patch[_selected_patch]->GenerateImage(30, 30, GL_STATIC_DRAW));
    update();
}
void GLWidget::set_patch_row(int value){
    if (value == _patch_row) return;
    _patch_row = value;

    DCoordinate3 p;
    _patch[_selected_patch]->GetData(_patch_row, _patch_col, p);

    emit update_patch_point_x(p.x());
    emit update_patch_point_y(p.y());
    emit update_patch_point_z(p.z());
    update();
}
void GLWidget::set_patch_col(int value){
    if (value == _patch_col) return;
    _patch_col = value;

    DCoordinate3 p;
    _patch[_selected_patch]->GetData(_patch_row, _patch_col, p);

    emit update_patch_point_x(p.x());
    emit update_patch_point_y(p.y());
    emit update_patch_point_z(p.z());
    update();
}

void GLWidget::set_patch_point_x(double value){
    DCoordinate3 p;
    _patch[_selected_patch]->GetData(_patch_row, _patch_col, p);

    if (p.x() == value) return;
    p.x() = value;
    _patch[_selected_patch]->SetData(_patch_row, _patch_col, value, p.y(), p.z());;
    _patch[_selected_patch]->PreserveContinuity(_patch_row, _patch_col, p);
    update();
}
void GLWidget::set_patch_point_y(double value){
    DCoordinate3 p;
    _patch[_selected_patch]->GetData(_patch_row, _patch_col, p);

    if (p.y() == value) return;
    p.y() = value;
    _patch[_selected_patch]->SetData(_patch_row, _patch_col, p.x(), value, p.z());
    _patch[_selected_patch]->PreserveContinuity(_patch_row, _patch_col, p);
    update();
}
void GLWidget::set_patch_point_z(double value){
    DCoordinate3 p;
    _patch[_selected_patch]->GetData(_patch_row, _patch_col, p);

    if (p.z() == value) return;
    p.z() = value;
    _patch[_selected_patch]->SetData(_patch_row, _patch_col, p.x(), p.y(), value);
    _patch[_selected_patch]->PreserveContinuity(_patch_row, _patch_col, p);
    update();
}

void GLWidget::set_show_control_points(bool value) {
    if (_show_control_points == value) return;
    _show_control_points = value;
    update();
}

void GLWidget::set_show_control_net(bool value) {
    if (_show_control_net == value) return;
    _show_control_net = value;
    update();
}

void GLWidget::set_selected_shader(int value) {
    if (value == _selected_shader) return;
    disableShader();
    _selected_shader = value;
    update();
}

void GLWidget::set_selected_patch(int value) {
    if (_selected_patch == value) return;
    _selected_patch = value;

    DCoordinate3 p;
    _patch[_selected_patch]->GetData(_patch_row, _patch_col, p);

    emit update_patch_point_x(p.x());
    emit update_patch_point_y(p.y());
    emit update_patch_point_z(p.z());

    emit update_alpha_u(_patch[_selected_patch]->GetUAlpha());
    emit update_alpha_v(_patch[_selected_patch]->GetVAlpha());

    emit update_translated_x(_patch[_selected_patch]->GetTranslatedX());
    emit update_translated_y(_patch[_selected_patch]->GetTranslatedY());
    emit update_translated_z(_patch[_selected_patch]->GetTranslatedZ());
    update();
}

void GLWidget::set_selected_curve(int value) {
    value++;
    if(_selected_trig_curve != value) {
        _selected_trig_curve = value;
        SecondOrderTrigArc& curve= *_trig_curve[_selected_trig_curve];
        DCoordinate3 &cp = curve[_trig_point];
        emit update_trig_point_x(cp[0]);
        emit update_trig_point_y(cp[1]);
        emit update_trig_point_z(cp[2]);
        emit update_alpha(curve.GetAlpha());
        update();
    }
}

void GLWidget::set_curve1(int value) {
    value++;
    if(_curve1 != value) {
        _curve1 = value;
        update();
    }
}

void GLWidget::set_curve2(int value) {
    value++;
    if(_curve2 != value) {
        _curve2 = value;
        update();
    }

}

void GLWidget::set_curve1_point(int value) {
    if(_curve1_point != value) {
        _curve1_point = value * 3;
        update();
    }
}

void GLWidget::set_curve2_point(int value) {
    if(_curve2_point != value) {
        _curve2_point = value * 3;
        update();
    }
}

void GLWidget::regenerate_curve(int n){

    _trig_curve[n]->DeleteVertexBufferObjectsOfData();
    if(!_trig_curve[n]->UpdateVertexBufferObjectsOfData(GL_STATIC_DRAW))
    {
        cout << "Could_not_create_the_vertex_buffer_object_of_the_cyclic_curve!" << endl;
    }


    if(_image_of_trig_curve[n]) _image_of_trig_curve[n]->DeleteVertexBufferObjects();
    _image_of_trig_curve[n] = _trig_curve[n]->GenerateImage(2, 100, GL_STATIC_DRAW);

    if(!_image_of_trig_curve[n])
    {
        cout << "Couldn't generate Second Order Trig Curve" << endl;
        return;
    }

    if(!_image_of_trig_curve[n]->UpdateVertexBufferObjects(_derivatives_scale, GL_STATIC_DRAW))
    {
        cout << "Could_not_create_the_vertex_buffer_object_of_the_cyclic_curve!" << endl;
    }
}

void GLWidget::curve_extend() {
    int n = _trig_curve.GetColumnCount();

    if (!_trig_curve.ResizeColumns(n+1)) {
        //@TODO Error
        return;
    }
    emit add_curve_to_list(n);

    if (_curve1_point == 0){
        _trig_curve[n] = _trig_curve[_curve1]->extendLeft();
    }
    else{
        _trig_curve[n] = _trig_curve[_curve1]->extendRight();
    }

    _image_of_trig_curve.ResizeColumns(n+1);
    regenerate_curve(n);

    update();
}

void GLWidget::curve_merge() {

    if (_curve1_point == 0){
        if( _curve2_point == 0){
            _trig_curve[_curve1]->mergeLeftLeft(_trig_curve[_curve2]);
        }
        else{
            _trig_curve[_curve1]->mergeLeftRight(_trig_curve[_curve2]);
        }
    }
    else{
        if( _curve2_point == 0){
            _trig_curve[_curve1]->mergeRightLeft(_trig_curve[_curve2]);
        }
        else{
            _trig_curve[_curve1]->mergeRightRight(_trig_curve[_curve2]);
        }
    }

    regenerate_curve(_curve1);
    regenerate_curve(_curve2);

    _trig_curve[_curve2]->PreserveAlpha(_trig_curve[_curve1]->GetAlpha());

    update();
}

void GLWidget::curve_join() {
    int n = _trig_curve.GetColumnCount();

    if (!_trig_curve.ResizeColumns(n+1)) {
        //@TODO Error
        return;
    }
    emit add_curve_to_list(n);

    if (_curve1_point == 0){
        if( _curve2_point == 0){
            _trig_curve[n] = _trig_curve[_curve1]->joinLeftLeft(_trig_curve[_curve2]);
        }
        else{
            _trig_curve[n] = _trig_curve[_curve1]->joinLeftRight(_trig_curve[_curve2]);
        }
    }
    else{
        if( _curve2_point == 0){
            _trig_curve[n] = _trig_curve[_curve1]->joinRightLeft(_trig_curve[_curve2]);
        }
        else{
            _trig_curve[n] = _trig_curve[_curve1]->joinRightRight(_trig_curve[_curve2]);
        }
    }

    _image_of_trig_curve.ResizeColumns(n+1);
    regenerate_curve(n);

    _trig_curve[_curve2]->PreserveAlpha(_trig_curve[_curve1]->GetAlpha());

    update();
}

// Patches

void GLWidget::patch_translate_x(double value) {
    //@TODO Preserve Continuity
    if (_patch[_selected_patch]->GetTranslatedX() == value) return;
    _patch[_selected_patch]->translateX(value);
    _patch[_selected_patch]->SetImage(_patch[_selected_patch]->GenerateImage(30, 30, GL_STATIC_DRAW));

    DCoordinate3 p;
    _patch[_selected_patch]->GetData(_patch_row, _patch_col, p);

    emit update_patch_point_x(p.x());
    emit update_patch_point_y(p.y());
    emit update_patch_point_z(p.z());

    emit update_alpha_u(_patch[_selected_patch]->GetUAlpha());
    emit update_alpha_v(_patch[_selected_patch]->GetVAlpha());

    update();
}

void GLWidget::patch_translate_y(double value) {
    //@TODO Preserve Continuity
    if (_patch[_selected_patch]->GetTranslatedY() == value) return;
    _patch[_selected_patch]->translateY(value);
    _patch[_selected_patch]->SetImage(_patch[_selected_patch]->GenerateImage(30, 30, GL_STATIC_DRAW));

    DCoordinate3 p;
    _patch[_selected_patch]->GetData(_patch_row, _patch_col, p);

    emit update_patch_point_x(p.x());
    emit update_patch_point_y(p.y());
    emit update_patch_point_z(p.z());

    emit update_alpha_u(_patch[_selected_patch]->GetUAlpha());
    emit update_alpha_v(_patch[_selected_patch]->GetVAlpha());

    update();
}

void GLWidget::patch_translate_z(double value) {
    //@TODO Preserve Continuity
    if (_patch[_selected_patch]->GetTranslatedZ() == value) return;
    _patch[_selected_patch]->translateZ(value);
    _patch[_selected_patch]->SetImage(_patch[_selected_patch]->GenerateImage(30, 30, GL_STATIC_DRAW));

    DCoordinate3 p;
    _patch[_selected_patch]->GetData(_patch_row, _patch_col, p);

    emit update_patch_point_x(p.x());
    emit update_patch_point_y(p.y());
    emit update_patch_point_z(p.z());

    emit update_alpha_u(_patch[_selected_patch]->GetUAlpha());
    emit update_alpha_v(_patch[_selected_patch]->GetVAlpha());

    update();
}

void GLWidget::set_patch_func_1(int value) {
    _patch_func_1 = value;
    update();
}
void GLWidget::set_patch_func_2(int value) {
    _patch_func_2 = value;
    update();
}

void GLWidget::set_patch_side_1(int value) {
    _patch_side_1 = value;
    update();
}
void GLWidget::set_patch_side_2(int value) {
    _patch_side_2 = value;
    update();
}

void GLWidget::patch_extend() {
    int n = _patch.GetColumnCount();
    _patch.ResizeColumns(n+1);
    n = n-1;
    _patch[n] = new (nothrow) SecTrigPatch3();
    if(!_patch[_patch_func_1]->ExtendPatch(_patch_extend_direction, *_patch[n])) {
        _patch.ResizeColumns(n+1);
        return;
    }
    _patch[n]->SetImage(_patch[n]->GenerateImage(30, 30, GL_STATIC_DRAW));

    emit add_patch_to_list(n);

    update();
}

void GLWidget::set_extend_direction(int value) {

    _patch_extend_direction = value;
}

void GLWidget::patch_join() {
    if(_patch_func_1 == _patch_func_2) return;
    int n = _patch.GetColumnCount();
    _patch.ResizeColumns(n+1);
    n = n-1;
    _patch[n] = new (nothrow) SecTrigPatch3();
    if(!_patch[_patch_func_1]->JoinPatch(*_patch[_patch_func_2], _patch_side_1 * 2, _patch_side_2 *2, *_patch[n])) {
        _patch.ResizeColumns(n+1);
        return;
    }
    _patch[n]->SetImage(_patch[n]->GenerateImage(30, 30, GL_STATIC_DRAW));

    emit add_patch_to_list(n);
    update();
}

void GLWidget::patch_merge() {
    if(_patch_func_1 == _patch_func_2) return;
    if(!_patch[_patch_func_1]->MergePatch(*_patch[_patch_func_2], _patch_side_1 * 2, _patch_side_2 *2)) {
        return;
    }

    _patch[_patch_func_1]->SetImage(_patch[_patch_func_1]->GenerateImage(30, 30, GL_STATIC_DRAW));
    _patch[_patch_func_2]->SetImage(_patch[_patch_func_2]->GenerateImage(30, 30, GL_STATIC_DRAW));
    update();
}

void GLWidget::set_smoothing(double value) {
    _shaders[3].Enable();
    _smoothing_parameter = value;
    _shaders[3].SetUniformVariable1f("smoothing", _smoothing_parameter);
    _shaders[3].Disable();
    update();
}
void GLWidget::set_scaling(double value) {
    _shaders[3].Enable();
    _scale_parameter = value;
    _shaders[3].SetUniformVariable1f("scale_factor", _scale_parameter);
    _shaders[3].Disable();
    update();
}
void GLWidget::set_shading(double value) {
    _shaders[3].Enable();
    _shading_parameter = value;
    _shaders[3].SetUniformVariable1f("shading", _shading_parameter);
    _shaders[3].Disable();
    update();
}

}
