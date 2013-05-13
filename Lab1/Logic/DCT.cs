using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab1
{
    public class DCT
    {
        static Dictionary<int, float[,,,]> calculatedDCTs;
        static Dictionary<int, float[,,,]> calculatedRevDCTs;

        public static float[,,,] dctForSize(int size)
        {
            if (calculatedDCTs == null)
                calculatedDCTs = new Dictionary<int, float[, , ,]>();
            float[,,,] retValue = null;
            if (calculatedDCTs.TryGetValue(size, out retValue))
                return retValue;

            retValue = new float[size, size, size, size];

            for (int u = 0; u < size; u++)
                for (int v = 0; v < size; v++)
                    for (int x = 0; x < size; x++)
                        for (int y = 0; y < size; y++)
                        {
                            retValue[u, v, x, y] = (float)(alpha(u, size) * alpha(v, size) * Math.Cos(Math.PI / size * (x + 0.5) * u) * Math.Cos(Math.PI / size * (y + 0.5) * v));
                        }
            calculatedDCTs[size] = retValue;
            return retValue;
        }

        public static float[, , ,] revDctForSize(int size)
        {
            if (calculatedRevDCTs == null)
                calculatedRevDCTs = new Dictionary<int, float[, , ,]>();

            float[, , ,] retValue = null;
            if (calculatedRevDCTs.TryGetValue(size, out retValue))
                return retValue;

            retValue = new float[size, size, size, size];

            for (int x = 0; x < size; x++)
                for (int y = 0; y < size; y++)
                    for (int u = 0; u < size; u++)
                        for (int v = 0; v < size; v++)
                        {
                            retValue[x, y, u, v] = (float)(alpha(u, size) * alpha(v, size) * Math.Cos(Math.PI / size * (x + 0.5) * u) * Math.Cos(Math.PI / size * (y + 0.5) * v));
                        }
            calculatedRevDCTs[size] = retValue;
            return retValue;
        }

        static float alpha(int u, int size)
        {
            if (u == 0)
                return (float)(1 / Math.Sqrt(size));
            else
                return (float)(Math.Sqrt(2 / (double)size));
        }
    }
}
