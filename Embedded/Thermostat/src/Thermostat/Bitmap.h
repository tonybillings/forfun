#ifndef __Bitmap__
#define __Bitmap__

class Bitmap
{
public:
    static RA8875*      TFT;
    static bool			UseOverrideColor;
	static uint16_t 	OverrideColor;

public:
    bool                IsSmallBitmap;
    int                 Width;
    int                 Height;
    uint16_t            TransparencyColor;

protected:
    uint16_t            LastPosX;
    uint16_t            LastPosY;
    uint16_t            StrokesArraySize;
    uint16_t            ColorsArraySize;
    const uint16_t*     Strokes;
    const uint8_t*      SmallStrokes;
    const uint16_t*     Colors;

public:
    static void draw(Bitmap* bmp, uint16_t x, uint16_t y, bool inverseColors = false)
    {
        if (TFT == nullptr) return;

        int xPos = x;
        int yPos = y;
        int xCur = 0;
        int yCur = 0;
        int rowProgress = 0;

        uint16_t stroke;
        uint8_t smallStroke;
        uint16_t stride;
        uint8_t smallStride;
        uint8_t colorIndex;
        uint16_t color;

        for (int i = 0; i < bmp->StrokesArraySize; i++)
        {

            if (bmp->IsSmallBitmap)
            {
                smallStroke = pgm_read_byte_near(bmp->SmallStrokes + i);
                colorIndex = (inverseColors) ? (bmp->ColorsArraySize - 1 - (smallStroke >> 7)) : (smallStroke >> 7);
                color = pgm_read_word_near(bmp->Colors + colorIndex);
                smallStride = (smallStroke & 127) + 1;         

                if (color != bmp->TransparencyColor) 
				    TFT->drawFastHLine(xPos + xCur, yPos + yCur, smallStride, ((Bitmap::UseOverrideColor) ? Bitmap::OverrideColor : color));

                rowProgress += smallStride;
            }
            else
            {
                stroke = pgm_read_word_near(bmp->Strokes + i);
                colorIndex = (inverseColors) ? (bmp->ColorsArraySize - 1 - (stroke >> 10)) : (stroke >> 10);
                color = pgm_read_word_near(bmp->Colors + colorIndex);
                stride = (stroke & 1023) + 1;  
                
                if (color != bmp->TransparencyColor)    
				    TFT->drawFastHLine(xPos + xCur, yPos + yCur, stride, ((Bitmap::UseOverrideColor) ? Bitmap::OverrideColor : color));

                rowProgress += stride;
            }

            if (rowProgress >= bmp->Width)
            {
                rowProgress = 0;
                xCur = 0;
                yCur++;
            }
            else
            {
                xCur += (bmp->IsSmallBitmap) ? smallStride : stride;
            }    
        }

        bmp->LastPosX = x;
        bmp->LastPosY = y;
    }

    static void clear(const Bitmap* bmp)
    {
        if (TFT == nullptr || bmp->LastPosX == -1 || bmp->LastPosY == -1) return;
        TFT->fillRect(bmp->LastPosX, bmp->LastPosY, bmp->Width, bmp->Height, 0x0000);
    }
};

RA8875* Bitmap::TFT = nullptr;
bool Bitmap::UseOverrideColor = false;
uint16_t Bitmap::OverrideColor = 0;

#endif
