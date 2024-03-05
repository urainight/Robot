using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;


using Emgu.CV;
using Emgu.CV.Structure;
using Emgu.CV.Util;
namespace Robot_processing
{
    public partial class Form1 : Form
    {
        VideoCapture _capture;
        bool _streaming;
        float sendx;
        float sendy;
        public Form1()
        {
            InitializeComponent();
        }
        private void button1_Click(object sender, EventArgs e)
        {
            if (!_streaming)
            {
                Application.Idle += streaming;
            }
            else
            {
                Application.Idle -= streaming;
            }
            _streaming = !_streaming;
        }
        private void DetectShape(Image<Bgr, byte> imgInput)
        {

            var temp = imgInput.SmoothGaussian(5).Convert<Gray,
           byte>().ThresholdBinary(new Gray(150), new Gray(255));
            VectorOfVectorOfPoint contours = new VectorOfVectorOfPoint();
            Mat m = new Mat();
            CvInvoke.FindContours(temp, contours, m,
           Emgu.CV.CvEnum.RetrType.External,
           Emgu.CV.CvEnum.ChainApproxMethod.ChainApproxSimple);
            for (int i = 0; i < contours.Size; i++)
            {
                double perimeter = CvInvoke.ArcLength(contours[i], true);
                VectorOfPoint approx = new VectorOfPoint();
                CvInvoke.ApproxPolyDP(contours[i], approx, 0.04 * perimeter, true);
                CvInvoke.DrawContours(imgInput, contours, i, new MCvScalar(0, 0,
               255), 2);
                RotatedRect plate_feature = CvInvoke.MinAreaRect(contours[i]);
                var moments = CvInvoke.Moments(contours[i]);
                int x = (int)(moments.M10 / moments.M00);
                int y = (int)(moments.M01 / moments.M00);

                if (approx.Size == 4)
                {
                    CvInvoke.PutText(imgInput, "Hinh Chu Nhat", new Point(x, y),
                    Emgu.CV.CvEnum.FontFace.HersheySimplex, 0.5, new
                   MCvScalar(0, 0, 255), 2);
                    Console.WriteLine("xR:" + sendx + "yR:" + sendy);
                    sendx = x;
                    sendy = y;
                    serialPort1.Write("R" + sendx + "," + sendy);
                    Console.WriteLine(serialPort1.ReadExisting());

                }
                if (approx.Size == 6)
                {
                    CvInvoke.PutText(imgInput, "Hinh Luc Giac", new Point(x, y),
                    Emgu.CV.CvEnum.FontFace.HersheySimplex, 0.5, new
                   MCvScalar(0, 0, 255), 2);
                    Console.WriteLine("xH:" + sendx + "yH:" + sendy);
                    sendx = x;
                    sendy = y;
                    serialPort1.Write("H" + sendx + "," + sendy);
                    Console.WriteLine(serialPort1.ReadExisting());

                }

                if (approx.Size > 6)
                {
                    CvInvoke.PutText(imgInput, "Hinh Tron", new Point(x, y),
                    Emgu.CV.CvEnum.FontFace.HersheySimplex, 0.5, new
                   MCvScalar(0, 0, 255), 2);
                    Console.WriteLine("xC:" + sendx + "yC:" + sendy);
                    sendx = x;
                    sendy = y;
                    serialPort1.WriteLine("C" + sendx + "," + sendy);
                }
                pictureBox1.Image = imgInput.Bitmap;
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            _streaming = false;
            _capture = new VideoCapture();

            serialPort1.Open();

        }

        private async void streaming(object sender, System.EventArgs e)
        {
            var img = _capture.QueryFrame().ToImage<Bgr, byte>();
            var bmp = img.Bitmap;
            pictureBox1.Image = bmp;

            try
            {
                Mat m = new Mat();
                _capture.Read(m);
                if (!m.IsEmpty)
                {
                    pictureBox1.Image = m.Bitmap;
                    DetectShape(m.ToImage<Bgr, byte>());
                    double fps =
                   _capture.GetCaptureProperty(Emgu.CV.CvEnum.CapProp.Fps);
                    await Task.Delay(1000 / Convert.ToInt32(fps));
                    Thread.Sleep(1200);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }
    }
}


