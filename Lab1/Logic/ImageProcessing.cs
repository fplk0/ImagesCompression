using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab1
{
    public class ImageProcessing
    {
        volatile bool cancelled;

        const float EPS = 1e-9f;

        #region ImageChanges

        public bool[] selectedComponents;
        public float[] addedBrightness;
        public float[] multipliedBrightness;
        public float[] componentsGamma;
        public List<Filter> filtersToApply;

        public bool useDWT;
        public int dwtBlockSize;
        public int dwtDepth;
        public bool useDWTQuant;
        public int dwtQuantQuality;

        public bool useDCT;
        public int dctBlockSize;
        public int dctZeroCoef;
        public bool useDCTQuant;
        public int dctQuantQuality;

        private HashSet<ImageProcessingObserver> observers;

        #endregion

        ColorSpace colorSpace;
        Image img;
        Image _resultImage;

        public Image resultImage
        {
            protected set { _resultImage = value; }
            get { return _resultImage; }
        }

        public ImageProcessing(Image _image, ColorSpace cs)
        {
            colorSpace = cs;
            img = _image.convertToColorSpace(colorSpace);
            cancelled = false;

            selectedComponents = new bool[colorSpace.componentsCount];
            addedBrightness = new float[colorSpace.componentsCount];
            multipliedBrightness = new float[colorSpace.componentsCount];
            componentsGamma = new float[colorSpace.componentsCount];

            for (int i = 0; i < colorSpace.componentsCount; i++)
            {
                selectedComponents[i] = true;
                addedBrightness[i] = 0.0f;
                multipliedBrightness[i] = 1.0f;
                componentsGamma[i] = 1.0f;
            }

            observers = new HashSet<ImageProcessingObserver>();
        }

        public ImageProcessing(System.Drawing.Image img, ColorSpace cs)
            : this(new Image(img), cs)
        {
 
        }

        public void cancel()
        {
            cancelled = true;

            notifyObserversCancel(this);
        }

        public void run()
        {
            lock (this)
            {
                System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
                //sw.Start();
                for (int i = 0; i < img.colorSpace.componentsCount; i++)
                {
                    if (cancelled)
                        return;
                    if (selectedComponents[i] != true)
                        img.disableComponent(i);

                    if (cancelled)
                        return;
                    if (Math.Abs(multipliedBrightness[i] - 1.0f) > EPS)
                        img.multiplyComponentByValue(i, multipliedBrightness[i]);

                    if (cancelled)
                        return;
                    if (Math.Abs(addedBrightness[i] - 0.0f) > EPS)
                        img.addValueToComponent(i, addedBrightness[i]);

                    if (cancelled)
                        return;
                    if (Math.Abs(componentsGamma[i] - 1.0f) > EPS)
                        img.setGammaForComponent(i, componentsGamma[i]);
                }

                Image imageAfterFilters = img;

                if (filtersToApply != null && filtersToApply.Count != 0)
                {
                    imageAfterFilters = img.convertToColorSpace(new ColorSpaceRGB());

                    for (int i = 0; i < filtersToApply.Count; i++)
                    {
                        if (cancelled)
                            return;
                        imageAfterFilters.applyFilter(filtersToApply[i]);
                    }
                }

                Image imgAfterTransforms = imageAfterFilters;

                if (useDWT)
                {
                    Wavelet hr = new Haar();
                    imgAfterTransforms.performDWT(hr, dwtBlockSize, dwtDepth);

                    if (cancelled)
                        return;

                    if (useDWTQuant)
                    {
                        imgAfterTransforms.quantizeDWT(dwtBlockSize, dwtDepth, dwtQuantQuality);
                    }

                    if (cancelled)
                        return;

                    imgAfterTransforms.performRevDWT(hr, dwtBlockSize, dwtDepth);
                }

                if (useDCT)
                {
                    imgAfterTransforms.performDCT(dctBlockSize);
                    if (cancelled)
                        return;

                    if (useDCTQuant)
                        imgAfterTransforms.quantizeDCT(dctBlockSize, dctQuantQuality);
                    if (dctZeroCoef != 0)
                        imgAfterTransforms.zeroCoef(dctBlockSize, dctZeroCoef);

                    if (cancelled)
                        return;

                    imgAfterTransforms.performRevDCT(dctBlockSize);
                }

                //Image ti = new Image(8, 8, new ColorSpaceGS());
                //ti.testDWT(hr);
                //Image gsImage = img;//.convertToColorSpace(new ColorSpaceGS());

                //gsImage.performDWT(hr, 64, 3);
                ////gsImage.zeroCoef(8, 7);
                //gsImage.performRevDWT(hr, 64, 3);
                //gsImage.performDCT(8);
                //gsImage.zeroCoef(8, 5);
                //gsImage.performRevDCT(8);

                this.resultImage = imgAfterTransforms;
                //sw.Stop();
                //System.Windows.MessageBox.Show(Convert.ToString(sw.ElapsedMilliseconds));

                if (!cancelled)
                    notifyObserversFinish(this);
            }
            //System.GC.Collect();
        }

        public void runAsynchronous()
        {
            Task.Run(() => { this.run(); });
        }

        public void addObserver(ImageProcessingObserver obs)
        {
            lock (observers)
            {
                observers.Add(obs);
            }
        }

        public void removeObserver(ImageProcessingObserver obs)
        {
            lock (observers)
            {
                observers.Remove(obs);
            }
        }

        private void notifyObserversCancel(ImageProcessing imgProc)
        {
            lock (observers)
            {
                foreach (ImageProcessingObserver observer in observers)
                {
                    observer.didCancelImageProcessing(imgProc);
                }
            }
        }

        private void notifyObserversFinish(ImageProcessing imgProc)
        {
            lock (observers)
            {
                foreach (ImageProcessingObserver observer in observers)
                {
                    observer.didFinishImageProcessing(imgProc);
                }
            }
        }
    }

    public interface ImageProcessingObserver
    {
        void didFinishImageProcessing(ImageProcessing imageProcessing);
        void didCancelImageProcessing(ImageProcessing imageProcessing);
    }
}
