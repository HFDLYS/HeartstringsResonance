#ifndef GRAPHICSTHREAD_H
#define GRAPHICSTHREAD_H

#include <QThread>

class GraphicsThread : public QThread {
    Q_OBJECT
   public:
    explicit GraphicsThread(QObject *parent = nullptr);
    ~GraphicsThread();

   private:
    void run() override;

   signals:
    void timeout();
};

#endif  // GRAPHICSTHREAD_H
