using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab1
{
    public class ColorSpaceRGB : ColorSpace
    {
        public ColorSpaceRGB()
        {
            this.name = "RGB";
            this.componentsCount = 3;
        }

        override public string nameOfComponentAtIndex(int componentIndex)
        {
            return new string("RGB"[componentIndex], 1);
        }

        override public float[] convertColorFromRGB(float[] originalColor)
        {
            return originalColor;
        }

        override public float[] convertColorToRGB(float[] color)
        {
            return color;
        }

        override public float[, ,] convertColorMatrixFromRGB(float[, ,] RGBComponents)
        {
            return RGBComponents;
        }

        override public float[, ,] convertColorMatrixToRGB(float[, ,] currentColorSpaceComponents)
        {
            return currentColorSpaceComponents;
        }

        override public float lowerBoundForComponent(int componentIndex)
        {
            return 0;
        }

        override public float upperBoundForComponent(int componentIndex)
        {
            return 1;
        }
    }
}
