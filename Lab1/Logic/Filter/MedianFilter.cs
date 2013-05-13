using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab1
{
    public class MedianFilter : Filter
    {
        int size;

        public MedianFilter(string name, int _size)
            : base(name)
        {
            size = _size;
        }

        public override float[, ,] applyToPixelMatrix(float[, ,] pixelMatrix, int[] channelIndexes)
        {
            int width = pixelMatrix.GetLength(0);
            int height = pixelMatrix.GetLength(1);
            int hfmWidth = size / 2;
            int hfmHeight = size / 2;
            int medianIndex = size * size / 2;

            float[, ,] retValue = new float[pixelMatrix.GetLength(0), pixelMatrix.GetLength(1), pixelMatrix.GetLength(2)];

            for (int i = 0; i < width; i++)
                for (int j = 0; j < height; j++)
                    for (int ci = 0; ci < channelIndexes.Length; ci++)
                    {
                        float[] values = new float[size * size];
                        int cnt = 0;

                        for (int di = 0; di < size; di++)
                        {
                            int curCol = i + di - hfmWidth;
                            if (curCol < 0 || curCol >= width)
                            {
                                values[cnt++] = 0.0f;
                                continue;
                            }
                            for (int dj = 0; dj < size; dj++)
                            {
                                int curRow = j + dj - hfmHeight;
                                if (curRow < 0 || curRow >= height)
                                {
                                    values[cnt++] = 0.0f;
                                    continue;
                                }
                                values[cnt++] = pixelMatrix[curCol, curRow, ci];
                            }
                        }
                        Array.Sort(values);

                        retValue[i, j, ci] = values[medianIndex];
                    }
            return retValue;
        }
    }
}
