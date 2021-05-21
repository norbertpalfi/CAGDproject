#ifndef SECONDORDERTRIGONOMETRICPATCH_H
#define SECONDORDERTRIGONOMETRICPATCH_H
#include <Core/TensorProductSurfaces3.h>
#include "../Core/Constants.h"

namespace cagd
{
    // 1. @TODO store the uip and vip, material and texture
    // 2. @TODO make the patches selectable
    // 3. @TODO Extend patches in the remaining 4 directions
    // 4. @TODO Test, test, test joining
    // 5. @TODO Figure out continuity
    enum Variable{U, V};

    // Directions
    // 1. N
    // 2. NE
    // 3. E
    // 4. SE
    // 5. S
    // 6. SW
    // 7. W
    // 8. NW

    class SecTrigPatch3: public TensorProductSurface3
    {
    private:
        GLdouble _alpha[2];
        GLdouble _translatedX;
        GLdouble _translatedY;
        GLdouble _translatedZ;

        GLboolean _has_neighbours;


        GLboolean _updated;

        SecTrigPatch3*      _neighbours[8];
        int                 _connected_side[8];

        TriangulatedMesh3*  _image;

        RowMatrix<GenericCurve3*>* _uip;
        RowMatrix<GenericCurve3*>* _vip;

        GLdouble f0(GLdouble alpha, GLdouble t) const;
        GLdouble f1(GLdouble alpha, GLdouble t) const;
        GLdouble f2(GLdouble alpha, GLdouble t) const;
        GLdouble f3(GLdouble alpha, GLdouble t) const;

        GLdouble f0_1(GLdouble alpha, GLdouble t) const;
        GLdouble f1_1(GLdouble alpha, GLdouble t) const;
        GLdouble f2_1(GLdouble alpha, GLdouble t) const;
        GLdouble f3_1(GLdouble alpha, GLdouble t) const;

        GLdouble f0_2(GLdouble alpha, GLdouble t) const;
        GLdouble f1_2(GLdouble alpha, GLdouble t) const;
        GLdouble f2_2(GLdouble alpha, GLdouble t) const;
        GLdouble f3_2(GLdouble alpha, GLdouble t) const;

        GLboolean ExtendNorth(SecTrigPatch3 &patch);
        GLboolean ExtendEast(SecTrigPatch3 &patch);
        GLboolean ExtendSouth(SecTrigPatch3 &patch);
        GLboolean ExtendWest(SecTrigPatch3 &patch);

        GLboolean ExtendNorthEast(SecTrigPatch3 &patch);
        GLboolean ExtendSouthEast(SecTrigPatch3 &patch);
        GLboolean ExtendSouthWest(SecTrigPatch3 &patch);
        GLboolean ExtendNorthWest(SecTrigPatch3 &patch);

        GLboolean JoinEastWest(SecTrigPatch3 &patch, int side1, int side2, SecTrigPatch3 &result);
        GLboolean JoinNorthSouth(SecTrigPatch3 &patch, int side1, int side2, SecTrigPatch3 &result);

        GLboolean JoinEastEast(SecTrigPatch3 &patch, SecTrigPatch3 &result);
        GLboolean JoinWestWest(SecTrigPatch3 &patch, SecTrigPatch3 &result);

        GLboolean JoinSouthSouth(SecTrigPatch3 &patch, SecTrigPatch3 &result);
        GLboolean JoinNorthNorth(SecTrigPatch3 &patch, SecTrigPatch3 &result);

        GLboolean JoinNorthEast(SecTrigPatch3 &patch, SecTrigPatch3 &result);
        GLboolean JoinNorthWest(SecTrigPatch3 &patch, SecTrigPatch3 &result);

        GLboolean JoinSouthWest(SecTrigPatch3 &patch, SecTrigPatch3 &result);
        GLboolean JoinSouthEast(SecTrigPatch3 &patch, SecTrigPatch3 &result);

        GLboolean MergeEastWest(SecTrigPatch3 &patch, int side1, int side2);
        GLboolean MergeNorthSouth(SecTrigPatch3 &patch, int side1, int side2);

        GLboolean MergeEastEast(SecTrigPatch3 &patch);
        GLboolean MergeWestWest(SecTrigPatch3 &patch);

        GLboolean MergeNorthNorth(SecTrigPatch3 &patch);
        GLboolean MergeSouthSouth(SecTrigPatch3 &patch);

        GLboolean MergeNorthWest(SecTrigPatch3 &patch);
        GLboolean MergeNorthEast(SecTrigPatch3 &patch);
        GLboolean MergeSouthWest(SecTrigPatch3 &patch);
        GLboolean MergeSouthEast(SecTrigPatch3 &patch);

        GLvoid    PreserveNorth(int row, int col, DCoordinate3 point);
        GLvoid    PreserveEast(int row, int col, DCoordinate3 point);
        GLvoid    PreserveSouth(int row, int col, DCoordinate3 point);
        GLvoid    PreserveWest(int row, int col, DCoordinate3 point);

        GLvoid    PreserveNorthEast(int row, int col, DCoordinate3 point);
        GLvoid    PreserveSouthEast(int row, int col, DCoordinate3 point);
        GLvoid    PreserveSouthWest(int row, int col, DCoordinate3 point);
        GLvoid    PreserveNorthWest(int row, int col, DCoordinate3 point);

        GLvoid    PreserveAlphaU(double u_alpha);
        GLvoid    PreserveAlphaV(double v_alpha);

    public:
        SecTrigPatch3(GLdouble u_alpha = PI / 2.0, GLdouble v_alpha = 1.0);

        // inherited pure virtual methods have to be redeclared and defined
        GLboolean UBlendingFunctionValues(GLdouble u, RowMatrix<GLdouble> &blending_values) const;
        GLboolean VBlendingFunctionValues(GLdouble v, RowMatrix<GLdouble> &blending_values) const;
        GLboolean CalculatePartialDerivatives(
                        GLuint maximum_order_of_partial_derivatives,
                        GLdouble u, GLdouble v, PartialDerivatives &pd) const;

        GLboolean   SetUAlpha(GLdouble alpha);
        GLdouble    GetUAlpha();

        GLboolean   SetVAlpha(GLdouble alpha);
        GLdouble    GetVAlpha();

        GLboolean   translateX(double value);
        GLboolean   translateY(double value);
        GLboolean   translateZ(double value);

        GLboolean   SetNeighbour(int neighbour, int side, SecTrigPatch3 *patch);

        double      GetTranslatedX();
        double      GetTranslatedY();
        double      GetTranslatedZ();

        GLvoid      SetImage(TriangulatedMesh3* image);
        TriangulatedMesh3* GetImage();

        GLboolean   ExtendPatch(int dir, SecTrigPatch3 &patch);
        GLboolean   JoinPatch(SecTrigPatch3 &patch, int side1, int side2, SecTrigPatch3 &result);
        GLboolean   MergePatch(SecTrigPatch3 &patch, int side1, int side2);

        GLvoid      PreserveContinuity(int row, int col, DCoordinate3 point);

        GLboolean   SetUip(RowMatrix<GenericCurve3*>* uip);
        GLboolean   SetVip(RowMatrix<GenericCurve3*>* vip);

        RowMatrix<GenericCurve3*>*      GetUip();
        RowMatrix<GenericCurve3*>*      GetVip();

    };
}
#endif // SECONDORDERTRIGONOMETRICPATCH_H
