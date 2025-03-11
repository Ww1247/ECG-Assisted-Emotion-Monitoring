#include "video_processor.h"
#include <QDebug>

VideoProcessor::VideoProcessor(QObject *parent)
    : QObject(parent),
      running_(false),
      processingThread(new QThread(this))
{
    moveToThread(processingThread);
    connect(processingThread, &QThread::started, this, &VideoProcessor::startProcessing);
    connect(this, &VideoProcessor::processedFrameReady, processingThread, &QThread::quit);
}

VideoProcessor::~VideoProcessor()
{
    stopProcessing();
    processingThread->quit();
    processingThread->wait();
    delete processingThread;
}

void VideoProcessor::startProcessing()
{
    running_ = true;
}

void VideoProcessor::stopProcessing()
{
    running_ = false;
}

void VideoProcessor::processFrame(const QImage &image)
{
    if (!running_) return;

    cv::Mat mat(image.height(), image.width(), CV_8UC3, const_cast<uchar *>(image.bits()), image.bytesPerLine());
    cv::Mat processedMat;
    cv::cvtColor(mat, processedMat, cv::COLOR_RGB2GRAY);
    cv::cvtColor(processedMat, processedMat, cv::COLOR_GRAY2RGB);

    QImage processedImage(processedMat.data, processedMat.cols, processedMat.rows, processedMat.step, QImage::Format_RGB888);
    emit processedFrameReady(processedImage);
}
