#ifndef __cr_triangle_button__
#define __cr_triangle_button__

#include "cr_button.h"
#include "cr_common.h"

class CRTriangleButton : public CRButton
{
public:
	CRTriangle	Triangle;
 
public:
    CRTriangleButton(TFT_TYPE_PTR tft = 0);
    CRTriangleButton(const CRTriangleButton& other);
    ~CRTriangleButton();
 
public:
    virtual void Initialize(TFT_TYPE_PTR tft);
    virtual void Draw();
};

#endif