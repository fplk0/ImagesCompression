using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab1
{
    public abstract class Filter
    {
        string _name;

        public string name
        {
            get { return _name; }
            protected set { _name = value; }
        }

        public Filter(string name)
        {
            this.name = name;
        }

        public float[, ,] applyToPixelMatrix(float[, ,] pixelMatrix)
        {
            int[] channelIndexes = new int[pixelMatrix.GetLength(2)];
            for (int i = 0; i < channelIndexes.Length; i++)
                channelIndexes[i] = i;
            return this.applyToPixelMatrix(pixelMatrix, channelIndexes);
        }

        public abstract float[, ,] applyToPixelMatrix(float[, ,] pixelMatrix, int[] channelIndexes);
    }
}
