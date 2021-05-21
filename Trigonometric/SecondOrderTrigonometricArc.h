#ifndef SOMEARCS3_H
#define SOMEARCS3_H

#include <Core/Constants.h>
#include <Core/LinearCombination3.h>

namespace cagd
{
    class SecondOrderTrigArc: public LinearCombination3
    {
    private:
        GLdouble _alpha = PI / 2.0; // possible shape parameter (if does not exist, delete this line)

        GLdouble f0(GLdouble t) const;
        GLdouble f1(GLdouble t) const;
        GLdouble f2(GLdouble t) const;
        GLdouble f3(GLdouble t) const;

        GLdouble f0_1(GLdouble t) const;
        GLdouble f1_1(GLdouble t) const;
        GLdouble f2_1(GLdouble t) const;
        GLdouble f3_1(GLdouble t) const;

        GLdouble f0_2(GLdouble t) const;
        GLdouble f1_2(GLdouble t) const;
        GLdouble f2_2(GLdouble t) const;
        GLdouble f3_2(GLdouble t) const;

        GenericCurve3* _image;


    public:
        // Variables for the interaction with curves
        SecondOrderTrigArc *left, *right;
        GLuint image_index;

        // special/default constructor
        SecondOrderTrigArc(GLdouble alpha = PI / 2.0, GLenum data_usage_flag = GL_STATIC_DRAW);

        // inherited pure virtual methods have to be redeclared and defined
        GLboolean BlendingFunctionValues(GLdouble u, RowMatrix<GLdouble> &values) const;
        GLboolean CalculateDerivatives(GLuint max_order_of_derivatives, GLdouble u, Derivatives& d) const;

        // project-dependent setters/getters, e.g.,

        void setImage(GenericCurve3* _image);
        GenericCurve3* getImage();

        // if exists, you will need to set/get the shape/tension parameter of the basis/blending functions
        GLboolean SetAlpha(GLdouble alpha);
        GLdouble GetAlpha() const;

        DCoordinate3& getPoint(GLuint index);
        void setPoint(GLuint index, GLdouble x, GLdouble y, GLdouble z);
        void setPoint(GLuint index, const DCoordinate3 &position);

        //Functions for manipulating arcs
        void translateX(GLdouble value);
        void translateY(GLdouble value);
        void translateZ(GLdouble value);

        SecondOrderTrigArc* extendLeft(); //Continue
        SecondOrderTrigArc* extendRight(); //Continue

        SecondOrderTrigArc* joinLeftLeft(SecondOrderTrigArc *arc);
        SecondOrderTrigArc* joinLeftRight(SecondOrderTrigArc *arc);
        SecondOrderTrigArc* joinRightLeft(SecondOrderTrigArc *arc);
        SecondOrderTrigArc* joinRightRight(SecondOrderTrigArc *arc);

        void mergeLeftLeft(SecondOrderTrigArc *arc);
        void mergeLeftRight(SecondOrderTrigArc *arc);
        void mergeRightLeft(SecondOrderTrigArc *arc);
        void mergeRightRight(SecondOrderTrigArc *arc);

        SecondOrderTrigArc* preserveContinuity(GLuint index,int axis,GLdouble value);

        void PreserveAlpha(double alpha);
    };
}

#endif // SOMEARCS3_H
