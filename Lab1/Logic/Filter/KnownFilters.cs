using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab1
{
    class KnownFilters
    {
        public static Filter[] get()
        {
            List<Filter> retValue = new List<Filter>();

            int t = 0;
            // blur 3x3, 5x5, 7x7
            for (int sz = 3; t < 3; t++, sz += 2)
            {
                float[,] fm = new float[sz, sz];
                float val = 1.0f / ((float)sz * sz);
                for (int i = 0; i < sz; i++)
                    for (int j = 0; j < sz; j++)
                        fm[i, j] = val;
                retValue.Add(new MatrixFilter(string.Format("Blur {0} x {0}", sz), fm));
            }

            { //Gauss blur 5x5
                const int gaussSize = 5;
                float[,] fm = new float[gaussSize, gaussSize];
                float sigma = 0.5f;
                float sum = 0;
                for (int i = -gaussSize / 2; i <= gaussSize / 2; i++)
                    for (int j = -gaussSize / 2; j <= gaussSize / 2; j++)
                    {
                        fm[i + gaussSize / 2, j + gaussSize / 2] = (float)Math.Exp(-(i * i + j * j) / (2 * sigma));
                        sum += fm[i + gaussSize / 2, j + gaussSize / 2];
                    }
                for (int i = -gaussSize / 2; i <= gaussSize / 2; i++)
                    for (int j = -gaussSize / 2; j <= gaussSize / 2; j++)
                        fm[i + gaussSize / 2, j + gaussSize / 2] /= sum;
                retValue.Add(new MatrixFilter("Gauss Blur 5 x 5", fm));
            }

            { //Contrast

                float[,] fm = new float[3, 3];
                for (int i = 0; i < 3; i++)
                    for (int j = 0; j < 3; j++)
                        fm[i, j] = -1;
                fm[1, 1] = 9;
                retValue.Add(new MatrixFilter("Contrast", fm));
            }

            { //Sobel
                float[,] fm = new float[,]{{-1,-2,-1},
                                             { 0, 0, 0},
                                             { 1, 2, 1}};
                retValue.Add(new MatrixFilter("Sobel vertical", fm));

                for (int i = 0; i < 3; i++)
                    for (int j = 0; j < i; j++)
                    {
                        float tmp = fm[i, j];
                        fm[i, j] = fm[j, i];
                        fm[j, i] = tmp;
                    }
                retValue.Add(new MatrixFilter("Sobel horizontal", fm));

            }

            { //Prewitt
                float[,] fm = new float[,]{{-1,-1,-1},
                                          { 0, 0, 0},
                                         { 1, 1, 1}};
                retValue.Add(new MatrixFilter("Prewitt vertical", fm));

                for (int i = 0; i < 3; i++)
                    for (int j = 0; j < i; j++)
                    {
                        float tmp = fm[i, j];
                        fm[i, j] = fm[j, i];
                        fm[j, i] = tmp;
                    }
                retValue.Add(new MatrixFilter("Prewitt horizontal", fm));
            }

            { //Laplass
                float[,] fm = new float[,]{{ 0, 1, 0},
                                         { 1, -4, 1},
                                         { 0, 1, 1}};
                retValue.Add(new MatrixFilter("Laplass", fm));
            }

            { //Median 3x3
                retValue.Add(new MedianFilter("Median 3 x 3", 3));
            }

            Filter[] rv = new Filter[retValue.Count];
            for (int i = 0; i < retValue.Count; i++)
                rv[i] = retValue[i];

            return rv;
        }
    }
}
