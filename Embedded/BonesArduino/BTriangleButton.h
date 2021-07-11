#ifndef __bones_triangle_button__
#define __bones_triangle_button__

#include "BButton.h"
#include "BonesCommon.h"

class BTriangleButton : public BButton
{
public:
	BTriangle	Triangle;
 
public:
    BTriangleButton(TFT_TYPE_PTR tft = 0);
    BTriangleButton(const BTriangleButton& other);
    ~BTriangleButton();
 
public:
    virtual void Initialize(TFT_TYPE_PTR tft);
    virtual void Draw();
};

#endif