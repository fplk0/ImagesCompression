using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing.Imaging;

namespace Lab1
{
    public static class ColorMatrixExtensions
    {
        public static ColorMatrix multiply(this ColorMatrix mx1, ColorMatrix other)
        {
            ColorMatrix retValue = new ColorMatrix();

            for (int i = 0; i < 5; i++)
                for (int j = 0; j < 5; j++)
                {
                    retValue[i, j] = 0;
                    for (int k = 0; k < 5; k++)
                    {
                        retValue[i, j] += mx1[i, k] * other[k, j];
                    }
                }
            return retValue;
        }
    }
}
