#include "dialog.h"
#include "ui_dialog.h"
#include <QPainter>
#include <QtGui>
#include <QString>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    resize(660,480);
    pix = QPixmap(200,200);
    pix.fill(Qt::white);
    scale = 1;
    mode = lineMode;
    isDrawing = false;
    red = 0;
    green = 0;
    blue = 0;
    ui->penColor->setStyleSheet("background-color: rgb("+QString::number(red)+", "+QString::number(green)+", "+QString::number(blue)+");");
    penWeight = 1;
    curFile = tr("未命名.png");
    setWindowTitle(curFile);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::paintEvent(QPaintEvent *){


    QPen pen;
    if(mode==eraseMode){
        pen.setColor(Qt::white);
    }else{
        pen.setColor(QColor(red,green,blue));
    }
    pen.setWidth(penWeight);
    if(mode==lineMode||mode==eraseMode){
        QPainter pp(&pix);
        pp.setPen(pen);
        draw(&pp);
    }else if(mode==rectangleMode||mode==cycleMode){
        //双缓冲图层
        if(isDrawing){
            tempPix = pix;
            QPainter pp(&tempPix);
            pp.setPen(pen);
            draw(&pp);
        }else{
            QPainter pp(&pix);
            pp.setPen(pen);
            draw(&pp);
            //发送点击事件，规避某个不明真相的bug
            QPoint pos(100,100);
            QMouseEvent event0(QEvent::MouseButtonPress, pos, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
            QApplication::sendEvent(this,&event0);
        }
    }

        QPainter painter(this);
        painter.scale(scale,scale);
        if(mode==lineMode||mode==eraseMode){
            painter.drawPixmap(0,0,pix);
        }else{
            if(isDrawing){
                painter.drawPixmap(0,0,tempPix);
            }else{
                painter.drawPixmap(0,0,pix);
            }
        }
}

//鼠标点击事件
void Dialog::mousePressEvent(QMouseEvent *event){
    if(event->button()==Qt::LeftButton){
        lastPoint=event->pos();
        endPoint=event->pos();
        isDrawing = true;
        update();
    }
}

//鼠标点击后移动事件
void Dialog::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons()==Qt::LeftButton){
        endPoint=event->pos();
        update();
    }
}

//鼠标释放事件
void Dialog::mouseReleaseEvent(QMouseEvent *event){
    if(event->button()==Qt::LeftButton){
        endPoint=event->pos();
        isDrawing = false;
        update();
        if(mode==lineMode||mode==eraseMode){
            QPoint pos(5000,5000);
            lastPoint = pos;
            endPoint = pos;
        }
    }
}




void Dialog::on_clearBtn_clicked()
{
    pix.fill(Qt::white);
    tempPix.fill(Qt::white);
    update();
}

void Dialog::draw(QPainter *painter){
    switch (mode) {
    case lineMode:
      drawLine(&painter);
        break;
    case rectangleMode:
      drawRect(&painter);
        break;
    case cycleMode:
      drawCycle(&painter);
        break;
    case eraseMode:
      drawLine(&painter);
        break;
    default:
        break;
    }
}

//划线
void Dialog::drawLine(QPainter **painter){
    (*painter)->drawLine(lastPoint/scale,endPoint/scale);
    lastPoint = endPoint;
}
//矩形
void Dialog::drawRect(QPainter **painter){
    int x,y,w,h;
    x=lastPoint.x();
    y=lastPoint.y();
    w=endPoint.x()-x;
    h=endPoint.y()-y;
    (*painter)->drawRect(x/scale,y/scale,w/scale,h/scale);
}

void Dialog::drawCycle(QPainter **painter){
    int x,y,w,h;
    x=lastPoint.x();
    y=lastPoint.y();
    w=endPoint.x()-x;
    h=endPoint.y()-y;
    (*painter)->drawEllipse(x/scale,y/scale,w/scale,h/scale);
}

//画笔颜色选择

void Dialog::on_redVal_textChanged(const QString &redVal)
{
    if(redVal.toInt()>=0&&redVal.toInt()<=255){
        red = redVal.toInt();
        ui->penColor->setStyleSheet("background-color: rgb("+QString::number(red)+", "+QString::number(green)+", "+QString::number(blue)+");");
    }
}

void Dialog::on_greenVal_textChanged(const QString &greenVal)
{
    if(greenVal.toInt()>=0&&greenVal.toInt()<=255){
        green = greenVal.toInt();
        ui->penColor->setStyleSheet("background-color: rgb("+QString::number(red)+", "+QString::number(green)+", "+QString::number(blue)+");");
    }
}

void Dialog::on_blueVal_textChanged(const QString &blueVal)
{
    if(blueVal.toInt()>=0&&blueVal.toInt()<=255){
        blue = blueVal.toInt();
        ui->penColor->setStyleSheet("background-color: rgb("+QString::number(red)+", "+QString::number(green)+", "+QString::number(blue)+");");
    }
}

void Dialog::on_rSlider_valueChanged(int rVal)
{
    if(rVal>=0&&rVal<=255){
        red = rVal;
        ui->penColor->setStyleSheet("background-color: rgb("+QString::number(red)+", "+QString::number(green)+", "+QString::number(blue)+");");
        ui->redVal->setText(QString::number(red));
    }
}

void Dialog::on_gSlider_valueChanged(int gVal)
{
    if(gVal>=0&&gVal<=255){
        green = gVal;
        ui->penColor->setStyleSheet("background-color: rgb("+QString::number(red)+", "+QString::number(green)+", "+QString::number(blue)+");");
        ui->greenVal->setText(QString::number(red));
    }
}

void Dialog::on_bSlider_valueChanged(int bVal)
{
    if(bVal>=0&&bVal<=255){
        blue = bVal;
        ui->penColor->setStyleSheet("background-color: rgb("+QString::number(red)+", "+QString::number(green)+", "+QString::number(blue)+");");
        ui->blueVal->setText(QString::number(red));
    }
}

//模式转换
void Dialog::on_pushButton_clicked()
{
    mode = lineMode;
    red = ui->redVal->text().toInt();
    green = ui->greenVal->text().toInt();
    blue = ui->blueVal->text().toInt();
}

void Dialog::on_pushButton_3_clicked()
{
    mode = rectangleMode;
    red = ui->redVal->text().toInt();
    green = ui->greenVal->text().toInt();
    blue = ui->blueVal->text().toInt();
}

void Dialog::on_pushButton_2_clicked()
{
    mode = cycleMode;
    red = ui->redVal->text().toInt();
    green = ui->greenVal->text().toInt();
    blue = ui->blueVal->text().toInt();
}

//放大功能
void Dialog::on_zoomInBtn_clicked()
{
    if(scale==1){
        scale*=2;
        update();
    }

}

void Dialog::on_zoomOutBtn_clicked()
{
    if(scale==2){
        scale/=2;
        update();
    }

}


void Dialog::on_lineWeightSlider_valueChanged(int pWeight)
{
    if(pWeight>=1&&pWeight<=99){
        penWeight=pWeight;
    }
    ui->penWeightVal->setText(QString::number(penWeight));
}


//文件操作
void Dialog::do_file_New(){

}

void Dialog::do_file_Save(){
    if(isSaved){
        saveFile(curFile);
    }else{
        do_file_SaveAs();
    }
}

void Dialog::do_file_SaveAs(){
    QString fileName = QFileDialog::getSaveFileName(this,tr("另存为"),curFile);

    if(!fileName.isEmpty()){
        saveFile(fileName);
    }
}

void Dialog::do_file_SaveOrNot(){
//    if(ui->textEdit->document()->isModified()){
        QMessageBox MessageBox;
        MessageBox.setIcon(QMessageBox::Warning);
        MessageBox.setWindowTitle("warning");
        MessageBox.setText(tr("是否保存")+curFile);
        MessageBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        if(MessageBox.exec()==QMessageBox::Yes){
            do_file_Save();
        }
//    }
}

bool Dialog::saveFile(const QString &fileName){
    QFile file(fileName);

    if(!file.open(QFile::WriteOnly|QFile::Text)){
        QMessageBox::warning(this,tr("保存文件"),tr("无法保存文件 %1 \n %2").arg(fileName).arg(file.errorString()));
        return false;
    }else{
//        QTextStream out(&file);
//        out<<pix.toImage();

        isSaved = true;
        pix.save(curFile);
        curFile = QFileInfo(fileName).canonicalFilePath();
        setWindowTitle(curFile);
        return true;
    }
}

void Dialog::on_openBtn_clicked()
{
    do_file_SaveOrNot();
    QString fileName = QFileDialog::getOpenFileName(this);
    if(pix.load(fileName)){
        setWindowTitle(fileName);
        curFile = fileName;
        update();
    }
}

void Dialog::on_saveBtn_clicked()
{
    do_file_Save();
}

void Dialog::on_saveAgainBtn_clicked()
{
    do_file_SaveAs();
}

void Dialog::on_exitBtn_clicked()
{
    do_file_SaveOrNot();
    this->close();
}

//橡皮擦按钮
void Dialog::on_eraseBtn_clicked()
{
   mode = eraseMode;
}

