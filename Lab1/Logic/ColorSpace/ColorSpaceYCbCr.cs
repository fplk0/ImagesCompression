using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab1
{
    public class ColorSpaceYCbCr : ColorSpace
    {
        //float _Kb;
        //float _Kr;

        public ColorSpaceYCbCr()
        {
            this.name = "YCbCr";
            this.componentsCount = 3;
            //_Kb = 0.114f;
            //_Kr = 0.299f;
        }

        public override string nameOfComponentAtIndex(int componentIndex)
        {
            switch (componentIndex)
            {
                case 0: return "Y";
                case 1: return "Cb";
                case 2: return "Cr";
            }

            return null;
        }

        override public float[] convertColorFromRGB(float[] originalColor)
        {
            float y = 0, cb = 0, cr = 0;
            this._YCbCrFromRGBComponents(originalColor[0], originalColor[1], originalColor[2], ref y, ref cb, ref cr);
            float[] retValue = {y, cb, cr};
            return retValue;
        }

        override public float[] convertColorToRGB(float[] color)
        {
            float r = 0, g = 0, b = 0;
            this._rgbFromYCbCrComponents(color[0], color[1], color[2], ref r, ref g, ref b);
            float[] retValue = new float[3]{r, g, b};
            return color;
        }

        override public float[, ,] convertColorMatrixFromRGB(float[, ,] rgbComponents)
        {
            float[, ,] retValue = new float[rgbComponents.GetLength(0), rgbComponents.GetLength(1), 3];

            for (int i = 0, iEnd = rgbComponents.GetLength(0); i < iEnd; i++)
                for (int j = 0, jEnd = rgbComponents.GetLength(1); j < jEnd; j++)
                {
                    float y = 0, cb = 0, cr = 0;
                    this._YCbCrFromRGBComponents(rgbComponents[i, j, 0], rgbComponents[i, j, 1], rgbComponents[i, j, 2], ref y, ref cb, ref cr);
                    retValue[i, j, 0] = y;
                    retValue[i, j, 1] = cb;
                    retValue[i, j, 2] = cr;
                }

            return retValue;
        }

        override public float[, ,] convertColorMatrixToRGB(float[, ,] components)
        {
            float[, ,] retValue = new float[components.GetLength(0), components.GetLength(1), 3];

            for (int i = 0, iEnd = components.GetLength(0); i < iEnd; i++)
                for (int j = 0, jEnd = components.GetLength(1); j < jEnd; j++)
                {
                    float r = 0, g = 0, b = 0;
                    this._rgbFromYCbCrComponents(components[i, j, 0], components[i, j, 1], components[i, j, 2], ref r, ref g, ref b);
                    retValue[i, j, 0] = r;
                    retValue[i, j, 1] = g;
                    retValue[i, j, 2] = b;
                }

            return retValue;
        }

        override public float lowerBoundForComponent(int componentIndex)
        {
            return 0;
        }

        override public float upperBoundForComponent(int componentIndex)
        {
            return 1;
        }

        override public float neutralValueForComponent(int componentIndex)
        {
            switch (componentIndex)
            {
                case 0: return 0.0f;
                case 1:
                case 2: return 0.5f;
            }
            return base.neutralValueForComponent(componentIndex);
        }

        [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)]

        void _YCbCrFromRGBComponents(float r, float g, float b, ref float y, ref float cb, ref float cr)
        {
            y = 0.299f * r + 0.587f * g + 0.114f * b;
            cb = 0.5f - 0.168736f * r - 0.331264f * g + 0.5f * b;
            cr = 0.5f + 0.5f * r - 0.418688f * g - 0.081312f * b;
        }

        [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)]

        void _rgbFromYCbCrComponents(float y, float cb, float cr, ref float r, ref float g, ref float b)
        {
            r = y + 1.402f * (cr - 0.5f);
            if (r > 1)
                r = 1;
            g = y - 0.34414f * (cb - 0.5f) - 0.71414f * (cr - 0.5f);
            if (g > 1)
                g = 1;
            b = y + 1.772f * (cb - 0.5f);
            if (b > 1)
                b = 1;
        }
    }
}
