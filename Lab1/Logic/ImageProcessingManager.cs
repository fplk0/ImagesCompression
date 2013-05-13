using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab1
{
    class ImageProcessingManager : ImageProcessingObserver
    {
        static int coresCount;

        private sealed class ImageProcessingManagerSingletonCreator
        {
            private static readonly ImageProcessingManager instance = new ImageProcessingManager();
            public static ImageProcessingManager Instance { get { return instance; } }
        }

        public static ImageProcessingManager Instance
        {
            get { return ImageProcessingManagerSingletonCreator.Instance; }
        }

        public static ImageProcessingManager singleInstance()
        {
            return ImageProcessingManagerSingletonCreator.Instance;
        }

        protected ImageProcessingManager()
        {
            coresCount = Environment.ProcessorCount;
            processingMutex = new System.Threading.Mutex();
            imageProcessingForTime = new SortedDictionary<int, ImageProcessing>();
            timeForImageProcessing = new Dictionary<ImageProcessing, int>();
            runningImageProcessings = new SortedSet<ImageProcessing>();
        }

        SortedDictionary<int, ImageProcessing> imageProcessingForTime;
        Dictionary<ImageProcessing, int> timeForImageProcessing;
        SortedSet<ImageProcessing> runningImageProcessings;
        System.Threading.Mutex processingMutex;

        
        int currentProcessingIndex;

        public void startImageProcessing(ImageProcessing imgProc)
        {
            Task.Run(() =>
            {
                processingMutex.WaitOne();

                if (runningImageProcessings.Count < coresCount)
                {
                    imgProc.addObserver(this);
                    imgProc.runAsynchronous();
                    runningImageProcessings.Add(imgProc);
                }
                else
                {
                    imageProcessingForTime[currentProcessingIndex] = imgProc;
                    timeForImageProcessing[imgProc] = currentProcessingIndex;
                }

                currentProcessingIndex++;

                processingMutex.ReleaseMutex();
            });
        }

        private void enqueueNextIfNeeded()
        {
            if (runningImageProcessings.Count < coresCount)
            {
                if (timeForImageProcessing.Count == 0)
                    return;
                KeyValuePair<int, ImageProcessing> imgProc = imageProcessingForTime.First();
                imageProcessingForTime.Remove(imgProc.Key);

                imgProc.Value.runAsynchronous();
                runningImageProcessings.Add(imgProc.Value);
            }
        }

        public void cancelImageProcessing(ImageProcessing imgProc)
        {
            Task.Run(() =>
            {
                processingMutex.WaitOne();

                if (runningImageProcessings.Contains(imgProc))
                {
                    imgProc.cancel();
                    imgProc.removeObserver(this);
                    runningImageProcessings.Remove(imgProc);
                    enqueueNextIfNeeded();
                }
                else
                {
                    int imageProcTime;
                    if (timeForImageProcessing.TryGetValue(imgProc, out imageProcTime))
                    {
                        timeForImageProcessing.Remove(imgProc);
                        imageProcessingForTime.Remove(imageProcTime);
                    }
                }

                processingMutex.ReleaseMutex();
            });
        }

        public void cancelAllImageProcessings()
        {
            foreach (ImageProcessing imgProc in runningImageProcessings)
            {
                imgProc.cancel();
            }

            foreach (KeyValuePair<int, ImageProcessing> imgProc in imageProcessingForTime)
            {
                imgProc.Value.cancel();
            }

            runningImageProcessings.Clear();
            imageProcessingForTime.Clear();
            timeForImageProcessing.Clear();
        }

        public void didFinishImageProcessing(ImageProcessing imageProcessing)
        {
            processingMutex.WaitOne();

            runningImageProcessings.Remove(imageProcessing);
            enqueueNextIfNeeded();

            processingMutex.ReleaseMutex();
        }

        public void didCancelImageProcessing(ImageProcessing imgProc)
        {
            processingMutex.WaitOne();

            if (runningImageProcessings.Contains(imgProc))
            {
                imgProc.cancel();
                imgProc.removeObserver(this);
                runningImageProcessings.Remove(imgProc);
                enqueueNextIfNeeded();
            }
            else
            {
                int imageProcTime;
                if (timeForImageProcessing.TryGetValue(imgProc, out imageProcTime))
                {
                    timeForImageProcessing.Remove(imgProc);
                    imageProcessingForTime.Remove(imageProcTime);
                }
            }

            processingMutex.ReleaseMutex();
        }
    }
}
