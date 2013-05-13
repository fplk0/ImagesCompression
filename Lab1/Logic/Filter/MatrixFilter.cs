using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab1
{
    public class MatrixFilter : Filter
    {
        float[,] filterMatrix;

        public MatrixFilter(string name, float[,] _filterMatrix) : base(name)
        {
            filterMatrix = (float[,])_filterMatrix.Clone();
        }

        public override float[, ,] applyToPixelMatrix(float[, ,] pixelMatrix, int[] channelIndexes)
        {
            int width = pixelMatrix.GetLength(0);
            int height = pixelMatrix.GetLength(1);
            int fmWidth = filterMatrix.GetLength(1);
            int fmHeight = filterMatrix.GetLength(0);
            int hfmWidth = fmWidth / 2;
            int hfmHeight = fmHeight / 2;

            float[, ,] retValue = new float[pixelMatrix.GetLength(0), pixelMatrix.GetLength(1), pixelMatrix.GetLength(2)];

            for (int i = 0; i < width; i++)
                for (int j = 0; j < height; j++)
                    for (int ci = 0; ci < channelIndexes.Length; ci++)
                    {
                        float componentValue = 0.0f;
                        for (int di = 0; di < fmWidth; di++)
                        {
                            int curCol = i + di - hfmWidth;
                            if (curCol < 0 || curCol >= width)
                                continue;
                            for (int dj = 0; dj < fmHeight; dj++)
                            {
                                int curRow = j + dj - hfmHeight;
                                if (curRow < 0 || curRow >= height)
                                    continue;
                                componentValue += pixelMatrix[curCol, curRow, ci] * filterMatrix[dj, di];
                            }
                        }
                        if (componentValue < 0.0f)
                            componentValue = 0.0f;
                        if (componentValue > 1.0f)
                            componentValue = 1.0f;
                        retValue[i, j, ci] = componentValue;
                    }

            return retValue;
        }
    }
}
