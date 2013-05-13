using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing.Imaging;
using System.Drawing;

namespace Lab1
{
    public abstract class ColorSpace
    {
        protected int _componentsCount;
        public int componentsCount
        {
            get { return _componentsCount; }
            protected set { _componentsCount = value; }
        }

        protected string _name;
        public string name
        {
            get { return _name; }
            protected set { _name = value; }
        }

        abstract public string nameOfComponentAtIndex(int componentIndex);
        abstract public float[] convertColorFromRGB(float[] originalColor);
        abstract public float[] convertColorToRGB(float[] color);
        abstract public float[, ,] convertColorMatrixFromRGB(float[, ,] RGBComponents);
        abstract public float[, ,] convertColorMatrixToRGB(float[, ,] currentColorSpaceComponents);
        abstract public float lowerBoundForComponent(int componentIndex);
        abstract public float upperBoundForComponent(int componentIndex);
        virtual public float neutralValueForComponent(int componentIndex)
        {
            return 0.0f;
        }
    }
}
