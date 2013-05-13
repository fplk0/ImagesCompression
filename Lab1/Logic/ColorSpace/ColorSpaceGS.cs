using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab1
{
    public class ColorSpaceGS : ColorSpace
    {
        float _r, _g, _b;
        public ColorSpaceGS()
        {
            this.name = "Grayscale";
            this.componentsCount = 1;
            _r = 0.333f;
            _g = 0.334f;
            _b = 0.333f;
        }

        public override string nameOfComponentAtIndex(int componentIndex)
        {
            switch (componentIndex)
            {
                case 0: return "Gs";
            }

            return null;
        }

        override public float[] convertColorFromRGB(float[] originalColor)
        {
            float gs = 0;
            this._gsFromRGBComponents(originalColor[0], originalColor[1], originalColor[2], ref gs);
            float[] retValue = {gs};
            return retValue;
        }

        override public float[] convertColorToRGB(float[] color)
        {
            float r = 0, g = 0, b = 0;
            this._rgbFromGSComponents(color[0], ref r, ref g, ref b);
            float[] retValue = new float[3]{r, g, b};
            return color;
        }

        override public float[, ,] convertColorMatrixFromRGB(float[, ,] rgbComponents)
        {
            float[, ,] retValue = new float[rgbComponents.GetLength(0), rgbComponents.GetLength(1), 1];

            for (int i = 0, iEnd = rgbComponents.GetLength(0); i < iEnd; i++)
                for (int j = 0, jEnd = rgbComponents.GetLength(1); j < jEnd; j++)
                {
                    float gs = 0;
                    this._gsFromRGBComponents(rgbComponents[i, j, 0], rgbComponents[i, j, 1], rgbComponents[i, j, 2], ref gs);
                    retValue[i, j, 0] = gs;
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
                    this._rgbFromGSComponents(components[i, j, 0], ref r, ref g, ref b);
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

        [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)]

        void _gsFromRGBComponents(float r, float g, float b, ref float gs)
        {
            gs = r * _r + g * _g + b * _b;
        }

        [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)]

        void _rgbFromGSComponents(float gs, ref float r, ref float g, ref float b)
        {
            r = gs;
            g = gs;
            b = gs;
        }
    }
}
