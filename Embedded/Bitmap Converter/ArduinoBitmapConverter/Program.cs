using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.Globalization;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;

namespace ArduinoBitmapConverter
{
    class Program
    {
        static readonly string _help = @"Arduino Bitmap Converter
USAGE: abc.exe <input_bitmap> <output_file> [transparency_color]

EXAMPLES: 
abc C:\MyFolder\CompanyLogo.bmp CompanyLogo
abc ""C:\My Folder\Company Logo.bmp"" CompanyLogo
abc ""C:\My Folder\Company Logo.bmp"" C:\Temp\CompanyLogo
abc ""C:\My Folder\Company Logo.bmp"" C:\Temp\CompanyLogo 9673FF

NOTES: The third argument is optional and is the color value (in hex format) that should NOT be drawn.  If omitted, this argument defaults to FF00FF (magenta).
";

        static readonly string _bmpTemplateBaseClass = @"#ifndef __Bitmap__
#define __Bitmap__

class Bitmap
{
public:
    static RA8875*      TFT;

public:
    int                 Width;
    int                 Height;
    uint16_t            TransparencyColor;

protected:
    uint16_t            LastPosX;
    uint16_t            LastPosY;
    uint16_t            StrokesArraySize;
    uint16_t            ColorsArraySize;
    const uint16_t*     Strokes;
    const uint16_t*     Colors;

public:
    static void draw(Bitmap& bmp, uint16_t x, uint16_t y, bool inverseColors = false)
    {
        if (TFT == nullptr) return;

        int xPos = x;
        int yPos = y;
        int xCur = 0;
        int yCur = 0;
        int rowProgress = 0;

        for (int i = 0; i < bmp.StrokesArraySize; i++)
        {
            uint16_t stroke = pgm_read_word_near(bmp.Strokes + i);
            uint8_t colorIndex = (inverseColors) ? (bmp.ColorsArraySize - 1 - (stroke >> 10)) : (stroke >> 10);
            uint16_t color = pgm_read_word_near(bmp.Colors + colorIndex);
            uint16_t stride = (stroke & 1023) + 1;

            if (color != bmp.TransparencyColor) 
				TFT->drawFastHLine(xPos + xCur, yPos + yCur, stride, color);

            rowProgress += stride;

            if (rowProgress >= bmp.Width)
            {
                rowProgress = 0;
                xCur = 0;
                yCur++;
            }
            else
            {
                xCur += stride;
            }    
        }

        bmp.LastPosX = x;
        bmp.LastPosY = y;
    }

    static void clear(const Bitmap& bmp)
    {
        if (TFT == nullptr || bmp.LastPosX == -1 || bmp.LastPosY == -1) return;
        TFT->fillRect(bmp.LastPosX, bmp.LastPosY, bmp.Width, bmp.Height, 0x0000);
    }
};

RA8875* Bitmap::TFT = nullptr;

#endif
";

        static string _bmpTemplateSubClass = @"#include ""Bitmap.h""

class @NAME@ : public Bitmap
{
public:
	@NAME@()
	{
		Width = @WIDTH@;
        Height = @HEIGHT@;
        TransparencyColor = @T_COLOR@;
        StrokesArraySize = sizeof(@NAME@_strokes) / sizeof(@NAME@_strokes[0]);
        ColorsArraySize = sizeof(@NAME@_colors) / sizeof(@NAME@_colors[0]);
		Strokes = @NAME@_strokes;
		Colors = @NAME@_colors;
        LastPosX = LastPosY = -1;
	}
};
";

        static ushort ConvertColor(uint arg)
        {
            return (ushort)(((((byte)((arg & 0xFF0000) >> 16)) & 0xf8) << 8) + ((((byte)((arg & 0xFF00) >> 8)) & 0xfc) << 3) + (((byte)(arg & 0xFF)) >> 3));
        }

        static void Main(string[] args)
        {

#if !DEBUG
            if (args.Length != 2 && args.Length != 3)
            {
                Console.WriteLine(_help);
                return;
            }
#endif

            string inFileName = null;

#if DEBUG
            inFileName = "C:\\Temp\\bitmap.bmp";
#else
            inFileName = args[0];
#endif

            Bitmap bmp = new Bitmap(inFileName);
            Rectangle rect = new Rectangle(0, 0, bmp.Width, bmp.Height);
            BitmapData bmpData = bmp.LockBits(rect, ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
            IntPtr ptr = bmpData.Scan0;

            int bytes = bmpData.Stride * bmp.Height;
            byte[] rgbValues = new byte[bytes];
            byte[] r = new byte[bytes / 3];
            byte[] g = new byte[bytes / 3];
            byte[] b = new byte[bytes / 3];

            Marshal.Copy(ptr, rgbValues, 0, bytes);

            int count = 0;
            int stride = bmpData.Stride;
            int index = 0;

            for (int row = 0; row < bmpData.Height; row++)
            {
                for (int column = 0; column < bmpData.Width; column++)
                {
                    index = (row * stride) + (column * 3);
                    b[count] = rgbValues[index];
                    g[count] = rgbValues[index + 1];
                    r[count++] = rgbValues[index + 2];
                }
            }

            string name = null;
#if DEBUG
            name = "Bitmap";
#else
            string filename = args[1];
            FileInfo info = new FileInfo(filename);
            name = (string.IsNullOrEmpty(info.Extension)) ? info.Name : info.Name.Replace(info.Extension, string.Empty);
#endif

            using (StreamWriter sw = new StreamWriter(Path.Combine(info.Directory.FullName, "Bitmap.h")))
            {
                sw.Write(_bmpTemplateBaseClass);
            }

            StringBuilder sbMain = new StringBuilder();
            StringBuilder sbStrokes = new StringBuilder();
            StringBuilder sbColors = new StringBuilder();

            sbMain.AppendLine($"#ifndef __{name}__");
            sbMain.AppendLine($"#define __{name}__" + Environment.NewLine);
            sbMain.AppendLine("#include <avr/pgmspace.h>");
            sbMain.AppendLine("#include \"RA8875.h\"");

            sbStrokes.AppendLine($"PROGMEM static const uint16_t {name}_strokes[] = {"{"}");
            sbColors.AppendLine($"PROGMEM static const uint16_t {name}_colors[] = {"{"}");

            byte[] colorBytes = new byte[4];
            ushort currentColor = 0;
            ushort currentStride = 0;
            int pixelCount = bmpData.Width * bmpData.Height;

            colorBytes[0] = b[0];
            colorBytes[1] = g[0];
            colorBytes[2] = r[0];

            currentColor = ConvertColor(BitConverter.ToUInt32(colorBytes, 0));
            currentStride = 0;

            List<ushort> colors = new List<ushort>();
            int indexOfLastStroke = 0;

            for (int i = 1; i < pixelCount; i++)
            {
                colorBytes[0] = b[i];
                colorBytes[1] = g[i];
                colorBytes[2] = r[i];

                ushort color = ConvertColor(BitConverter.ToUInt32(colorBytes, 0));

                if (!colors.Contains(color))
                {
                    colors.Add(color);
                    sbColors.AppendLine($"{color},");
                }

                if (color == currentColor && i % bmpData.Width != 0)
                {
                    currentStride++;
                }
                else
                {

                    ushort colorIndex = (ushort)(colors.IndexOf(currentColor) << 10);
                    sbStrokes.AppendLine($"{colorIndex | currentStride}{((i == pixelCount - 1) ? string.Empty : ",")}");

                    currentColor = color;
                    currentStride = 0;
                    indexOfLastStroke = i;
                }
            }

            if (indexOfLastStroke != pixelCount - 1)
                sbStrokes.AppendLine($"{(ushort)(colors.IndexOf(currentColor) << 10) | currentStride}");

            sbStrokes.AppendLine("};");

            string tmp = sbColors.ToString().TrimEnd("\r\n,".ToCharArray());
            sbColors.Clear();
            sbColors.AppendLine(tmp);
            sbColors.AppendLine("};");

            uint transparencyColor = 0xFF00FF;
            if (args.Length > 2)
            {
                transparencyColor = uint.Parse(args[2], NumberStyles.HexNumber);
            }

            string logoTemplate = _bmpTemplateSubClass
                .Replace("@NAME@", name)
                .Replace("@WIDTH@", bmpData.Width.ToString())
                .Replace("@HEIGHT@", bmpData.Height.ToString())
#if DEBUG
                .Replace("@T_COLOR@", ConvertColor(0xFF00FF).ToString());
#else
                .Replace("@T_COLOR@", ConvertColor(transparencyColor).ToString());
#endif

            string outFileName = null;
#if DEBUG
            outFileName = "C:\\Temp\\bitmap.h";
#else
            outFileName = args[1];
            if (!outFileName.EndsWith(".h")) outFileName += ".h";
#endif

            using (StreamWriter sw = new StreamWriter(outFileName))
            {
                sw.WriteLine(sbMain.ToString());
                sw.WriteLine(sbStrokes.ToString());
                sw.WriteLine(sbColors.ToString());

                sbMain.Clear();
                sbMain.AppendLine(logoTemplate + Environment.NewLine + "#endif");
                sw.Write(sbMain.ToString());
            }

            Console.WriteLine("Done!");
        }
    }
}
