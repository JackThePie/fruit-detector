/* Program for detecting apples on the tree using OpenCV and different methods: HSV treshold, Hough circle detection, blob detection and Canny edge detection.
by Jacek Bera */


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <stdio.h>
#include <iostream>

// Create new window
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

// Destructor
MainWindow::~MainWindow()
{
    delete ui;
}


// Button to open file
void MainWindow::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Wybierz film..."), "C:/", tr("Pliki avi (*.avi)"));
     filename1 = filename.toStdString();
}

// Files for testing
void MainWindow::on_pushButton_8_clicked()
{
    filename1 = "jabzo2a.avi";
}

void MainWindow::on_pushButton_6_clicked()
{
    filename1 = "jabzo5zob.avi";
}

void MainWindow::on_pushButton_9_clicked()
{
    filename1 = "jabzo5a.avi";
}

// Conversion matrics to QImage instance
QImage MainWindow::MatToQImage(const Mat& mat)
{
    if(mat.type()==CV_8UC1)
    {
        // Set the color table
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    }
    else if(mat.type()==CV_8UC3)
    {
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    else
    {
        return QImage();
    }
}

// Blob detection
void MainWindow::on_pushButton_4_clicked()
{
	// Capture the video from web cam
    VideoCapture capture(filename1); 
	// If not success, exit program
    if (!capture.isOpened())  
    {
        cout << "Cannot open the video file" << endl;
    }

    namedWindow("Ustawienia Blob", CV_WINDOW_AUTOSIZE); // Create a window called "Ustawienia Blob"
    int colour = 10;
    int iLowTresh = 0;
    int iHighTresh = 300;
    int minArea = 150;
    int minCirc = 0.1;
    int minConv = 0.087;
    int minInertia = 0.0001;

    // Create trackbars in "Ustawienia Blob" window
    cvCreateTrackbar("Colour", "Ustawienia Blob", &colour, 255); //Hue (0 - 179)
    cvCreateTrackbar("minTresh", "Ustawienia Blob", &iLowTresh, 100); //Hue (0 - 179)
    cvCreateTrackbar("maxTresh", "Ustawienia Blob", &iHighTresh, 700);
    cvCreateTrackbar("minArea", "Ustawienia Blob", &minArea, 700); //Saturation (0 - 255)
    cvCreateTrackbar("minCirc", "Ustawienia Blob", &minCirc, 200); //Value (0 - 255)
    cvCreateTrackbar("minConv", "Ustawienia Blob", &minConv, 200);
    cvCreateTrackbar("minInertia", "Ustawienia Blob", &minInertia, 100);

    while (true)
    {
        Mat imgOriginal;
		// Read a new frame from video
        bool bSuccess = capture.read(imgOriginal); 
		// If not success, break loop
        if (!bSuccess) 
        {
            break;
        }
        // Setup SimpleBlobDetector parameters.
        SimpleBlobDetector::Params params;

        params.filterByColor = true;
        params.blobColor = colour;

        // Change thresholds
        params.minThreshold = iLowTresh;
        params.maxThreshold = iHighTresh;

        // Filter by Area
        params.filterByArea = true;
        params.minArea = minArea;

        // Filter by Circularity
        params.filterByCircularity = true;
        params.minCircularity = minCirc;

        // Filter by Convexity
        params.filterByConvexity = true;
        params.minConvexity = minConv;

        // Filter by Inertia
        params.filterByInertia = true;
        params.minInertiaRatio = minInertia;

        // Set up detector with params
        SimpleBlobDetector detector(params);

        // Storage for blobs
        std::vector<KeyPoint> keypoints;

        // Detect blobs
        detector.detect(imgOriginal, keypoints);

        // Draw detected blobs as red circles.
        // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
        // the size of the circle corresponds to the size of blob

        Mat im_with_keypoints;
        drawKeypoints(imgOriginal, keypoints, im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

        QImage qigm=MatToQImage(imgOriginal);
        image=imgOriginal.clone();
        ui->label->setPixmap(QPixmap::fromImage(qigm).scaledToWidth(ui->label->size().width(),Qt::FastTransformation));

        QImage qigm2=MatToQImage(im_with_keypoints);
        image=im_with_keypoints.clone();
        ui->label_2->setPixmap(QPixmap::fromImage(qigm2).scaledToWidth(ui->label->size().width(),Qt::FastTransformation));

		// Wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        if (waitKey(30) == 27) 
        {
            break;
        }
    }
}

// Hogh circle detection
void MainWindow::on_pushButton_2_clicked()  
{
	// Capture the video from web cam
    VideoCapture cap(filename1); 
	// If not success, exit program
    if (!cap.isOpened())  
    {
        cout << "Cannot open the video file" << endl;
    }

	// Create a window called "Control"
    namedWindow("Ustawienia Hougha", CV_WINDOW_AUTOSIZE); 
    int size_x = 9;
    int size_y = 9;
    int deviation_x = 2;
    int deviation_y = 2;
    int upperTresholdCanny = 150;
    int tresholdCenter = 100;
    int minRad = 0;
    int maxRad = 0;

    cvCreateTrackbar("deviation_x", "Ustawienia Hougha", &deviation_x, 10);
    cvCreateTrackbar("deviation_y", "Ustawienia Hougha", &deviation_y, 10);
    cvCreateTrackbar("upperTresholdCanny", "Ustawienia Hougha", &upperTresholdCanny, 300);
    cvCreateTrackbar("tresholdCenter", "Ustawienia Hougha", &tresholdCenter, 300);
    cvCreateTrackbar("minRad", "Ustawienia Hougha", &minRad, 100);
    cvCreateTrackbar("maxRad", "Ustawienia Hougha", &maxRad, 1000);

    while (true)
    {
        Mat imgOriginal;
        bool bSuccess = cap.read(imgOriginal);
        if (!bSuccess)
        {
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }

        Mat src_gray;
        Mat out;
        // Convert the image to grayscale
        cvtColor(imgOriginal, src_gray, CV_BGR2GRAY );

        // Reduce the noise so we avoid false circle detection
        GaussianBlur(src_gray, src_gray, Size(size_x, size_y), deviation_x, deviation_y );

        vector<Vec3f> circles;

        // Apply the Hough Transform to find the circles
        HoughCircles(src_gray, circles, CV_HOUGH_GRADIENT, 1, 20, upperTresholdCanny, tresholdCenter, minRad, maxRad );

        // Draw the circles detected
        for( size_t i = 0; i < circles.size(); i++ )
        {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            // Circle center
            circle(imgOriginal, center, 3, Scalar(0,255,0), -1, 8, 0 );
            // Circle outline
            circle(imgOriginal, center, radius, Scalar(0,0,255), 3, 8, 0 );
        }

        QImage qigm=MatToQImage(imgOriginal);
        image=imgOriginal.clone();
        ui->label->setPixmap(QPixmap::fromImage(qigm).scaledToWidth(ui->label->size().width(),Qt::FastTransformation));

        QImage qigm2=MatToQImage(src_gray);
        image=src_gray.clone();
        ui->label_2->setPixmap(QPixmap::fromImage(qigm2).scaledToWidth(ui->label->size().width(),Qt::FastTransformation));

        if (waitKey(30) == 27)
        {
            break;
        }

    }
}

// Canny edge detections
void MainWindow::on_pushButton_5_clicked() 
{
    VideoCapture cap(filename1);
    if (!cap.isOpened())
    {
        cout << "Cannot open the video file" << endl;
    }

	// Create a window called "Ustawienia Edge"
    namedWindow("Ustawienia Edge", CV_WINDOW_AUTOSIZE); 

    // int edgeThresh = 1;
    int lowThreshold = 40;
    // int max_lowThreshold = 100;

    int ratio = 3;
    int kernel_size = 3;


    // Create trackbars in "Ustawienia Edge" window
    cvCreateTrackbar("lowThreshold", "Ustawienia Edge", &lowThreshold, 100);
    cvCreateTrackbar("ratio", "Ustawienia Edge", &ratio, 20);


    while (true)
    {
        Mat imgOriginal;
        bool bSuccess = cap.read(imgOriginal);
        if (!bSuccess)
        {
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }

        Mat src_gray;
        Mat detected_edges;


        // Convert the image to grayscale
        cvtColor(imgOriginal, src_gray, CV_BGR2GRAY );

        // Reduce noise with a kernel 3x3
        blur(src_gray, detected_edges, Size(3,3) );

        // Canny detector
        Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

		// Show the original image
        QImage qigm=MatToQImage(imgOriginal);
        image=imgOriginal.clone();
        ui->label->setPixmap(QPixmap::fromImage(qigm).scaledToWidth(ui->label->size().width(),Qt::FastTransformation));

        QImage qigm2=MatToQImage(detected_edges);
        image=detected_edges.clone();
        ui->label_2->setPixmap(QPixmap::fromImage(qigm2).scaledToWidth(ui->label->size().width(),Qt::FastTransformation));

        if (waitKey(30) == 27)
        {
            break;
        }
    }
}


// HSV tresholding
void MainWindow::on_pushButton_7_clicked()  
{


    VideoCapture cap(filename1);
    if (!cap.isOpened())
    {
        cout << "Cannot open the video file" << endl;
    }

	// Create a window called "Ustawienia HSV"
    namedWindow("Ustawienia HSV", CV_WINDOW_AUTOSIZE); 
    int iLowH = 0;
    int iHighH = 179;
    int iLowS = 0;
    int iHighS = 255;
    int iLowV = 0;
    int iHighV = 255;

    // Create trackbars in "Ustawienia HSV" window
    cvCreateTrackbar("LowH", "Ustawienia HSV", &iLowH, 179); // Hue (0 - 179)
    cvCreateTrackbar("HighH", "Ustawienia HSV", &iHighH, 179);
    cvCreateTrackbar("LowS", "Ustawienia HSV", &iLowS, 255); // Saturation (0 - 255)
    cvCreateTrackbar("HighS", "Ustawienia HSV", &iHighS, 255);
    cvCreateTrackbar("LowV", "Ustawienia HSV", &iLowV, 255); // Value (0 - 255)
    cvCreateTrackbar("HighV", "Ustawienia HSV", &iHighV, 255);

    while (true)
    {
        Mat imgOriginal;
        bool bSuccess = cap.read(imgOriginal);
        if (!bSuccess)
        {
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }

        Mat imgHSV;
		// Convert the captured frame from BGR to HSV
        cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); 

        Mat imgThresholded;
		// Threshold the image
        inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); 

        // Morphological opening (remove small objects from the foreground)
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        // Morphological closing (fill small holes in the foreground)
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

		// Show the original image
        QImage qigm=MatToQImage(imgOriginal);
        image=imgOriginal.clone();
        ui->label->setPixmap(QPixmap::fromImage(qigm).scaledToWidth(ui->label->size().width(),Qt::FastTransformation));

		// Show the thresholded image
        QImage qigm2=MatToQImage(imgThresholded);
        image=imgThresholded.clone();
        ui->label_2->setPixmap(QPixmap::fromImage(qigm2).scaledToWidth(ui->label->size().width(),Qt::FastTransformation));

        if (waitKey(30) == 27)
        {
            break;
        }
    }
}
