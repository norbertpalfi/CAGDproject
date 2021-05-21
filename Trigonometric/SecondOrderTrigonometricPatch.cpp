#include "SecondOrderTrigonometricPatch.h"

using namespace cagd;

GLdouble SecTrigPatch3::f0(GLdouble alpha, GLdouble t) const
{
    return f3(alpha, alpha - t);
}

GLdouble SecTrigPatch3::f1(GLdouble alpha, GLdouble t) const
{
    return f2(alpha, alpha - t);
}

GLdouble SecTrigPatch3::f2(GLdouble alpha, GLdouble t) const
{
    GLdouble sin4Alpha = sin(alpha / 2) * sin(alpha / 2) * sin(alpha / 2) * sin(alpha / 2);
    GLdouble sin3T = sin(t / 2) * sin(t / 2) * sin(t / 2);

    return ((4 * cos(alpha / 2)) / sin4Alpha) * sin((alpha - t) / 2) * sin3T +
            ((1 + 2 * cos(alpha / 2) * cos(alpha / 2)) / sin4Alpha)
            * (sin((alpha - t) / 2) * sin((alpha - t) / 2))
            * (sin(t / 2) * sin(t / 2));
}

GLdouble SecTrigPatch3::f3(GLdouble alpha, GLdouble t) const
{
    GLdouble sin4Alpha = sin(alpha / 2) * sin(alpha / 2) * sin(alpha / 2) * sin(alpha / 2);
    GLdouble sin4T = sin(t / 2) * sin(t / 2) * sin(t / 2) * sin(t / 2);

    return (1 / (sin4Alpha)) * (sin4T);
}

GLdouble SecTrigPatch3::f0_1(GLdouble alpha, GLdouble t) const
{
    return -f3_1(alpha, alpha - t);
}

GLdouble SecTrigPatch3::f1_1(GLdouble alpha, GLdouble t) const
{
    return -f2_1(alpha, alpha - t);
}

GLdouble SecTrigPatch3::f2_1(GLdouble alpha, GLdouble t) const
{
    GLdouble sin4Alpha = sin(alpha / 2.0) * sin(alpha / 2.0) * sin(alpha / 2.0) * sin(alpha / 2.0);
    return -(sin(t/2.0)*(2.0*cos(alpha/2.0)*cos((alpha-t)/2.0)*sin(t/2.0)*sin(t/2.0)+((2.0*cos(alpha/2.0)*cos(alpha/2.0)+1.0)*cos((alpha-t)/2.0)*sin((alpha-t)/2.0)-6*cos(alpha/2.0)*sin((alpha-t)/2.0)*cos(t/2.0))*sin(t/2.0)+(-2.0*cos(alpha/2.0)*cos(alpha/2.0)-1.0)*sin((alpha-t)/2.0)*sin((alpha-t)/2.0)*cos(t/2.0)))/sin4Alpha;
}

GLdouble SecTrigPatch3::f3_1(GLdouble alpha, GLdouble t) const
{
    GLdouble sin4Alpha = sin(alpha / 2.0) * sin(alpha / 2.0) * sin(alpha / 2.0) * sin(alpha / 2.0);
    GLdouble sin3T = sin(t / 2.0) * sin(t / 2.0) * sin(t / 2.0);
    return (1.0 / (sin4Alpha)) * 2.0 * (sin3T) * cos(t / 2.0);
}

GLdouble SecTrigPatch3::f0_2(GLdouble alpha, GLdouble t) const
{
    return f3_2(alpha, alpha - t);
}

GLdouble SecTrigPatch3::f1_2(GLdouble alpha, GLdouble t) const
{
    return f2_2(alpha, alpha - t);
}

GLdouble SecTrigPatch3::f2_2(GLdouble alpha, GLdouble t) const
{
    GLdouble sin4Alpha = sin(alpha / 2.0) * sin(alpha / 2.0) * sin(alpha / 2.0) * sin(alpha / 2.0);
    GLdouble sin3T = sin(t/2.0) * sin(t/2.0) * sin(t/2.0);

    return -(8.0*cos(alpha/2.0)*sin((alpha-t)/2.0)*sin3T+(12.0*cos(alpha/2.0)*cos((alpha-t)/2.0)*cos(t/2.0)+(4.0*cos(alpha/2.0)*cos(alpha/2.0)+2.0)*sin((alpha-t)/2.0)*sin((alpha-t)/2.0)+(-2*cos(alpha/2.0)*cos(alpha/2.0)-1.0)*cos((alpha-t)/2.0)*cos((alpha-t)/2.0))*sin(t/2.0)*sin(t/2.0)+((8.0*cos(alpha/2.0)*cos(alpha/2.0)+4.0)*cos((alpha-t)/2.0)*sin((alpha-t)/2.0)*cos(t/2.0)-12.0*cos(alpha/2.0)*sin((alpha-t)/2.0)*cos(t/2.0)*cos(t/2.0))*sin(t/2.0)+(-2.0*cos(alpha/2.0)*cos(alpha/2.0)-1.0)*sin((alpha-t)/2.0)*sin((alpha-t)/2.0)*cos(t/2.0)*cos(t/2.0))/(2.0*sin4Alpha);
}

GLdouble SecTrigPatch3::f3_2(GLdouble alpha, GLdouble t) const
{
    GLdouble sin4Alpha = sin(alpha / 2.0) * sin(alpha / 2.0) * sin(alpha / 2.0) * sin(alpha / 2.0);
    GLdouble sin3T = sin(t / 2.0) * sin(t / 2.0) * sin(t / 2.0);
    return (1.0 / (sin4Alpha)) * 3.0 * (sin(t / 2.0) * sin(t / 2.0)) * cos(t / 2.0) * cos(t / 2.0) + (1.0 / (sin4Alpha)) * (sin3T) * -sin(t / 2.0);
}

SecTrigPatch3::SecTrigPatch3(GLdouble u, GLdouble v):TensorProductSurface3(0.0, u, 0.0, v){
    _alpha[0] = u; _alpha[1] = v; _image = nullptr;
    _translatedX = 0.0;
    _translatedY = 0.0;
    _translatedZ = 0.0;

    _has_neighbours = false;

    _uip = nullptr;
    _vip = nullptr;

    _updated = false;

    for (int i = 0; i < 8; i++) {
        _neighbours[i] = nullptr;
        _connected_side[i] = -1;
    }
}

GLboolean SecTrigPatch3::UBlendingFunctionValues(
        GLdouble u_knot, RowMatrix<GLdouble>& blending_values) const
{
    blending_values.ResizeColumns(5);

    blending_values[0] = f0(_alpha[0], u_knot);
    blending_values[1] = f1(_alpha[0], u_knot);
    blending_values[2] = f2(_alpha[0], u_knot);
    blending_values[3] = f3(_alpha[0], u_knot);

    return GL_TRUE;
}

GLboolean SecTrigPatch3::VBlendingFunctionValues(
        GLdouble v_knot, RowMatrix<GLdouble>& blending_values) const
{
    blending_values.ResizeColumns(5);

    blending_values[0] = f0(_alpha[1], v_knot);
    blending_values[1] = f1(_alpha[1], v_knot);
    blending_values[2] = f2(_alpha[1], v_knot);
    blending_values[3] = f3(_alpha[1], v_knot);

    return GL_TRUE;
}

GLboolean SecTrigPatch3::CalculatePartialDerivatives(
        GLuint maximum_order_of_partial_derivatives,
        GLdouble u, GLdouble v, PartialDerivatives& pd) const
{
    if (u < 0.0 || u > _alpha[0] || v < 0.0 || v > _alpha[1] || maximum_order_of_partial_derivatives > 1)
        return GL_FALSE;

    RowMatrix<GLdouble> u_blending_values(4), d1_u_blending_values(4);
    RowMatrix<GLdouble> v_blending_values(4), d1_v_blending_values(4);

    UBlendingFunctionValues(u, u_blending_values);
    VBlendingFunctionValues(v, v_blending_values);

    d1_u_blending_values(0) = f0_1(_alpha[0], u);
    d1_u_blending_values(1) = f1_1(_alpha[0], u);
    d1_u_blending_values(2) = f2_1(_alpha[0], u);
    d1_u_blending_values(3) = f3_1(_alpha[0], u);

    d1_v_blending_values(0) = f0_1(_alpha[1], v);
    d1_v_blending_values(1) = f1_1(_alpha[1], v);
    d1_v_blending_values(2) = f2_1(_alpha[1], v);
    d1_v_blending_values(3) = f3_1(_alpha[1], v);

    pd.ResizeRows(2);
    pd.LoadNullVectors();

    for(GLuint row = 0; row < 4; ++row)
    {
        DCoordinate3 aux_d0_v, aux_d1_v;
        for (GLuint column = 0; column < 4; ++column)
        {
            aux_d0_v += _data(row,column) * v_blending_values(column);
            aux_d1_v += _data(row, column) * d1_v_blending_values(column);
        }
        pd(0,0) += aux_d0_v * u_blending_values(row);
        pd(1,0) += aux_d0_v * d1_u_blending_values(row);
        pd(1,1) += aux_d1_v * u_blending_values(row);
    }

    return GL_TRUE;
}

GLboolean SecTrigPatch3::SetUAlpha(GLdouble alpha) {
    _alpha[0] = alpha;
    _u_max = alpha;
    return true;
}
GLdouble SecTrigPatch3::GetUAlpha() {
    return _alpha[0];
}

GLboolean SecTrigPatch3::SetVAlpha(GLdouble alpha) {
    _alpha[1] = alpha;
    _v_max = alpha;
    return true;
}
GLdouble SecTrigPatch3::GetVAlpha() {
    return _alpha[1];
}

// @TODO If the patch has neighbours, then... IDk...
// Option 1: Don't allow to translate
// Option 2: Figure the patch moving out
GLboolean SecTrigPatch3::translateX(double value) {
    if (_translatedX == value) return false;
    double diff = value - _translatedX;
    for(GLuint i = 0; i < _data.GetRowCount(); i++) {
        for(GLuint j = 0; j < _data.GetColumnCount(); j++) {
            DCoordinate3 point;
            GetData(i, j, point);
            SetData(i, j, point.x() + diff, point.y(), point.z());
        }
    }
    if(_has_neighbours==true) {
        for(GLuint i = 0; i < _data.GetRowCount(); i++) {
            DCoordinate3 point;
            GetData(i, 0, point);
            PreserveContinuity(i, 0, point);

            GetData(i, 3, point);
            PreserveContinuity(i, 3, point);
        }
        for(GLuint i = 1; i < _data.GetRowCount()-1; i++) {
            DCoordinate3 point;
            GetData(0, i, point);
            PreserveContinuity(0, i, point);

            GetData(3, i, point);
            PreserveContinuity(3, i, point);
        }
    }
    _translatedX = value;
    DeleteVertexBufferObjectsOfData();
    UpdateVertexBufferObjectsOfData();
    return true;
}

// @TODO If the patch has neighbours, then... IDk...
// Option 1: Don't allow to translate
// Option 2: Figure the patch moving out
GLboolean SecTrigPatch3::translateY(double value) {
    if (_translatedY == value) return false;
    double diff = value - _translatedY;
    for(GLuint i = 0; i < _data.GetRowCount(); i++) {
        for(GLuint j = 0; j < _data.GetColumnCount(); j++) {
            DCoordinate3 point;
            GetData(i, j, point);
            SetData(i, j, point.x(), point.y() + diff, point.z());
        }
    }
    if(_has_neighbours==true) {
        for(GLuint i = 0; i < _data.GetRowCount(); i++) {
            DCoordinate3 point;
            GetData(i, 0, point);
            PreserveContinuity(i, 0, point);

            GetData(i, 3, point);
            PreserveContinuity(i, 3, point);
        }
        for(GLuint i = 1; i < _data.GetRowCount()-1; i++) {
            DCoordinate3 point;
            GetData(0, i, point);
            PreserveContinuity(0, i, point);

            GetData(3, i, point);
            PreserveContinuity(3, i, point);
        }
    }
    _translatedY = value;
    DeleteVertexBufferObjectsOfData();
    UpdateVertexBufferObjectsOfData();
    return true;
}

GLboolean SecTrigPatch3::translateZ(double value) {
    if (_translatedZ == value) return false;
    double diff = value - _translatedZ;
    for(GLuint i = 0; i < _data.GetRowCount(); i++) {
        for(GLuint j = 0; j < _data.GetColumnCount(); j++) {
            DCoordinate3 point;
            GetData(i, j, point);
            SetData(i, j, point.x(), point.y(), point.z() + diff);
        }
    }
    if(_has_neighbours==true) {
        for(GLuint i = 0; i < _data.GetRowCount(); i++) {
            DCoordinate3 point;
            GetData(i, 0, point);
            PreserveContinuity(i, 0, point);

            GetData(i, 3, point);
            PreserveContinuity(i, 3, point);
        }
        for(GLuint i = 1; i < _data.GetRowCount()-1; i++) {
            DCoordinate3 point;
            GetData(0, i, point);
            PreserveContinuity(0, i, point);

            GetData(3, i, point);
            PreserveContinuity(3, i, point);
        }
    }
    _translatedZ = value;
    DeleteVertexBufferObjectsOfData();
    UpdateVertexBufferObjectsOfData();
    return true;
}

GLvoid SecTrigPatch3::SetImage(TriangulatedMesh3* image) {
    if (_image != nullptr) _image->DeleteVertexBufferObjects();
    _image = image;
    _image->UpdateVertexBufferObjects();
}

TriangulatedMesh3* SecTrigPatch3::GetImage() { return _image; }

GLboolean SecTrigPatch3::ExtendPatch(int dir, SecTrigPatch3 &patch) {
    _has_neighbours = true;
    patch._has_neighbours = true;
    if (_neighbours[dir] != nullptr) return false;
    _neighbours[dir] = &patch;
    patch.SetNeighbour((dir + 4)%8, dir, this);

    switch(dir) {
    case 0:
        ExtendNorth(patch);
        _connected_side[0] = 4;
        break;

    case 1:
        ExtendNorthEast(patch);
        _connected_side[1] = 5;
        break;

    case 2:
        ExtendEast(patch);
        _connected_side[2] = 6;
        break;
    case 3:
        ExtendSouthEast(patch);
        _connected_side[3] = 7;
        break;

    case 4:
        ExtendSouth(patch);
        _connected_side[4] = 0;
        break;

    case 5:
        ExtendSouthWest(patch);
        _connected_side[5] = 1;
        break;

    case 6:
        ExtendWest(patch);
        _connected_side[6] = 2;
        break;
    case 7:
        ExtendNorthWest(patch);
        _connected_side[3] = 7;
        break;
    }

    patch.UpdateVertexBufferObjectsOfData();
    return true;
}

GLboolean SecTrigPatch3::ExtendNorth(SecTrigPatch3 &patch) {
    // Set patch last row to our first row
    for(int i = 0; i < 4; i++) {
        DCoordinate3 point;
        GetData(0, i, point);
        patch.SetData(3, i, point);
    }

    // Set patch second to last row to 2 * first row - second row
    for(int i = 0; i < 4; i++) {
        DCoordinate3 point;
        DCoordinate3 point2;
        GetData(0, i, point);
        GetData(1, i, point2);
        patch.SetData(2, i, 2 * point - point2);
    }
    // Do this for the remaining patch
    for(int i = 1; i >= 0; i--) {
        for(int j = 0; j < 4; j++) {
            DCoordinate3 point;
            DCoordinate3 point2;
            patch.GetData(i+1, j, point);
            patch.GetData(i+2, j, point2);
            patch.SetData(i, j, 2 * point - point2);
        }
    }

    return true;
}

GLboolean SecTrigPatch3::ExtendEast(SecTrigPatch3 &patch) {
    // Set patch first column to our last column
    for(int i = 0; i < 4; i++) {
        DCoordinate3 point;
        GetData(i, 3, point);
        patch.SetData(i, 0, point);
    }

    // Set patch second to last row to 2 * first row - second row
    for(int i = 0; i < 4; i++) {
        DCoordinate3 point;
        DCoordinate3 point2;
        GetData(i, 3, point);
        GetData(i, 2, point2);
        patch.SetData(i, 1, 2 * point - point2);
    }
    // Do this for the remaining patch
    for(int i = 2; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            DCoordinate3 point;
            DCoordinate3 point2;
            patch.GetData(j, i-1, point);
            patch.GetData(j, i-2, point2);
            patch.SetData(j, i, 2 * point - point2);
        }
    }

    return true;
}

GLboolean SecTrigPatch3::ExtendSouth(SecTrigPatch3 &patch) {
    // Set patch first row to our last row
    for(int i = 0; i < 4; i++) {
        DCoordinate3 point;
        GetData(3, i, point);
        patch.SetData(0, i, point);
    }

    // Set patch second row to 2 * last row - second to last row
    for(int i = 0; i < 4; i++) {
        DCoordinate3 point;
        DCoordinate3 point2;
        GetData(3, i, point);
        GetData(2, i, point2);
        patch.SetData(1, i, 2 * point - point2);
    }
    // Do this for the remaining patch
    for(int i = 2; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            DCoordinate3 point;
            DCoordinate3 point2;
            patch.GetData(i-1, j, point);
            patch.GetData(i-2, j, point2);
            patch.SetData(i, j, 2 * point - point2);
        }
    }

    return true;
}

GLboolean SecTrigPatch3::ExtendWest(SecTrigPatch3 &patch) {
    // Set patch last column to our first column
    for(int i = 0; i < 4; i++) {
        DCoordinate3 point;
        GetData(i, 0, point);
        patch.SetData(i, 3, point);
    }

    // Set patch second to last columt to 2 * first colum - second column
    for(int i = 0; i < 4; i++) {
        DCoordinate3 point;
        DCoordinate3 point2;
        GetData(i, 0, point);
        GetData(i, 1, point2);
        patch.SetData(i, 2, 2 * point - point2);
    }
    // Do this for the remaining patch
    for(int i = 1; i >= 0; i--) {
        for(int j = 0; j < 4; j++) {
            DCoordinate3 point;
            DCoordinate3 point2;
            patch.GetData(j, i+1, point);
            patch.GetData(j, i+2, point2);
            patch.SetData(j, i, 2 * point - point2);
        }
    }

    return true;
}

GLboolean SecTrigPatch3::ExtendNorthEast(SecTrigPatch3 &patch) {
    DCoordinate3 p1;
    DCoordinate3 p2;
    DCoordinate3 p3;
    DCoordinate3 p4;

    GetData(0, 3, p1);
    GetData(0, 2, p2);
    GetData(1, 3, p3);
    GetData(1, 2, p4);

    patch.SetData(3, 0, p1);
    patch.SetData(3, 1, 2 * p1 - p2);
    patch.SetData(2, 0, 2 * p1 - p3);
    patch.SetData(2, 1, 2 * p1 - p4);
    // First Row
    for (int i = 2; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 pp;
        patch.GetData(3, i-2, p);
        patch.GetData(3, i-1, pp);
        patch.SetData(3, i, 2 * pp - p);
    }

    // First Col
    for (int i = 1; i >= 0; i--) {
        DCoordinate3 p;
        DCoordinate3 pp;
        patch.GetData(i+2, 0, pp);
        patch.GetData(i+1, 0, p);
        patch.SetData(i, 0, 2 * p - pp);
    }

    // Second Row
    for (int i = 2; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 pp;
        patch.GetData(2, i-2, p);
        patch.GetData(2, i-1, pp);
        patch.SetData(2, i, 2 * pp - p);
    }

    // Second Col
    for (int i = 1; i >= 0; i--) {
        DCoordinate3 p;
        DCoordinate3 pp;
        patch.GetData(i+2, 1, pp);
        patch.GetData(i+1, 1, p);
        patch.SetData(i, 1, 2 * p - pp);
    }

    for (int i = 2; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            DCoordinate3 p;
            DCoordinate3 pp;
            patch.GetData(j, i-2, p);
            patch.GetData(j, i-1, pp);
            patch.SetData(j, i, 2 * pp - p);
        }
    }

    return true;
}
GLboolean SecTrigPatch3::ExtendSouthEast(SecTrigPatch3 &patch) {
    DCoordinate3 p1;
    DCoordinate3 p2;
    DCoordinate3 p3;
    DCoordinate3 p4;

    GetData(3, 3, p1);
    GetData(3, 2, p2);
    GetData(2, 3, p3);
    GetData(2, 2, p4);

    patch.SetData(0, 0, p1);
    patch.SetData(0, 1, 2 * p1 - p2);
    patch.SetData(1, 0, 2 * p1 - p3);
    patch.SetData(1, 1, 2 * p1 - p4);
    // First Row
    for (int i = 2; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 pp;
        patch.GetData(0, i-2, p);
        patch.GetData(0, i-1, pp);
        patch.SetData(0, i, 2 * pp - p);
    }

    // First Col
    for (int i = 2; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 pp;
        patch.GetData(i-2, 0, p);
        patch.GetData(i-1, 0, pp);
        patch.SetData(i, 0, 2 * pp - p);
    }

    // Second Row
    for (int i = 2; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 pp;
        patch.GetData(1, i-2, p);
        patch.GetData(1, i-1, pp);
        patch.SetData(1, i, 2 * pp - p);
    }

    // Second Col
    for (int i = 2; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 pp;
        patch.GetData(i-2, 1, p);
        patch.GetData(i-1, 1, pp);
        patch.SetData(i, 1, 2 * pp - p);
    }

    for (int i = 2; i < 4; i++) {
        for (int j = 2; j < 4; j++) {
            DCoordinate3 p;
            DCoordinate3 pp;
            patch.GetData(j, i-2, p);
            patch.GetData(j, i-1, pp);
            patch.SetData(j, i, 2 * pp - p);
        }
    }
    return true;
}
GLboolean SecTrigPatch3::ExtendSouthWest(SecTrigPatch3 &patch) {
    DCoordinate3 p1;
    DCoordinate3 p2;
    DCoordinate3 p3;
    DCoordinate3 p4;

    GetData(3, 0, p1);
    GetData(3, 1, p2);
    GetData(2, 0, p3);
    GetData(2, 1, p4);

    patch.SetData(0, 3, p1);
    patch.SetData(0, 2, 2 * p1 - p2);
    patch.SetData(1, 3, 2 * p1 - p3);
    patch.SetData(1, 2, 2 * p1 - p4);

    // First Row
    for (int i = 1; i >= 0; i--) {
        DCoordinate3 p;
        DCoordinate3 pp;
        patch.GetData(0, i+2, p);
        patch.GetData(0, i+1, pp);
        patch.SetData(0, i, 2 * pp - p);
    }

    // First Col
    for (int i = 2; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 pp;
        patch.GetData(i-2, 3, p);
        patch.GetData(i-1, 3, pp);
        patch.SetData(i, 3, 2 * pp - p);
    }

    // Second Row
    for (int i = 1; i >= 0; i--) {
        DCoordinate3 p;
        DCoordinate3 pp;
        patch.GetData(1, i+2, p);
        patch.GetData(1, i+1, pp);
        patch.SetData(1, i, 2 * pp - p);
    }

    // Second Col
    for (int i = 2; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 pp;
        patch.GetData(i-2, 2, p);
        patch.GetData(i-1, 2, pp);
        patch.SetData(i, 2, 2 * pp - p);
    }

    for (int i = 2; i < 4; i++) {
        for (int j = 1; j >= 0; j--) {
            DCoordinate3 p;
            DCoordinate3 pp;
            patch.GetData(i, j+2, p);
            patch.GetData(i, j+1, pp);
            patch.SetData(i, j, 2 * pp - p);
        }
    }
    return true;
}
GLboolean SecTrigPatch3::ExtendNorthWest(SecTrigPatch3 &patch) {
    DCoordinate3 p1;
    DCoordinate3 p2;
    DCoordinate3 p3;
    DCoordinate3 p4;

    GetData(0, 0, p1);
    GetData(0, 1, p2);
    GetData(1, 0, p3);
    GetData(1, 1, p4);

    patch.SetData(3, 3, p1);
    patch.SetData(3, 2, 2 * p1 - p2);
    patch.SetData(2, 3, 2 * p1 - p3);
    patch.SetData(2, 2, 2 * p1 - p4);

    // First Row
    for (int i = 1; i >= 0; i--) {
        DCoordinate3 p;
        DCoordinate3 pp;
        patch.GetData(3, i+2, p);
        patch.GetData(3, i+1, pp);
        patch.SetData(3, i, 2 * pp - p);
    }

    // First Col
    for (int i = 1; i >= 0; i--) {
        DCoordinate3 p;
        DCoordinate3 pp;
        patch.GetData(i+2, 3, p);
        patch.GetData(i+1, 3, pp);
        patch.SetData(i, 3, 2 * pp - p);
    }

    // Second Row
    for (int i = 1; i >= 0; i--) {
        DCoordinate3 p;
        DCoordinate3 pp;
        patch.GetData(2, i+2, p);
        patch.GetData(2, i+1, pp);
        patch.SetData(2, i, 2 * pp - p);
    }

    // Second Col
    for (int i = 1; i >= 0; i--) {
        DCoordinate3 p;
        DCoordinate3 pp;
        patch.GetData(i+2, 2, p);
        patch.GetData(i+1, 2, pp);
        patch.SetData(i, 2, 2 * pp - p);
    }

    for (int i = 1; i >= 0; i--) {
        for (int j = 1; j >= 0; j--) {
            DCoordinate3 p;
            DCoordinate3 pp;
            patch.GetData(i, j+2, p);
            patch.GetData(i, j+1, pp);
            patch.SetData(i, j, 2 * pp - p);
        }
    }
    return false;
}

double SecTrigPatch3::GetTranslatedX() {
    return _translatedX;
}
double SecTrigPatch3::GetTranslatedY() {
    return _translatedY;
}
double SecTrigPatch3::GetTranslatedZ() {
    return _translatedZ;
}


GLboolean SecTrigPatch3::SetUip(RowMatrix<GenericCurve3*>* uip) {
    if (_uip) {
        for (GLuint i = 0; i < _uip->GetColumnCount(); i++) {
            GenericCurve3 *curve = (*_uip)[i];
            if (curve) {
                curve->DeleteVertexBufferObjects();
            }
        }
    }

    if (!uip) return false;

    _uip = uip;

    for (GLuint i = 0; i < _uip->GetColumnCount(); i++) {
        GenericCurve3 *curve = (*_uip)[i];
        if (curve) {
            curve->UpdateVertexBufferObjects();
        }
    }

    return true;
}
GLboolean SecTrigPatch3::SetVip(RowMatrix<GenericCurve3*>* vip) {
    if (_vip) {
        for (GLuint i = 0; i < _vip->GetColumnCount(); i++) {
            GenericCurve3 *curve = (*_vip)[i];
            if (curve) {
                curve->DeleteVertexBufferObjects();
            }
        }
    }

    if (!vip) return false;

    _vip = vip;

    for (GLuint i = 0; i < _vip->GetColumnCount(); i++) {
        GenericCurve3 *curve = (*_vip)[i];
        if (curve) {
            curve->UpdateVertexBufferObjects();
        }
    }

    return true;
}

RowMatrix<GenericCurve3*>* SecTrigPatch3::GetUip() {
    return _uip;
}
RowMatrix<GenericCurve3*>* SecTrigPatch3::GetVip() {
    return _vip;
}

GLboolean SecTrigPatch3::SetNeighbour(int neighbour, int side, SecTrigPatch3 *patch) {
    if(_neighbours[neighbour] != nullptr) return false;
    _neighbours[neighbour] = patch;
    _connected_side[neighbour] = side;
    return true;
}

/* ########## JOINING OF PATCHES  #################### */
GLboolean SecTrigPatch3::JoinPatch(SecTrigPatch3 &patch, int side1, int side2, SecTrigPatch3 &result) {
    _has_neighbours = true;
    patch._has_neighbours = true;
    bool joinResult = false;
    if(side1 == side2) {
        switch (side1) {
        case(0):
            joinResult = JoinNorthNorth(patch, result);
            break;
        case(2):
            joinResult = JoinEastEast(patch, result);
            break;
        case(4):
            joinResult = JoinSouthSouth(patch, result);
            break;
        case(6):
            joinResult = JoinWestWest(patch, result);
            break;
        }
    } else {
        if ((side1 == 2 || side1 == 6) && (side2 == 2 || side2 == 6)) joinResult = JoinEastWest(patch, side1, side2, result);
        if ((side1 == 0 || side1 == 4) && (side2 == 0 || side2 == 4)) joinResult = JoinNorthSouth(patch, side1, side2, result);

        if (side1 == 0 && side2 == 2) joinResult = JoinNorthEast(patch, result);
        if (side1 == 2 && side2 == 0) joinResult = patch.JoinNorthEast(*this, result);

        if (side1 == 0 && side2 == 6) joinResult = JoinNorthWest(patch, result);
        if (side1 == 6 && side2 == 0) joinResult = patch.JoinNorthWest(*this, result);

        if (side1 == 4 && side2 == 2) joinResult = JoinSouthEast(patch, result);
        if (side1 == 2 && side2 == 4) joinResult = patch.JoinSouthEast(*this, result);

        if (side1 == 4 && side2 == 6) joinResult = JoinSouthWest(patch, result);
        if (side1 == 6 && side2 == 4) joinResult = patch.JoinSouthWest(*this, result);
    }

    if (side1%4 != side2%4) {
        PreserveAlphaU(GetUAlpha());
        PreserveAlphaV(GetUAlpha());
    } else if (side1 %4 == 0 ) {
        PreserveAlphaU(GetUAlpha());
    } else if (side1 %4 == 2 ){
        PreserveAlphaV(GetVAlpha());
    }

    return joinResult;
}

GLboolean SecTrigPatch3::JoinEastWest(SecTrigPatch3 &patch, int side1, int side2, SecTrigPatch3 &result) {
    // Set the first column of the result to our last column
    // Set the last column of the result to the patch first column
    int p1_col1;
    int p1_col2;

    int p2_col1;
    int p2_col2;
    if (side2 == 2 && side1 == 6) {
        return patch.JoinEastWest(*this, side2, side1, result);
    } else {
        SetNeighbour(side1, side2, &result);
        patch.SetNeighbour(side2, side1, &result);
        result.SetNeighbour(side1, side2, &patch);
        result.SetNeighbour(side2, side1, this);
    }

    if (side1 == 2) {
        p1_col1 = 3;
        p1_col2 = 2;

        p2_col1 = 0;
        p2_col2 = 1;
    } else {
        p1_col1 = 0;
        p1_col2 = 1;

        p2_col1 = 3;
        p2_col2 = 2;
    }
    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        GetData(i, p1_col1, p);
        result.SetData(i, 0, p);

        patch.GetData(i, p2_col1, p2);
        result.SetData(i, 3, p2);
    }

    // Set the second column of the result to our line
    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        GetData(i, p1_col1, p);
        GetData(i, p1_col2, p2);
        result.SetData(i, 1, 2 * p - p2);
    }
    // Set the third column to the patch's line
    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        patch.GetData(i, p2_col1, p);
        patch.GetData(i, p2_col2, p2);
        result.SetData(i, 2, 2 * p - p2);
    }

    result.UpdateVertexBufferObjectsOfData();
    return true;
}

GLboolean SecTrigPatch3::JoinNorthSouth(SecTrigPatch3 &patch, int side1, int side2, SecTrigPatch3 &result) {
    int p1_row1;
    int p1_row2;

    int p2_row1;
    int p2_row2;
    if (side2 != 4) {
        SetNeighbour(side1, side2, &result);
        patch.SetNeighbour(side2, side1, &result);
        result.SetNeighbour(side1, side2, &patch);
        result.SetNeighbour(side2, side1, this);
    } else {
        return patch.JoinNorthSouth(*this, side2, side1, result);
    }


    if (side1 == 4) {
        p1_row1 = 3;
        p1_row2 = 2;

        p2_row1 = 0;
        p2_row2 = 1;
    } else {
        p1_row1 = 0;
        p1_row2 = 1;

        p2_row1 = 3;
        p2_row2 = 2;
    }
    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        GetData(p1_row1, i, p);
        result.SetData(0, i, p);

        patch.GetData(p2_row1, i, p2);
        result.SetData(3, i, p2);
    }


    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        GetData(p1_row1, i, p);
        GetData(p1_row2, i, p2);
        result.SetData(1, i, 2 * p - p2);
    }

    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        patch.GetData(p2_row1, i, p);
        patch.GetData(p2_row2, i, p2);
        result.SetData(2, i, 2 * p - p2);
    }
    result.UpdateVertexBufferObjectsOfData();
    return true;
}

GLboolean SecTrigPatch3::JoinEastEast(SecTrigPatch3 &patch, SecTrigPatch3 &result) {
    SetNeighbour(2, 6, &result);
    patch.SetNeighbour(2, 2, &result);
    result.SetNeighbour(2, 2, &patch);
    result.SetNeighbour(6, 2, this);

    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        GetData(i, 3, p);
        result.SetData(i, 0, p);

        patch.GetData(i, 3, p2);
        result.SetData(i, 3, p2);
    }

    // Set the second column of the result to our line
    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        GetData(i, 3, p);
        GetData(i, 2, p2);
        result.SetData(i, 1, 2 * p - p2);
    }
    // Set the third column to the patch's line
    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        patch.GetData(i, 3, p);
        patch.GetData(i, 2, p2);
        result.SetData(i, 2, 2 * p - p2);
    }
    result.UpdateVertexBufferObjectsOfData();
    return true;
}

GLboolean SecTrigPatch3::JoinWestWest(SecTrigPatch3 &patch, SecTrigPatch3 &result) {
    SetNeighbour(6, 2, &result);
    patch.SetNeighbour(6, 6, &result);
    result.SetNeighbour(6, 6, &patch);
    result.SetNeighbour(2, 6, this);

    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        GetData(i, 0, p);
        result.SetData(i, 3, p);

        patch.GetData(i, 0, p2);
        result.SetData(i, 0, p2);
    }

    // Set the second column of the result to our line
    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        GetData(i, 0, p);
        GetData(i, 1, p2);
        result.SetData(i, 2, 2 * p - p2);
    }
    // Set the third column to the patch's line
    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        patch.GetData(i, 0, p);
        patch.GetData(i, 1, p2);
        result.SetData(i, 1, 2 * p - p2);
    }
    result.UpdateVertexBufferObjectsOfData();
    return true;
}

GLboolean SecTrigPatch3::JoinSouthSouth(SecTrigPatch3 &patch, SecTrigPatch3 &result) {
    SetNeighbour(4, 0, &result);
    patch.SetNeighbour(4, 4, &result);
    result.SetNeighbour(4, 4, &patch);
    result.SetNeighbour(0, 4, this);
    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        GetData(3, i, p);
        result.SetData(0, i, p);

        patch.GetData(3, i, p2);
        result.SetData(3, i, p2);
    }

    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        GetData(3, i, p);
        GetData(2, i, p2);
        result.SetData(1, i, 2 * p - p2);
    }

    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        patch.GetData(3, i, p);
        patch.GetData(2, i, p2);
        result.SetData(2, i, 2 * p - p2);
    }
    result.UpdateVertexBufferObjectsOfData();
    return true;
}

GLboolean SecTrigPatch3::JoinNorthNorth(SecTrigPatch3 &patch, SecTrigPatch3 &result) {
    SetNeighbour(0, 4, &result);
    patch.SetNeighbour(0, 0, &result);
    result.SetNeighbour(0, 0, &patch);
    result.SetNeighbour(4, 0, this);

    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        GetData(0, i, p);
        result.SetData(3, i, p);

        patch.GetData(0, i, p2);
        result.SetData(0, i, p2);
    }

    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        GetData(0, i, p);
        GetData(1, i, p2);
        result.SetData(2, i, 2 * p - p2);
    }

    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        patch.GetData(0, i, p);
        patch.GetData(1, i, p2);
        result.SetData(1, i, 2 * p - p2);
    }
    result.UpdateVertexBufferObjectsOfData();
    return true;
}

GLboolean SecTrigPatch3::JoinNorthEast(SecTrigPatch3 &patch, SecTrigPatch3 &result) {
    int p1_row1;
    int p1_row2;

    int p2_col1;
    int p2_col2;

    SetNeighbour(0, 4, &result);
    patch.SetNeighbour(2, 0, &result);
    result.SetNeighbour(0, 2, &patch);
    result.SetNeighbour(4, 0, this);

    p1_row1 = 0;
    p1_row2 = 1;

    p2_col1 = 3;
    p2_col2 = 2;

    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        GetData(p1_row1, i, p);
        result.SetData(3, i, p);

        patch.GetData(i, p2_col1, p2);
        result.SetData(0, 3 - i, p2);
    }

    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        GetData(p1_row1, i, p);
        GetData(p1_row2, i, p2);
        result.SetData(2, i, 2 * p - p2);
    }

    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        patch.GetData(i, p2_col1, p);
        patch.GetData(i, p2_col2, p2);
        result.SetData(1, 3 - i, 2 * p - p2);
    }
    result.UpdateVertexBufferObjectsOfData();
    return true;
}

GLboolean SecTrigPatch3::JoinNorthWest(SecTrigPatch3 &patch, SecTrigPatch3 &result) {
    int p1_row1;
    int p1_row2;

    int p2_col1;
    int p2_col2;

    SetNeighbour(0, 4, &result);
    patch.SetNeighbour(6, 0, &result);
    result.SetNeighbour(0, 6, &patch);
    result.SetNeighbour(4, 0, this);

    p1_row1 = 0;
    p1_row2 = 1;

    p2_col1 = 0;
    p2_col2 = 1;

    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        GetData(p1_row1, i, p);
        result.SetData(3, i, p);

        patch.GetData(i, p2_col1, p2);
        result.SetData(0, i, p2);
    }

    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        GetData(p1_row1, i, p);
        GetData(p1_row2, i, p2);
        result.SetData(2, i, 2 * p - p2);
    }

    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        patch.GetData(i, p2_col1, p);
        patch.GetData(i, p2_col2, p2);
        result.SetData(1, i, 2 * p - p2);
    }
    result.UpdateVertexBufferObjectsOfData();
    return true;
}

GLboolean SecTrigPatch3::JoinSouthWest(SecTrigPatch3 &patch, SecTrigPatch3 &result) {
    int p1_row1;
    int p1_row2;

    int p2_col1;
    int p2_col2;

    SetNeighbour(4, 0, &result);
    patch.SetNeighbour(6, 4, &result);
    result.SetNeighbour(4, 6, &patch);
    result.SetNeighbour(0, 4, this);

    p1_row1 = 3;
    p1_row2 = 2;

    p2_col1 = 0;
    p2_col2 = 1;

    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        GetData(p1_row1, i, p);
        result.SetData(0, i, p);

        patch.GetData(3 - i, p2_col1, p2);
        result.SetData(3, i, p2);
    }

    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        GetData(p1_row1, i, p);
        GetData(p1_row2, i, p2);
        result.SetData(1, i, 2 * p - p2);
    }

    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        patch.GetData(3 - i, p2_col1, p);
        patch.GetData(3 - i, p2_col2, p2);
        result.SetData(2, i , 2 * p - p2);
    }
    result.UpdateVertexBufferObjectsOfData();
    return true;
}

GLboolean SecTrigPatch3::JoinSouthEast(SecTrigPatch3 &patch, SecTrigPatch3 &result) {
    int p1_row1;
    int p1_row2;

    int p2_col1;
    int p2_col2;

    SetNeighbour(4, 0, &result);
    patch.SetNeighbour(2, 4, &result);
    result.SetNeighbour(4, 2, &patch);
    result.SetNeighbour(0, 4, this);

    p1_row1 = 3;
    p1_row2 = 2;

    p2_col1 = 3;
    p2_col2 = 2;

    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        GetData(p1_row1, i, p);
        result.SetData(0, i, p);

        patch.GetData(i, p2_col1, p2);
        result.SetData(3, i, p2);
    }

    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        GetData(p1_row1, i, p);
        GetData(p1_row2, i, p2);
        result.SetData(1, i, 2 * p - p2);
    }

    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        patch.GetData(i, p2_col1, p);
        patch.GetData(i, p2_col2, p2);
        result.SetData(2, i, 2 * p - p2);
    }
    result.UpdateVertexBufferObjectsOfData();
    return true;
}


/* ########## MERGING OF PATCHES  #################### */

GLboolean SecTrigPatch3::MergePatch(SecTrigPatch3 &patch, int side1, int side2) {
    _has_neighbours = true;
    patch._has_neighbours = true;
    if (_neighbours[side1] != nullptr) return false;
    if (patch._neighbours[side2] != nullptr) return false;

    if (side1 == side2) {
        switch(side1) {
        case (0):
            MergeNorthNorth(patch);
            break;
        case (2):
            MergeEastEast(patch);
            break;
        case (4):
            MergeSouthSouth(patch);
            break;
        case (6):
            MergeWestWest(patch);
            break;
        }
    }

    if ((side1 == 0 && side2 == 4) || (side1 == 4 && side2 == 0)) MergeNorthSouth(patch, side1, side2);
    else if ((side1 == 2 && side2 == 6) || (side1 == 6 && side2 == 2)) MergeEastWest(patch, side1, side2);
    else if (side1 == 0 && side2 == 2) MergeNorthEast(patch);
    else if (side1 == 2 && side2 == 0) patch.MergeNorthEast(*this);
    else if (side1 == 0 && side2 == 6) MergeNorthWest(patch);
    else if (side1 == 6 && side2 == 0) patch.MergeNorthWest(*this);
    else if (side1 == 4 && side2 == 2) MergeSouthEast(patch);
    else if (side1 == 2 && side2 == 4) patch.MergeSouthEast(*this);
    else if (side1 == 4 && side2 == 6) MergeSouthWest(patch);
    else if (side1 == 6 && side2 == 4) patch.MergeSouthWest(*this);

    if (side1%4 != side2%4) {
        PreserveAlphaU(GetUAlpha());
        PreserveAlphaV(GetUAlpha());
    } else if (side1 %4 == 0 ) {
        PreserveAlphaU(GetUAlpha());
    } else if (side1 %4 == 2 ){
        PreserveAlphaV(GetVAlpha());
    }

    DeleteVertexBufferObjectsOfData();
    UpdateVertexBufferObjectsOfData();
    patch.DeleteVertexBufferObjectsOfData();
    patch.UpdateVertexBufferObjectsOfData();
    return true;
}

GLboolean SecTrigPatch3::MergeEastWest(SecTrigPatch3 &patch, int side1, int side2) {
    // Set the first column of the result to our last column
    // Set the last column of the result to the patch first column
    int p1_col1;
    int p1_col2;

    int p2_col1;
    int p2_col2;

    _neighbours[side1] = &patch;
    _connected_side[side1] = side2;
    patch.SetNeighbour(side2, side1, this);

    if (side1 == 2) {
        p1_col1 = 3;
        p1_col2 = 2;

        p2_col1 = 0;
        p2_col2 = 1;
    } else {
        p1_col1 = 0;
        p1_col2 = 1;

        p2_col1 = 3;
        p2_col2 = 2;
    }
    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        DCoordinate3 p3;
        GetData(i, p1_col1, p);
        patch.GetData(i, p2_col1, p2);
        GetData(i, p1_col2, p3);

        SetData(i, p1_col1, (p + p2) / 2);
        patch.SetData(i, p2_col1, (p + p2) / 2);

        patch.SetData(i, p2_col2, p + p2 - p3);
    }

    for (int i = 0; i < 4; i++) {
        DCoordinate3 p;
        GetData(i, p1_col1, p);
        PreserveContinuity(i, p1_col1, p);
    }

    return true;
}

GLboolean SecTrigPatch3::MergeNorthSouth(SecTrigPatch3 &patch, int side1, int side2) {
    int p1_row1;
    int p1_row2;

    int p2_row1;
    int p2_row2;

    _neighbours[side1] = &patch;
    _connected_side[side1] = side2;
    patch.SetNeighbour(side2, side1, this);

    if (side1 == 4) {
        p1_row1 = 3;
        p1_row2 = 2;

        p2_row1 = 0;
        p2_row2 = 1;
    } else {
        p1_row1 = 0;
        p1_row2 = 1;

        p2_row1 = 3;
        p2_row2 = 2;
    }

    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        DCoordinate3 p3;
        GetData(p1_row1, i, p);
        patch.GetData(p2_row1, i, p2);
        GetData(p1_row2, i, p3);

        SetData(p1_row1, i, (p + p2) / 2);
        patch.SetData(p2_row1, i, (p + p2) / 2);
        patch.SetData(p2_row2, i, p + p2 - p3);
    }

    for (int i = 0; i < 4; i++) {
        DCoordinate3 p;
        GetData(p1_row1, i, p);
        PreserveContinuity(p1_row1, i, p);
    }
    return true;
}

GLboolean SecTrigPatch3::MergeEastEast(SecTrigPatch3 &patch) {
    _neighbours[2] = &patch;
    _connected_side[2] = 2;
    patch.SetNeighbour(2, 2, this);
    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        DCoordinate3 p3;
        GetData(i, 3, p);
        patch.GetData(i, 3, p2);
        GetData(i, 2, p3);

        SetData(i, 3, (p + p2) / 2);
        patch.SetData(i, 3, (p + p2) / 2);

        patch.SetData(i, 2, p + p2 - p3);
    }

    for (int i = 0; i < 4; i++) {
        DCoordinate3 p;
        GetData(i, 3, p);
        PreserveContinuity(i, 3, p);
    }

    return true;
}

GLboolean SecTrigPatch3::MergeWestWest(SecTrigPatch3 &patch) {
    _neighbours[6] = &patch;
    _connected_side[6] = 6;
    patch.SetNeighbour(6, 6, this);
    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        DCoordinate3 p3;
        GetData(i, 0, p);
        patch.GetData(i, 0, p2);
        GetData(i, 1, p3);

        SetData(i, 0, (p + p2) / 2);
        patch.SetData(i, 0, (p + p2) / 2);

        patch.SetData(i, 1, p + p2 - p3);
    }

    for (int i = 0; i < 4; i++) {
        DCoordinate3 p;
        GetData(i, 0, p);
        PreserveContinuity(i, 0, p);
    }
    return true;
}

GLboolean SecTrigPatch3::MergeNorthNorth(SecTrigPatch3 &patch) {
    _neighbours[0] = &patch;
    _connected_side[0] = 0;
    patch.SetNeighbour(0, 0, this);
    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        DCoordinate3 p3;
        GetData(0, i, p);
        patch.GetData(0, i, p2);
        GetData(1, i, p3);
        SetData(0, i, (p + p2) / 2);
        patch.SetData(0, i, (p + p2) / 2);
        patch.SetData(1, i, p + p2 - p3);
    }

    for (int i = 0; i < 4; i++) {
        DCoordinate3 p;
        GetData(0, i, p);
        PreserveContinuity(0, i, p);
    }
    return true;
}

GLboolean SecTrigPatch3::MergeSouthSouth(SecTrigPatch3 &patch) {
    _neighbours[4] = &patch;
    _connected_side[4] = 4;
    patch.SetNeighbour(4, 4, this);
    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        DCoordinate3 p3;
        GetData(3, i, p);
        patch.GetData(3, i, p2);
        GetData(2, i, p3);
        SetData(3, i, (p + p2) / 2);
        patch.SetData(3, i, (p + p2) / 2);
        patch.SetData(2, i, p + p2 - p3);
    }

    for (int i = 0; i < 4; i++) {
        DCoordinate3 p;
        GetData(3, i, p);
        PreserveContinuity(3, i, p);
    }
    return true;
}

GLboolean SecTrigPatch3::MergeNorthWest(SecTrigPatch3 &patch) {
    _neighbours[0] = &patch;
    _connected_side[0] = 6;
    patch.SetNeighbour(6, 0, this);
    // Set the first column of the result to our last column
    // Set the last column of the result to the patch first column
    int p1_row1;
    int p1_row2;

    int p2_col1;
    int p2_col2;

    p1_row1 = 0;
    p1_row2 = 1;

    p2_col1 = 0;
    p2_col2 = 1;

    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        DCoordinate3 p3;
        GetData(p1_row1, i, p);
        GetData(p1_row2, i, p3);
        patch.GetData(i, p2_col1, p2);


        SetData(p1_row1, i, (p + p2) / 2);
        patch.SetData(i, p2_col1, (p + p2) / 2);
        patch.SetData(i, p2_col2, p + p2 - p3);
    }

    for (int i = 0; i < 4; i++) {
        DCoordinate3 p;
        GetData(p1_row1, i, p);
        PreserveContinuity(p1_row1, i, p);
    }
    return true;
}

GLboolean SecTrigPatch3::MergeNorthEast(SecTrigPatch3 &patch) {
    _neighbours[0] = &patch;
    _connected_side[0] = 2;
    patch.SetNeighbour(2, 0, this);
    // Set the first column of the result to our last column
    // Set the last column of the result to the patch first column
    int p1_row1;
    int p1_row2;

    int p2_col1;
    int p2_col2;

    p1_row1 = 0;
    p1_row2 = 1;

    p2_col1 = 3;
    p2_col2 = 2;

    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        DCoordinate3 p3;
        GetData(p1_row1, 3 - i, p);
        GetData(p1_row2, 3 - i, p3);
        patch.GetData(i, p2_col1, p2);


        SetData(p1_row1, 3 - i, (p + p2) / 2);
        patch.SetData(i, p2_col1, (p + p2) / 2);
        patch.SetData(i, p2_col2, p + p2 - p3);
    }

    for (int i = 0; i < 4; i++) {
        DCoordinate3 p;
        GetData(p1_row1, 3 - i, p);
        PreserveContinuity(p1_row1, 3 - i, p);
    }
    return true;
}

GLboolean SecTrigPatch3::MergeSouthWest(SecTrigPatch3 &patch) {
    _neighbours[4] = &patch;
    _connected_side[4] = 6;
    patch.SetNeighbour(6, 4, this);
    // Set the first column of the result to our last column
    // Set the last column of the result to the patch first column
    int p1_row1;
    int p1_row2;

    int p2_col1;
    int p2_col2;

    p1_row1 = 3;
    p1_row2 = 2;

    p2_col1 = 0;
    p2_col2 = 1;

    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        DCoordinate3 p3;
        GetData(p1_row1, i, p);
        GetData(p1_row2, i, p3);
        patch.GetData(3 - i, p2_col1, p2);


        SetData(p1_row1, i, (p + p2) / 2);
        patch.SetData(3 - i, p2_col1, (p + p2) / 2);
        patch.SetData(3 - i, p2_col2, p + p2 - p3);
    }

    for (int i = 0; i < 4; i++) {
        DCoordinate3 p;
        GetData(p1_row1, i, p);
        PreserveContinuity(p1_row1, i, p);
    }
    return true;
}

GLboolean SecTrigPatch3::MergeSouthEast(SecTrigPatch3 &patch) {
    _neighbours[4] = &patch;
    _connected_side[4] = 2;
    patch.SetNeighbour(2, 4, this);
    // Set the first column of the result to our last column
    // Set the last column of the result to the patch first column
    int p1_row1;
    int p1_row2;

    int p2_col1;
    int p2_col2;

    p1_row1 = 3;
    p1_row2 = 2;

    p2_col1 = 3;
    p2_col2 = 2;

    for(int i = 0; i < 4; i++) {
        DCoordinate3 p;
        DCoordinate3 p2;
        DCoordinate3 p3;
        GetData(p1_row1, i, p);
        GetData(p1_row2, i, p3);
        patch.GetData(i, p2_col1, p2);


        SetData(p1_row1, i, (p + p2) / 2);
        patch.SetData(i, p2_col1, (p + p2) / 2);
        patch.SetData(i, p2_col2, p + p2 - p3);
    }

    for (int i = 0; i < 4; i++) {
        DCoordinate3 p;
        GetData(p1_row1, i, p);
        PreserveContinuity(p1_row1, i, p);
    }
    return true;
}

// 1. N
// 2. NE
// 3. E
// 4. SE
// 5. S
// 6. SW
// 7. W
// 8. NW
GLvoid SecTrigPatch3::PreserveContinuity(int row, int col, DCoordinate3 point) {
    if(_updated) return;
    _updated = true;

    DeleteVertexBufferObjectsOfData();
    UpdateVertexBufferObjectsOfData();
    SetImage(GenerateImage(30, 30));

    if (row <= 1) {
        // Modify patches in the directions: N, NE, NW
        PreserveNorth(row, col, point);
        // Row > 1
    } else {
        // Modify patches in the directions: S, SE, SW
        PreserveSouth(row, col, point);
    }

    if (col <= 1) {
        // Modify patches in the directions: E, NE, SE
        PreserveWest(row, col, point);
    } else {
        // Modify patches in the directions: W, NW, SW
        PreserveEast(row, col, point);
    }
    _updated = false;
}

GLvoid SecTrigPatch3::PreserveNorth(int row, int col, DCoordinate3 point) {
    if (!_neighbours[0]) return;
    if (row == 0) {
        if (_connected_side[0] == 4) {
            // South side
            DCoordinate3 adjacentPoint;
            GetData(1, col, adjacentPoint);
            _neighbours[0]->SetData(3, col, point);
            _neighbours[0]->SetData(2, col, 2 * point - adjacentPoint);
            _neighbours[0]->PreserveContinuity(3, col, point);
            _neighbours[0]->PreserveContinuity(2, col, 2 * point - adjacentPoint);
        } else if (_connected_side[0] == 0) {
            // South side
            DCoordinate3 adjacentPoint;
            GetData(1, col, adjacentPoint);
            _neighbours[0]->SetData(0, col, point);
            _neighbours[0]->SetData(1, col, 2 * point - adjacentPoint);
            _neighbours[0]->PreserveContinuity(0, col, point);
            _neighbours[0]->PreserveContinuity(1, col, 2 * point - adjacentPoint);
        } else if (_connected_side[0] == 2) {
            // South side
            DCoordinate3 adjacentPoint;
            GetData(1, col, adjacentPoint);
            _neighbours[0]->SetData(3 - col, 3, point);
            _neighbours[0]->SetData(3 - col, 2, 2 * point - adjacentPoint);
            _neighbours[0]->PreserveContinuity(3 - col, 3, point);
            _neighbours[0]->PreserveContinuity(3 - col, 2, 2 * point - adjacentPoint);
        } else if (_connected_side[0] == 6) {
            // South side
            DCoordinate3 adjacentPoint;
            GetData(1, col, adjacentPoint);
            _neighbours[0]->SetData(col, 0, point);
            _neighbours[0]->SetData(col, 1, 2 * point - adjacentPoint);
            _neighbours[0]->PreserveContinuity(col, 0, point);
            _neighbours[0]->PreserveContinuity(col, 1, 2 * point - adjacentPoint);
        } else if (_connected_side[0] == 3) {
            // SouthEast side
        } else if (_connected_side[0] == 5) {
            // SouthWest
        }
    } else {
        if (_connected_side[0] == 4) {
            // South side
            DCoordinate3 sidePoint;
            GetData(0, col, sidePoint);
            _neighbours[0]->SetData(2, col, 2 * sidePoint - point);
            _neighbours[0]->PreserveContinuity(2, col, 2 * sidePoint - point);
        } else if (_connected_side[0] == 0) {
            // South side
            DCoordinate3 sidePoint;
            GetData(0, col, sidePoint);
            _neighbours[0]->SetData(1, col, 2 * sidePoint - point);
            _neighbours[0]->PreserveContinuity(1, col, 2 * sidePoint - point);
        } else if (_connected_side[0] == 2) {
            // South side
            DCoordinate3 sidePoint;
            GetData(0, col, sidePoint);
            _neighbours[0]->SetData(3 - col, 2, 2 * sidePoint - point);
            _neighbours[0]->PreserveContinuity(3 - col, 2, 2 * sidePoint - point);
        } else if (_connected_side[0] == 6) {
            // South side
            DCoordinate3 sidePoint;
            GetData(0, col, sidePoint);
            _neighbours[0]->SetData(col, 1, 2 * sidePoint - point);
            _neighbours[0]->PreserveContinuity(col, 1, 2 * sidePoint - point);
        } else if (_connected_side[0] == 3) {
            // SouthEast side
        } else if (_connected_side[0] == 5) {
            // SouthWest
        }
    }
}
GLvoid SecTrigPatch3::PreserveEast(int row, int col, DCoordinate3 point) {
    if (!_neighbours[2]) return;
    if (col == 3) {
        if (_connected_side[2] == 6) {
            // West side
            DCoordinate3 adjacentPoint;
            GetData(row, 2, adjacentPoint);
            _neighbours[2]->SetData(row, 0, point);
            _neighbours[2]->SetData(row, 1, 2 * point - adjacentPoint);
            _neighbours[2]->PreserveContinuity(row, 0, point);
            _neighbours[2]->PreserveContinuity(row, 1, 2 * point - adjacentPoint);
        } else if (_connected_side[2] == 2) {
            // West side
            DCoordinate3 adjacentPoint;
            GetData(row, 2, adjacentPoint);
            _neighbours[2]->SetData(row, 3, point);
            _neighbours[2]->SetData(row, 2, 2 * point - adjacentPoint);
            _neighbours[2]->PreserveContinuity(row, 3, point);
            _neighbours[2]->PreserveContinuity(row, 2, 2 * point - adjacentPoint);
        } else if (_connected_side[2] == 0) {
            // North side
            DCoordinate3 adjacentPoint;
            GetData(row, 2, adjacentPoint);
            _neighbours[2]->SetData(0, 3 - row, point);
            _neighbours[2]->SetData(1, 3 - row, 2 * point - adjacentPoint);
            _neighbours[2]->PreserveContinuity(0, 3 - row, point);
            _neighbours[2]->PreserveContinuity(1, 3 - row, 2 * point - adjacentPoint);
        } else if (_connected_side[2] == 4) {
            // South side
            DCoordinate3 adjacentPoint;
            GetData(row, 2, adjacentPoint);
            _neighbours[2]->SetData(3, row, point);
            _neighbours[2]->SetData(2, row, 2 * point - adjacentPoint);
            _neighbours[2]->PreserveContinuity(3, row, point);
            _neighbours[2]->PreserveContinuity(2, row, 2 * point - adjacentPoint);
        } else if (_connected_side[2] == 1) {
            // SouthEast side
        } else if (_connected_side[2] == 3) {
            // SouthWest
        }
    } else {
        if (_connected_side[2] == 6) {
            // South side
            DCoordinate3 sidePoint;
            GetData(row, 3, sidePoint);
            _neighbours[2]->SetData(row, 1, 2 * sidePoint - point);
            _neighbours[2]->PreserveContinuity(row, 1, 2 * sidePoint - point);
        } else if (_connected_side[2] == 2) {
            // South side
            DCoordinate3 sidePoint;
            GetData(row, 3, sidePoint);
            _neighbours[2]->SetData(row, 2, 2 * sidePoint - point);
            _neighbours[2]->PreserveContinuity(row, 2, 2 * sidePoint - point);
        } else if (_connected_side[2] == 0) {
            // South side
            DCoordinate3 sidePoint;
            GetData(row, 3, sidePoint);
            _neighbours[2]->SetData(1, 3 - row, 2 * sidePoint - point);
            _neighbours[2]->PreserveContinuity(1, 3 - row, 2 * sidePoint - point);
        } else if (_connected_side[2] == 4) {
            // South side
            DCoordinate3 sidePoint;
            GetData(row, 3, sidePoint);
            _neighbours[2]->SetData(2, row, 2 * sidePoint - point);
            _neighbours[2]->PreserveContinuity(2, row, 2 * sidePoint - point);
        } else if (_connected_side[2] == 1) {
            // SouthEast side
        } else if (_connected_side[2] == 3) {
            // SouthWest
        }
    }
}
GLvoid SecTrigPatch3::PreserveSouth(int row, int col, DCoordinate3 point) {
    if (!_neighbours[4]) return;
    if (row == 3) {
        if (_connected_side[4] == 0) {
            // South side
            DCoordinate3 adjacentPoint;
            GetData(2, col, adjacentPoint);
            _neighbours[4]->SetData(0, col, point);
            _neighbours[4]->SetData(1, col, 2 * point - adjacentPoint);
            _neighbours[4]->PreserveContinuity(0, col, point);
            _neighbours[4]->PreserveContinuity(1, col, 2 * point - adjacentPoint);
        } else if (_connected_side[4] == 4) {
            // South side
            DCoordinate3 adjacentPoint;
            GetData(2, col, adjacentPoint);
            _neighbours[4]->SetData(3, col, point);
            _neighbours[4]->SetData(2, col, 2 * point - adjacentPoint);
            _neighbours[4]->PreserveContinuity(3, col, point);
            _neighbours[4]->PreserveContinuity(2, col, 2 * point - adjacentPoint);
        } else if (_connected_side[4] == 2) {
            // South side
            DCoordinate3 adjacentPoint;
            GetData(2, col, adjacentPoint);
            _neighbours[4]->SetData(col, 3, point);
            _neighbours[4]->SetData(col, 2, 2 * point - adjacentPoint);
            _neighbours[4]->PreserveContinuity(col, 3, point);
            _neighbours[4]->PreserveContinuity(col, 2, 2 * point - adjacentPoint);
        } else if (_connected_side[4] == 6) {
            // South side
            DCoordinate3 adjacentPoint;
            GetData(2, col, adjacentPoint);
            _neighbours[4]->SetData(3 - col, 0, point);
            _neighbours[4]->SetData(3 - col, 1, 2 * point - adjacentPoint);
            _neighbours[4]->PreserveContinuity(3 - col, 0, point);
            _neighbours[4]->PreserveContinuity(3 - col, 1, 2 * point - adjacentPoint);
        } else if (_connected_side[4] == 7) {
            // SouthEast side
        } else if (_connected_side[4] == 1) {
            // SouthWest
        }
    } else {
        if (_connected_side[4] == 0) {
            // South side
            DCoordinate3 sidePoint;
            GetData(3, col, sidePoint);
            _neighbours[4]->SetData(1, col, 2 * sidePoint - point);
            _neighbours[4]->PreserveContinuity(1, col, 2 * sidePoint - point);
        } else if (_connected_side[4] == 4) {
            // South side
            DCoordinate3 sidePoint;
            GetData(3, col, sidePoint);
            _neighbours[4]->SetData(2, col, 2 * sidePoint - point);
            _neighbours[4]->PreserveContinuity(2, col, 2 * sidePoint - point);
        } else if (_connected_side[4] == 2) {
            // South side
            DCoordinate3 sidePoint;
            GetData(3, col, sidePoint);
            _neighbours[4]->SetData(col, 2, 2 * sidePoint - point);
            _neighbours[4]->PreserveContinuity(col, 2, 2 * sidePoint - point);
        } else if (_connected_side[4] == 6) {
            // South side
            DCoordinate3 sidePoint;
            GetData(3, col, sidePoint);
            _neighbours[4]->SetData(3 - col, 1, 2 * sidePoint - point);
            _neighbours[4]->PreserveContinuity(3 - col, 1, 2 * sidePoint - point);
        } else if (_connected_side[4] == 7) {
            // SouthEast side
        } else if (_connected_side[4] == 1) {
            // SouthWest
        }
    }
}
GLvoid SecTrigPatch3::PreserveWest(int row, int col, DCoordinate3 point) {
    if (!_neighbours[6]) return;
    if (col == 0) {
        if (_connected_side[6] == 2) {
            // West side
            DCoordinate3 adjacentPoint;
            GetData(row, 1, adjacentPoint);
            _neighbours[6]->SetData(row, 3, point);
            _neighbours[6]->SetData(row, 2, 2 * point - adjacentPoint);
            _neighbours[6]->PreserveContinuity(row, 3, point);
            _neighbours[6]->PreserveContinuity(row, 2, 2 * point - adjacentPoint);
        } else if (_connected_side[6] == 6) {
            // West side
            DCoordinate3 adjacentPoint;
            GetData(row, 1, adjacentPoint);
            _neighbours[6]->SetData(row, 0, point);
            _neighbours[6]->SetData(row, 1, 2 * point - adjacentPoint);
            _neighbours[6]->PreserveContinuity(row, 0, point);
            _neighbours[6]->PreserveContinuity(row, 1, 2 * point - adjacentPoint);
        } else if (_connected_side[6] == 0) {
            // North side
            DCoordinate3 adjacentPoint;
            GetData(row, 1, adjacentPoint);
            _neighbours[6]->SetData(0, row, point);
            _neighbours[6]->SetData(1, row, 2 * point - adjacentPoint);
            _neighbours[6]->PreserveContinuity(0, row, point);
            _neighbours[6]->PreserveContinuity(1, row, 2 * point - adjacentPoint);
        } else if (_connected_side[6] == 4) {
            // West side
            DCoordinate3 adjacentPoint;
            GetData(row, 1, adjacentPoint);
            _neighbours[6]->SetData(3, 3 - row, point);
            _neighbours[6]->SetData(2, 3 - row, 2 * point - adjacentPoint);
            _neighbours[6]->PreserveContinuity(3, 3 - row, point);
            _neighbours[6]->PreserveContinuity(2, 3 - row, 2 * point - adjacentPoint);
        } else if (_connected_side[6] == 5) {
            // SouthEast side
        } else if (_connected_side[6] == 7) {
            // SouthWest
        }
    } else {
        if (_connected_side[6] == 2) {
            // South side
            DCoordinate3 sidePoint;
            GetData(row, 0, sidePoint);
            _neighbours[6]->SetData(row, 2, 2 * sidePoint - point);
            _neighbours[6]->PreserveContinuity(row, 2, 2 * sidePoint - point);
        } else if (_connected_side[6] == 6) {
            // South side
            DCoordinate3 sidePoint;
            GetData(row, 0, sidePoint);
            _neighbours[6]->SetData(row, 1, 2 * sidePoint - point);
            _neighbours[6]->PreserveContinuity(row, 1, 2 * sidePoint - point);
        } else if (_connected_side[6] == 0) {
            // South side
            DCoordinate3 sidePoint;
            GetData(row, 0, sidePoint);
            _neighbours[6]->SetData(1, row, 2 * sidePoint - point);
            _neighbours[6]->PreserveContinuity(1, row, 2 * sidePoint - point);
        } else if (_connected_side[6] == 4) {
            // South side
            DCoordinate3 sidePoint;
            GetData(row, 0, sidePoint);
            _neighbours[6]->SetData(2, 3 - row, 2 * sidePoint - point);
            _neighbours[6]->PreserveContinuity(2, 3 - row, 2 * sidePoint - point);
        } else if (_connected_side[6] == 5) {
            // SouthEast side
        } else if (_connected_side[6] == 7) {
            // SouthWest
        }
    }
}

GLvoid SecTrigPatch3::PreserveNorthEast(int row, int col, DCoordinate3 point) {

}
GLvoid SecTrigPatch3::PreserveSouthEast(int row, int col, DCoordinate3 point) {

}
GLvoid SecTrigPatch3::PreserveSouthWest(int row, int col, DCoordinate3 point) {

}
GLvoid SecTrigPatch3::PreserveNorthWest(int row, int col, DCoordinate3 point) {

}

GLvoid SecTrigPatch3::PreserveAlphaU(double u_alpha) {
    if (GetUAlpha() == u_alpha) return;
    SetUAlpha(u_alpha);
    for (int i = 0; i < 8;  i++) {
        if (_neighbours[i] == nullptr) continue;
        _neighbours[i]->PreserveAlphaU(u_alpha);
    }
}
GLvoid SecTrigPatch3::PreserveAlphaV(double v_alpha) {
    if (GetVAlpha() == v_alpha) return;
    SetVAlpha(v_alpha);
    for (int i = 0; i < 8;  i++) {
        if (_neighbours[i] == nullptr) continue;
        _neighbours[i]->PreserveAlphaV(v_alpha);
    }
}
