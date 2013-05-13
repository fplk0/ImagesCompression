using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab1
{
    public abstract class Wavelet
    {
        int _waveLength;
        public int waveLength
        {
            get { return _waveLength; }
            protected set { _waveLength = value; }
        }

        float[] _coeffs;
        public float[] coeffs
        {
            get { return _coeffs; }
            protected set { _coeffs = value; }
        }

        float[] _scales;
        public float[] scales
        {
            get { return _scales; }
            set { _scales = value; }
        }

        public void forward(float[] src, float[] dst, int len)
        {
            int k = 0;
            int h = len >> 1;

            for (int i = 0; i < h; i++)
            {
                for (int j = 0; j < _waveLength; j++)
                {
                    k = (i << 1) + j;
                    while (k >= len)
                        k -= len;

                    dst[i] += src[k] * _scales[j]; // low pass filter - energy (approximation)
                    dst[i + h] += src[k] * _coeffs[j]; // high pass filter - details
                } // wavelet
            } // h
        }

        public void reverse(float[] src, float[] dst, int len)
        {
            int k = 0;
            int h = len >> 1;

            for (int i = 0; i < h; i++)
            {
                for (int j = 0; j < _waveLength; j++)
                {
                    k = (i << 1) + j;
                    while (k >= len)
                        k -= len;

                    dst[k] += (src[i] * _scales[j] + src[i + h] * _coeffs[j]); // adding up details times energy (approximation)
                } // wavelet
            } //  h
        }
    }

    public class Haar : Wavelet
    {
        public Haar()
        {
            waveLength = 2;

            coeffs = new float[waveLength];
            float sqrt2 = (float)Math.Sqrt(2);
            coeffs[0] = 1.0f / sqrt2; // w0 - normed by sqrt( 2 )
            coeffs[1] = -1.0f / sqrt2; // w1 - normed by sqrt( 2 )

            scales = new float[waveLength];

            scales[0] = -coeffs[1]; // -w1
            scales[1] = coeffs[0]; // w0

        }
    }
}
