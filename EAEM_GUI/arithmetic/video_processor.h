#ifndef VIDEO_PROCESSOR_H
#define VIDEO_PROCESSOR_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <opencv2/opencv.hpp>

class VideoProcessor : public QObject
{
    Q_OBJECT
public:
    explicit VideoProcessor(QObject *parent = nullptr);
    ~VideoProcessor();

    void startProcessing();
    void stopProcessing();

public slots:
    void processFrame(const QImage &image);

signals:
    void processedFrameReady(const QImage &image);

private:
    QThread *processingThread;
    bool running_;
};

#endif // VIDEO_PROCESSOR_H
