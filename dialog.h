#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include <QPushButton>
#include <QPainter>
#include <QtGui>

const int lineMode = 1;
const int rectangleMode = 2;
const int cycleMode = 3;
const int eraseMode = 4;

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();


private:
    Ui::Dialog *ui;
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void drawLine(QPainter **painter);
    void drawRect(QPainter **painter);
    void drawCycle(QPainter **painter);
    void draw(QPainter *painter);
    void do_file_New();
    void do_file_SaveOrNot();
    void do_file_Save();
    void do_file_SaveAs();
    bool saveFile(const QString &fileName);
    QPixmap pix;
    QPoint lastPoint;
    QPoint endPoint;
    QPoint tempLastPoint;
    QPoint tempEndPoint;
    QPixmap tempPix; //临时画布
    QString curFile; //当期文件名
    bool isDrawing;
    bool isSaved; //是否已保存
    int scale; //放大倍数
    int mode; //画图模式
    //画笔颜色rgb值
    int red;
    int green;
    int blue;
    int penWeight; //线条粗细

private slots:
//    void zoomIn();
    void on_clearBtn_clicked();
    void on_redVal_textChanged(const QString &arg1);
    void on_greenVal_textChanged(const QString &arg1);
    void on_blueVal_textChanged(const QString &arg1);
    void on_rSlider_valueChanged(int value);
    void on_gSlider_valueChanged(int value);
    void on_bSlider_valueChanged(int value);
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void on_zoomInBtn_clicked();
    void on_zoomOutBtn_clicked();
    void on_pushButton_2_clicked();
    void on_lineWeightSlider_valueChanged(int value);
    void on_openBtn_clicked();
    void on_saveBtn_clicked();
    void on_saveAgainBtn_clicked();
    void on_exitBtn_clicked();
    void on_eraseBtn_clicked();
};

#endif // DIALOG_H
