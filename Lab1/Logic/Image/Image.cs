using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab1
{
    public class Image
    {
        float[, ,] imagePixels;
        ColorSpace _colorSpace;

        int _originalWidth, _originalHeight;

        public float getPixel(int x, int y, int dim)
        {
            return imagePixels[y, x, dim];
        }

        public void setPixel(int x, int y, int dim, float value)
        {
            imagePixels[y, x, dim] = value;
        }

        public int width
        {
            get { return imagePixels.GetLength(1); }
        }

        public int height
        {
            get { return imagePixels.GetLength(0); }
        }

        public ColorSpace colorSpace
        {
            get { return _colorSpace; }
            protected set { _colorSpace = value; }
        }

        public Image(Image otherImage)
        {
            this.colorSpace = otherImage.colorSpace;
            imagePixels = (float[,,])otherImage.imagePixels.Clone();
        }

        public Image(int height, int width, ColorSpace colorSpace)
        {
            imagePixels = new float[height, width, colorSpace.componentsCount];
            _colorSpace = colorSpace;
        }

        public Image(float[, ,] pixels, ColorSpace colorSpace)
        {
            if (pixels.GetLength(2) != colorSpace.componentsCount)
            {
                return;
            }
            imagePixels = (float[, ,])pixels.Clone();
            _colorSpace = colorSpace;
        }

        unsafe public Image(System.Drawing.Image img)
            : this(img.Height, img.Width, new ColorSpaceRGB())
        {
            Type t = img.GetType();
            System.Drawing.Bitmap bmp = null;
            if (t.Equals(typeof(Bitmap)))
                bmp = (Bitmap)img;
            else
                bmp = new Bitmap(img);

            System.Drawing.Imaging.BitmapData bmpData = bmp.LockBits(new Rectangle(0, 0, this.width, this.height), System.Drawing.Imaging.ImageLockMode.WriteOnly, bmp.PixelFormat);
            int bytesPerPixel = Math.Abs(bmpData.Stride / width);

            for (int i = 0, iEnd = this.height; i < iEnd; i++)
            {
                byte* lineStart = (byte*)bmpData.Scan0 + Math.Abs(bmpData.Stride) * i;
                byte* cur = lineStart;
                for (int j = 0, jEnd = this.width; j < jEnd; j++)
                {
                    byte r = cur[2];
                    byte g = cur[1];
                    byte b = cur[0];
                    imagePixels[i, j, 0] = r / 255.0f;
                    imagePixels[i, j, 1] = g / 255.0f;
                    imagePixels[i, j, 2] = b / 255.0f;
                    cur += bytesPerPixel;
                }
            }

            bmp.UnlockBits(bmpData);
        }

        public Image convertToColorSpace(ColorSpace colorSpaceToConvert)
        {
            float[, ,] rgbMatrix = this.colorSpace.convertColorMatrixToRGB(imagePixels);
            float[, ,] rv = colorSpaceToConvert.convertColorMatrixFromRGB(rgbMatrix);
            Image retValue = new Image(rv, colorSpaceToConvert);
            return retValue;
        }

        unsafe public System.Drawing.Bitmap convertToBitmap()
        {
            float[, ,] rgbMatrix = this.colorSpace.convertColorMatrixToRGB(imagePixels);
            System.Drawing.Bitmap retValue = new System.Drawing.Bitmap(this.width, this.height, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
            System.Drawing.Imaging.BitmapData bmpData = retValue.LockBits(new Rectangle(0, 0, this.width, this.height), System.Drawing.Imaging.ImageLockMode.WriteOnly, retValue.PixelFormat);

            int bytesPerPoint = 4;

            for (int i = 0, iEnd = this.height; i < iEnd; i++)
            {
                byte* lineStart = (byte*)bmpData.Scan0 + Math.Abs(bmpData.Stride) * i;
                byte* cur = lineStart;
                for (int j = 0, jEnd = this.width; j < jEnd; j++)
                {
                    Func<int, byte> myNorm = x => (byte)((((x > 255) ? 255 : x) < 0) ? 0 : x);
                    int r = (int)System.Math.Round(rgbMatrix[i, j, 0] * 255);
                    int g = (int)System.Math.Round(rgbMatrix[i, j, 1] * 255);
                    int b = (int)System.Math.Round(rgbMatrix[i, j, 2] * 255);
                    cur[0] = myNorm(b);
                    cur[1] = myNorm(g);
                    cur[2] = myNorm(r);
                    //cur[0] = (byte)b;
                    //cur[1] = (byte)g;
                    //cur[2] = (byte)r;
                    cur[3] = 255;
                    cur += bytesPerPoint;
                }
            }

            retValue.UnlockBits(bmpData);

            return retValue;
        }

        public void addValueToComponent(int component, float value)
        {
            float maxValue = this.colorSpace.upperBoundForComponent(component);
            float minValue = this.colorSpace.lowerBoundForComponent(component);
            for (int j = 0, jEnd = this.height; j < jEnd; j++)
                for (int i = 0, iEnd = this.width; i < iEnd; i++)
                {
                    float newValue = imagePixels[j, i, component] + value;
                    newValue = (newValue > maxValue) ? maxValue : newValue;
                    newValue = (newValue < minValue) ? minValue : newValue;
                    imagePixels[j, i, component] = newValue;
                }
        }

        public void multiplyComponentByValue(int component, float value)
        {
            float maxValue = this.colorSpace.upperBoundForComponent(component);
            float minValue = this.colorSpace.lowerBoundForComponent(component);
            for (int j = 0, jEnd = this.height; j < jEnd; j++)
                for (int i = 0, iEnd = this.width; i < iEnd; i++)
                {
                    float newValue = imagePixels[j, i, component] * value;                    
                    if (newValue > maxValue)
                        newValue = maxValue;
                    newValue = (newValue > maxValue) ? maxValue : newValue;
                    if (newValue < minValue)
                        newValue = minValue;
                    newValue = (newValue < minValue) ? minValue : newValue;
                    imagePixels[j, i, component] = newValue;
                }
        }

        public int[,] diagramsForComponents(int[] componentIndexes)
        {
            int[,] retValue = new int[componentIndexes.Length, 255];
            float[] minValuesForComponents = new float[componentIndexes.Length];
            float[] maxValuesForComponents = new float[componentIndexes.Length];

            for (int k = 0; k < componentIndexes.Length; k++)
            {
                minValuesForComponents[k] = this.colorSpace.lowerBoundForComponent(componentIndexes[k]);
                maxValuesForComponents[k] = this.colorSpace.upperBoundForComponent(componentIndexes[k]);
            }

            for (int j = 0, jEnd = this.height; j < jEnd; j++)
                for (int i = 0, iEnd = this.width; i < iEnd; i++)
                    for (int k = 0; k < componentIndexes.Length; k++)
                    {
                        float curValue = imagePixels[j, i, componentIndexes[k]];
                        int normalizedValue = (int)System.Math.Round((curValue - minValuesForComponents[k]) / (maxValuesForComponents[k] - minValuesForComponents[k]) * 255);
                        retValue[k, normalizedValue]++;
                    }
            return retValue;
        }

        public void disableComponent(int componentIndex)
        {
            float neutralValue = this.colorSpace.neutralValueForComponent(componentIndex);
            for (int j = 0, jEnd = this.height; j < jEnd; j++)
                for (int i = 0, iEnd = this.width; i < iEnd; i++)
                {
                    imagePixels[j, i, componentIndex] = neutralValue;
                }
        }

        public void setGammaForComponent(int componentIndex, float gammaValue)
        {
            for (int j = 0, jEnd = this.height; j < jEnd; j++)
            {
                for (int i = 0, iEnd = this.width; i < iEnd; i++)
                {
                    float value = imagePixels[j, i, componentIndex];
                    value = (float)Math.Pow(value, gammaValue);
                    imagePixels[j, i, componentIndex] = value;
                }
            }
        }

        public void applyFilter(Filter filter)
        {
            float[, ,] newPixels = filter.applyToPixelMatrix(this.imagePixels);
            imagePixels = newPixels;
        }

        public void testDCT()
        {
            imagePixels = new float[8, 8, 1];
            for (int i = 0; i < 8; i++)
                for (int j = 0; j < 8; j++)
                {
                    imagePixels[i, j, 0] = 1;
                }
            this.performDCT(8);
            this.performRevDCT(8);
        }

        public void testDWT(Wavelet wt)
        {
            imagePixels = new float[8, 8, 1];
            for (int i = 0; i < 8; i++)
                for (int j = 0; j < 8; j++)
                {
                    if ((i & 1) != 0)
                        imagePixels[i, j, 0] = 1;
                    else
                        imagePixels[i, j, 0] = 0;
                }
            this.performDWT(wt, 8, 3);
            this.performRevDWT(wt, 8, 3);
        }

        public void performDCT(int size)
        {
            _originalWidth = this.width;
            _originalHeight = this.height;

            int newWidth = (this.width + size - 1) / size * size;
            int newHeight = (this.height + size - 1) / size * size;

            float[, ,] dctedImage = new float[newWidth, newHeight, colorSpace.componentsCount];
            float[,,,] dctMatrix = DCT.dctForSize(size);

            for (int dim = 0; dim < colorSpace.componentsCount; dim++)
                for (int x = 0; x < newWidth; x += size)
                    for (int y = 0; y < newHeight; y += size)
                    {
                        for (int i = 0; i < size; i++)
                            for (int j = 0; j < size; j++)
                            {
                                float curVal = 0;
                                for (int xx = 0; xx < size; xx++)
                                    for (int yy = 0; yy < size; yy++)
                                    {
                                        if (x + xx < _originalWidth && y + yy < _originalHeight)
                                            curVal += (this.imagePixels[y + yy, x + xx,  dim] - 0.5f) * dctMatrix[i, j, xx, yy];
                                    }
                                dctedImage[y + j, x + i, dim] = curVal;
                            }
                    }

            this.imagePixels = dctedImage;
        }

        public void zeroCoef(int dctSize, int zeroSize)
        {
            int w = this.width;
            int h = this.height;
            int ddct = dctSize * 2;

            for (int dim = 0; dim < colorSpace.componentsCount; dim++)
                for (int y = 0; y < h; y += dctSize)
                    for (int x = 0; x < w; x += dctSize)
                        for (int j = 0; j < dctSize; j++)
                            for (int i = 0; i < dctSize; i++)
                                if (i + j + zeroSize + 1 >= ddct)
                                    this.imagePixels[y + j, x + i, dim] = 0;
        }

        public void performRevDCT(int size)
        {
            float[, ,] originalImage = new float[_originalWidth, _originalHeight, colorSpace.componentsCount];
            float[, , ,] dctMatrix = DCT.revDctForSize(size);

            for (int dim = 0; dim < colorSpace.componentsCount; dim++)
            {
                float lb = colorSpace.lowerBoundForComponent(dim);
                float ub = colorSpace.upperBoundForComponent(dim);
                for (int y = 0; y < _originalHeight; y += size)
                    for (int x = 0; x < _originalWidth; x += size)
                    {
                        for (int j = 0; j < size; j++)
                            for (int i = 0; i < size; i++)
                            {
                                if (x + i >= _originalWidth || y + j >= _originalHeight)
                                    continue;

                                float curVal = 0;
                                for (int yy = 0; yy < size; yy++)
                                    for (int xx = 0; xx < size; xx++)
                                    {
                                        curVal += this.imagePixels[y + yy, x + xx, dim] * dctMatrix[i, j, xx, yy];
                                    }
                                originalImage[y + j, x + i, dim] = curVal + 0.5f;
                                if (originalImage[y + j, x + i, dim] < lb)
                                    originalImage[y + j, x + i, dim] = lb;
                                if (originalImage[y + j, x + i, dim] > ub)
                                    originalImage[y + j, x + i, dim] = ub;
                            }
                    }
            }
            this.imagePixels = originalImage;
        }

        public void quantizeDCT(int size, int quality)
        {
            float q = (101 - quality) / 10.0f;
            float[,] jpgQuantMatrix = new float[,] { { 16,11, 10, 16, 24, 40, 51,61 }, 
                                                    { 12, 12, 14, 19, 26, 58, 60, 55 }, 
                                                    { 14, 13, 16, 24, 40, 57, 69, 56 }, 
                                                    { 14, 17, 22, 29, 51, 87, 80, 62 }, 
                                                    { 18, 22, 37, 56, 68, 109, 103, 77 }, 
                                                    { 24, 35, 55, 64, 81, 104, 113, 92 }, 
                                                    { 49, 64, 78, 87, 103, 121, 120, 101 }, 
                                                    { 72, 92, 95, 98, 112, 100, 103, 99 } };

            if (size == 16)
            {
                float[,] jpgQuantMatrix16 = new float[16, 16];
                for (int i = 0; i < 14; i++)
                    for (int j = 0; j < 14; j++)
                    {
                        if ((i & 1) == 0)
                            if ((j & 1) == 0)
                                jpgQuantMatrix16[i, j] = jpgQuantMatrix[i >> 1, j >> 1];
                            else
                                jpgQuantMatrix16[i, j] = (jpgQuantMatrix[i >> 1, j >> 1] + jpgQuantMatrix[i >> 1, (j >> 1) + 1]) / 2;
                        else
                            if ((j & 1) == 0)
                                jpgQuantMatrix16[i, j] = (jpgQuantMatrix[i >> 1, j >> 1] + jpgQuantMatrix[(i >> 1) + 1, j >> 1]) / 2;
                            else
                                jpgQuantMatrix16[i, j] = (jpgQuantMatrix[i >> 1, j >> 1] + jpgQuantMatrix[i >> 1, (j >> 1) + 1] +
                                    jpgQuantMatrix[(i >> 1) + 1, j >> 1] + jpgQuantMatrix[(i >> 1)  + 1, (j >> 1) + 1]) / 4;
                    }
                for (int i = 0; i < 15; i++)
                {
                    jpgQuantMatrix16[i, 15] = jpgQuantMatrix16[i, 14];
                    jpgQuantMatrix16[15, i] = jpgQuantMatrix16[14, i];
                }
                jpgQuantMatrix = jpgQuantMatrix16;
            }

            for (int i = 0; i < size; i++)
                for (int j = 0; j < size; j++)
                    jpgQuantMatrix[i, j] *= q;

            int w = this.width;
            int h = this.height;

            for (int dim = 0; dim < colorSpace.componentsCount; dim++)
                for (int x = 0; x < w; x += size)
                    for (int y = 0; y < h; y += size)
                    {
                        for (int i = 0; i < size; i++)
                            for (int j = 0; j < size; j++)
                            {
                                float val = imagePixels[x + i, y + j, dim];
                                val = Math.Sign(val) * ((int)(Math.Abs(val) * 255 / jpgQuantMatrix[i, j])) * jpgQuantMatrix[i, j] / 255.0f;
                                imagePixels[x + i, y + j, dim] = val;
                            }
                    }
        }

        public void performDWT(Wavelet wavelet, int size, int depth)
        {
            _originalWidth = this.width;
            _originalHeight = this.height;

            int newWidth = (this.width + size - 1) / size * size;
            int newHeight = (this.height + size - 1) / size * size;

            float[, ,] transformedImage = new float[newWidth, newHeight, colorSpace.componentsCount];
            float[] waveletInputBuf = new float[size];
            float[] waveletOutputBuf = new float[size];

            for (int dim = 0; dim < colorSpace.componentsCount; dim++)
                for (int i = 0; i < _originalWidth; i++)
                    for (int j = 0; j < _originalHeight; j++)
                    {
                        transformedImage[i, j, dim] = imagePixels[i, j, dim];
                    }

            for (int dim = 0; dim < colorSpace.componentsCount; dim++)
                for (int x = 0; x < newWidth; x += size)
                    for (int y = 0; y < newHeight; y += size)
                    {
                        int h = size;
                        int it = 0;
                        while (h >= wavelet.waveLength && (depth-it) > 0)
                        {
                            for (int i = 0; i < h; i++)
                            {
                                for (int j = 0; j < h; j++)
                                {
                                    waveletInputBuf[j] = transformedImage[x + i, y + j, dim];
                                    waveletOutputBuf[j] = 0;
                                }

                                wavelet.forward(waveletInputBuf, waveletOutputBuf, h);

                                for (int j = 0; j < h; j++)
                                    transformedImage[x + i, y + j, dim] = waveletOutputBuf[j];
                            }

                            for (int j = 0; j < h; j++)
                            {
                                for (int i = 0; i < h; i++)
                                {
                                    waveletInputBuf[i] = transformedImage[x + i, y + j, dim];
                                    waveletOutputBuf[i] = 0;
                                }
                                wavelet.forward(waveletInputBuf, waveletOutputBuf, h);
                                for (int i = 0; i < h; i++)
                                    transformedImage[x + i, y + j, dim] = waveletOutputBuf[i];
                            }
                            h = h >> 1;
                            it++;
                        }
                    }
            this.imagePixels = transformedImage;
        }

        public void performRevDWT(Wavelet wavelet, int size, int depth)
        {
            float[, ,] transformedImage = new float[_originalWidth, _originalHeight, colorSpace.componentsCount];
            float[] waveletInputBuf = new float[size];
            float[] waveletOutputBuf = new float[size];

            for (int dim = 0; dim < colorSpace.componentsCount; dim++)
                for (int x = 0; x < _originalWidth; x += size)
                    for (int y = 0; y < _originalHeight; y += size)
                    {
                        int h = size >> (depth-1);
                        while (h <= size)
                        {
                            for (int i = 0; i < h; i++)
                            {
                                for (int j = 0; j < h; j++)
                                {
                                    waveletInputBuf[j] = imagePixels[x + i, y + j, dim];
                                    waveletOutputBuf[j] = 0;
                                }

                                wavelet.reverse(waveletInputBuf, waveletOutputBuf, h);

                                for (int j = 0; j < h; j++)
                                    imagePixels[x + i, y + j, dim] = waveletOutputBuf[j];
                            }

                            for (int j = 0; j < h; j++)
                            {
                                for (int i = 0; i < h; i++)
                                {
                                    waveletInputBuf[i] = imagePixels[x + i, y + j, dim];
                                    waveletOutputBuf[i] = 0;
                                }
                                wavelet.reverse(waveletInputBuf, waveletOutputBuf, h);
                                for (int i = 0; i < h; i++)
                                    imagePixels[x + i, y + j, dim] = waveletOutputBuf[i];
                            }
                            h = h << 1;
                        }
                    }

            for (int dim = 0; dim < colorSpace.componentsCount; dim++)
            {
                float lb = colorSpace.lowerBoundForComponent(dim);
                float ub = colorSpace.upperBoundForComponent(dim);
                for (int i = 0; i < _originalWidth; i++)
                    for (int j = 0; j < _originalHeight; j++)
                    {
                        transformedImage[i, j, dim] = imagePixels[i, j, dim];
                        if (transformedImage[i, j, dim] < lb)
                            transformedImage[i, j, dim] = lb;
                        if (transformedImage[i, j, dim] > ub)
                            transformedImage[i, j, dim] = ub;
                    }
            }

            this.imagePixels = transformedImage;
        }

        public void quantizeDWT(int size, int depth, int quality)
        {
            double R = 8, I = depth, c = 8.5, f = 8;
            float tau = (float)(Math.Pow(2, R - c + I) * (1 + f / (1 << 11))) * (101 - quality) / 10;

            int width = this.width;
            int height = this.height;

            for (int dim = 0; dim < colorSpace.componentsCount; dim++)
                for (int x = 0; x < width; x += size)
                    for (int y = 0; y < height; y += size)
                    {
                        int h = size >> (depth - 1);
                        float d = (float)(tau / Math.Pow(2, I)); //blur portion quantization step
                        int k = 1;
                        if (d > 1)
                        {
                            for (int i = 0; i < h; i++)
                                for (int j = 0; j < h; j++)
                                {
                                    float val = imagePixels[x + i, y + j, dim];
                                    imagePixels[x + i, y + j, dim] = Math.Sign(val) * ((int)(Math.Abs(val) * 255 / d)) * d / 255.0f;
                                }
                        }
                        while (h < size)
                        {
                            float dvh = (float)(tau / Math.Pow(2, k - 1));
                            float dd = dvh / 2;

                            for (int i = 0; i < h; i++)
                                for (int j = 0; j < h; j++)
                                {
                                    float val;
                                    if (dvh > 1)
                                    {
                                        val = imagePixels[x + i + h, y + j, dim];
                                        imagePixels[x + i + h, y + j, dim] = Math.Sign(val) * ((int)(Math.Abs(val) * 255 / dvh)) * dvh / 255.0f;
                                        val = imagePixels[x + i, y + j + h, dim];
                                        imagePixels[x + i, y + j + h, dim] = Math.Sign(val) * ((int)(Math.Abs(val) * 255 / dvh)) * dvh / 255.0f;
                                    }
                                    if (dd > 0)
                                    {
                                        val = imagePixels[x + i + h, y + j + h, dim];
                                        imagePixels[x + i + h, y + j + h, dim] = Math.Sign(val) * ((int)(Math.Abs(val) * 255 / dd)) * dd / 255.0f;
                                    }
                                }
                            h <<= 1;
                        }
                    }
        }
    }
}
