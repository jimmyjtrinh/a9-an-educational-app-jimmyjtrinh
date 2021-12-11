#include "model.h"
#include <QBrush>


//remove later
#include <iostream>
using namespace std;


/*!
 * \brief Model::Model Method constructs model object and initializes paramaeters
 * \param parent
 */
Model::Model(QObject *parent) : QObject(parent)
{
    resetFaces();
    bestTime.setHMS(0,0,0);
    connect(timer, &QTimer::timeout, this, &Model::updateCountdown);


}


/*!
 * \brief Model::makeGrid makes the grid for the sprite drawing.
 * \param canvasSize size of the canvas/preview drawing of sprite
 */
void Model::makeGrid(Faces face ){
    // add grid lines on top of sprite image's pixmap
    QPixmap pixmap(QPixmap::fromImage(face.getImage()).scaled(120, 120, Qt::KeepAspectRatio));
    QPainter painter(&pixmap);
    // use black as the line color
    //painter.setPen(QColor(0, 0, 0, 255));
    painter.setPen(QPen(Qt::black, 3));


    // draw vertical and horizontal lines
    for(float lineIndex = 0; lineIndex <= pixmap.width(); lineIndex+=pixmap.width()/3){
        //vertical lines
        painter.drawLine(lineIndex, 0, lineIndex, pixmap.height());
        //horizontal lines
        painter.drawLine(0, lineIndex, pixmap.width(), lineIndex);

    }
    // send grid to view to draw
    if (face.getName() == "front"){
        emit sendFrontGrid(pixmap);
    } else if (face.getName() == "back"){
        emit sendBackGrid(pixmap);
    } else if (face.getName() == "up"){
        emit sendUpGrid(pixmap);
    } else if (face.getName() == "down"){
        emit sendDownGrid(pixmap);
    } else if (face.getName() == "left"){
        emit sendLeftGrid(pixmap);
    } else {
        emit sendRightGrid(pixmap);
    }
}

/*!
 * \brief Model::updateFaces Redraw the grids and repaint faces of rubix
 */
void Model::updateFaces(){
    makeGrid(front);
    makeGrid(back);
    makeGrid(up);
    makeGrid(down);
    makeGrid(left);
    makeGrid(right);

    if(isScrambled){
        if(isSolved()){
            isScrambled = false;
            //do some sort of celebration here
            emit showAnimation();
        }
    }


    save();
}

/*!
 * \brief Model::isSolved Method goes through all faces and makes sure they all match the middle piece. Checks if rubiks is solved
 * \return true if cube is solved false otherwise
 */\
bool Model::isSolved(){
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            // front face surrounding check
            if (!(front.getColorCorrespondingToInt(i, j) == front.getColorCorrespondingToInt(1,1))){
                return false;
            }
            // back face surrounding check
            else if (!(back.getColorCorrespondingToInt(i, j) ==  back.getColorCorrespondingToInt(1,1))){
                return false;
            }
            // left face surrounding check
            else if (!(left.getColorCorrespondingToInt(i, j) == left.getColorCorrespondingToInt(1,1))){
                return false;
            }
            // right face surrounding check
            else if (!(right.getColorCorrespondingToInt(i, j) == right.getColorCorrespondingToInt(1,1))){
                return false;
            }
            // up face surrounding check
            else if (!(up.getColorCorrespondingToInt(i, j) ==  up.getColorCorrespondingToInt(1,1))){
                return false;
            }
            // down face surrounding check
            else if (!(down.getColorCorrespondingToInt(i, j) == down.getColorCorrespondingToInt(1,1))){
                return false;
            }

        }
    }
    stopUpdateCountdown();

    return true;
}

/*!
 * \brief Model::frontMove makes a single turn, (90°), clockwise, of the front face.
 */
void Model::frontMove(){

    QVector<QColor> temp = left.getCol(2); //sets up to left row
    QVector<QColor> temp2 = down.getRow(0); //sets up to left row
    QVector<QColor> temp3 = right.getCol(0); //sets up to left row
    QVector<QColor> temp4 = up.getRow(2); //sets up to left row
    QVector<QColor> temp5;
    QVector<QColor> temp6;


    for(int i =0; i < 3; i++){
        temp5.push_front(temp3[i]);
        temp6.push_front(temp[i]);
    }

    up.swapRow(2, temp6);
    left.swapColumn(2, temp2);
    down.swapRow(0, temp5);
    right.swapColumn(0, temp4);

    //rotate front 90 degrees;
    front.rotateClockwise();

    updateFaces();
}

/*!
 * \brief Model::rightMove makes a single turn, (90°), clockwise, of the right face.
 */
void Model::rightMove(){

    QVector<QColor> temp = up.getCol(2); //sets up to left row
    QVector<QColor> temp2 = front.getCol(2); //sets up to left row
    QVector<QColor> temp3 = down.getCol(2); //sets up to left row
    QVector<QColor> temp4 = back.getCol(0); //sets up to left row
    QVector<QColor> temp5;
    QVector<QColor> temp6;

    for(int i =0; i < 3; i++){
        temp5.push_front(temp4[i]);
        temp6.push_front(temp[i]);
    }

    up.swapColumn(2, temp2);
    front.swapColumn(2, temp3);
    down.swapColumn(2, temp5);
    back.swapColumn(0, temp6);

    //rotate front 90 degrees;
    right.rotateClockwise();

    updateFaces();
}

/*!
 * \brief Model::upMove makes a single turn, (90°), clockwise, of the up face.
 */
void Model::upMove(){
    //left front right back

    QVector<QColor> temp = left.getRow(0);
    QVector<QColor> temp2 = front.getRow(0);
    QVector<QColor> temp3 = right.getRow(0);
    QVector<QColor> temp4 = back.getRow(0);


    left.swapRow(0, temp2);
    front.swapRow(0, temp3);
    right.swapRow(0, temp4);
    back.swapRow(0, temp);

    //rotate front 90 degrees;
    up.rotateClockwise();

    updateFaces();
}

/*!
 * \brief Model::backMove makes a single turn, (90°), clockwise, of the back face.
 */
void Model::backMove()
{
    // QVector<QColor> temp = up.swapRow(2, left.getCol(2)); //sets up to left row
    QVector<QColor> temp = left.getCol(0); //sets up to left row
    QVector<QColor> temp2 = down.getRow(2); //sets up to left row
    QVector<QColor> temp3 = right.getCol(2); //sets up to left row
    QVector<QColor> temp4 = up.getRow(0); //sets up to left row
    QVector<QColor> temp5;
    QVector<QColor> temp6;


    for(int i =0; i < 3; i++){
        temp5.push_front(temp4[i]);
        temp6.push_front(temp2[i]);
    }

    up.swapRow(0, temp3);
    left.swapColumn(0, temp5);
    down.swapRow(2, temp);
    right.swapColumn(2, temp6);

    //rotate front 90 degrees;
    back.rotateClockwise();

    updateFaces();

}

/*!
 * \brief Model::leftMove makes a single turn, (90°), clockwise, of the left face.
 */
void Model::leftMove(){

    QVector<QColor> temp = up.getCol(0); //sets up to left row
    QVector<QColor> temp2 = front.getCol(0); //sets up to left row
    QVector<QColor> temp3 = down.getCol(0); //sets up to left row
    QVector<QColor> temp4 = back.getCol(2); //sets up to left row
    QVector<QColor> temp5;
    QVector<QColor> temp6;

    for(int i =0; i < 3; i++){
        temp5.push_front(temp4[i]);
        temp6.push_front(temp3[i]);
    }

    up.swapColumn(0, temp5);
    front.swapColumn(0, temp);
    down.swapColumn(0, temp2);
    back.swapColumn(2, temp6);

    //rotate front 90 degrees;
    left.rotateClockwise();

    updateFaces();
}

/*!
 * \brief Model::downMove makes a single turn, (90°), clockwise, of the down face.
 */
void Model::downMove(){

    QVector<QColor> temp = back.getRow(2);
    QVector<QColor> temp2 = front.getRow(2);
    QVector<QColor> temp3 = left.getRow(2);
    QVector<QColor> temp4 = right.getRow(2);

    left.swapRow(2, temp);
    front.swapRow(2, temp3);
    right.swapRow(2, temp2);
    back.swapRow(2, temp4);
    //rotate front 90 degrees;
    down.rotateClockwise();

    updateFaces();
}

/*!
 * \brief Model::frontMovePrime does F' move to the rubix cube, updating faces accordingly
 */
void Model::frontMovePrime(){

    for(int i =0; i<3;i++)
        frontMove();
}

/*!
 * \brief Model::rightMovePrime does R' move to the rubix cube, updating faces accordingly
 */
void Model::rightMovePrime(){
    for(int i =0; i<3;i++)
        rightMove();
}

/*!
 * \brief Model::upMovePrime does U' move to the rubix cube, updating faces accordingly
 */
void Model::upMovePrime(){
    for(int i =0; i<3;i++)
        upMove();
}

/*!
 * \brief Model::backMovePrime does B' move to the rubix cube, updating faces accordingly
 */
void Model::backMovePrime(){
    for(int i =0; i<3;i++)
        backMove();
}

/*!
 * \brief Model::leftMovePrime does L' move to the rubix cube, updating faces accordingly
 */
void Model::leftMovePrime(){
    for(int i =0; i<3;i++)
        leftMove();
}

/*!
 * \brief Model::downMovePrime does D' move to the rubix cube, updating faces accordingly
 */
void Model::downMovePrime(){
    for(int i =0; i<3;i++)
        downMove();
}



/*!
 * \brief Model::scramble method scrambles rubix cube faces such that rubix cube is randomized
 */
void Model::scramble(){
    for(int i=0; i< 1; i++){    //*******************************************scrable is set to 1 for testing purposes while working with celebration screen, change back to 20****************************
        // get random number that corresponds to a value
        int v = QRandomGenerator::global()->bounded(1, 7);

        switch(v){
        case 1:
            leftMove();
            break;
        case 2:  frontMove();
            break;
        case 3:  rightMove();
            break;
        case 4: upMove();
            break;
        case 5: backMove();
            break;
        case 6: downMove();
            break;
        }
    }
    isScrambled = true;
    timer->start(1000);
    time.setHMS(0,0,0);
}


/*!
 * \brief Model::resetFaces method resets faces of rubix to be unscrambles
 */
void Model::resetFaces(){
    isScrambled = false;
    fixTimerIfResetPressed();
    front = Faces(Qt::green, "front");
    back = Faces(Qt::blue, "back");
    up = Faces(Qt::white, "up"); //QColor(255,165,0)
    down = Faces(Qt::yellow, "down");
    left = Faces(QColor(255,165,0), "left");
    right = Faces(Qt::red, "right");
    updateFaces();
    emit resetCube();
}

void Model::save(){
    QImage newimg =  QImage(6,9, QImage::Format_RGBA64);
    QImage temp =  QImage(3,3, QImage::Format_RGBA64);

    QTransform tempTransform;
    tempTransform.rotate(-270);


    //top half
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            temp = left.getImage();
            newimg.setPixelColor(i, j,  temp.transformed(tempTransform).mirrored(false, true).pixelColor(i,j));
        }
    }

    for(int i = 0; i < 3; i++){
        for(int j = 3; j < 6; j++){
            temp = down.getImage();
            newimg.setPixelColor(i, j, temp.transformed(tempTransform).mirrored(false, true).pixelColor(i,j-3));
        }
    }

    for(int i = 0; i < 3; i++){
        for(int j = 6; j < 9; j++){
            temp = front.getImage();
            newimg.setPixelColor(i, j, temp.transformed(tempTransform).mirrored(false, true).pixelColor(i,j-6));
        }
    }


    //bottom half
    for(int i = 3; i < 6; i++){
        for(int j = 0; j < 3; j++){
            temp = back.getImage();
            newimg.setPixelColor(i, j, temp.transformed(tempTransform).mirrored(false, true).pixelColor(i-3,j));
        }
    }

    for(int i = 3; i < 6; i++){
        for(int j = 3; j < 6; j++){
            temp = up.getImage();
            newimg.setPixelColor(i, j, temp.transformed(tempTransform).mirrored(false, true).pixelColor(i-3,j-3));
        }
    }

    for(int i = 3; i < 6; i++){
        for(int j = 6; j < 9; j++){
            temp = right.getImage();
            newimg.setPixelColor(i, j, temp.transformed(tempTransform).mirrored(false, true).pixelColor(i-3,j-6));
        }
    }

    QTransform transform;
    transform.rotate(90);

    newimg = newimg.scaled(500, 750, Qt::KeepAspectRatio).transformed(transform);

    QPixmap pixmap(QPixmap::fromImage(newimg));
    QPainter painter(&pixmap);
    // use black as the line color
    //painter.setPen(QColor(0, 0, 0, 255));
    painter.setPen(QPen(Qt::black, 6));

    for(float x = 0; x <= pixmap.width(); x+=pixmap.width()/9){
        painter.drawLine(x, 0, x, pixmap.height());
    }
    //horizontal lines
    for(float y = 0; y <= pixmap.height(); y+=pixmap.height()/6){
        painter.drawLine(0, y, pixmap.width(), y);
    }

    newimg = pixmap.toImage();
    newimg.save("filename");
    emit sendImage(newimg);
}

/*!
 * \brief Model::startTutorial Begins tutorial by setting tutorial sequence to first phase
 */
void Model::startTutorial(){
    currStep = 0;
    emit sendStep(currStep);
}

/*!
 * \brief Model::incrementStep method goes one step forward and updates view
 */
void Model::incrementStep(){
    if (currStep <= 5){
        currStep++;
    }
    emit sendStep(currStep);
}

/*!
 * \brief Model::decrementStep method goes one step back and updates view
 */
void Model::decrementStep(){
    if (currStep > 0){
        currStep--;
    }
    emit sendStep(currStep);
}

/*!
 * \brief Model::setStep method takes in steps and send updated step to view
 * \param i the ith step we are setting
 */
void Model::setStep(int i) {
    currStep = i;
    emit sendStep(currStep);
}

/*!
 * \brief Model::rotateRight rotates cube right
 */
void Model::rotateRight(){
    //front -> right -> back -> left
    Faces temp = left;
    left = back;
    left.setName("left");
    back = right;
    back.setName("back");
    right = front;
    right.setName("right");
    front  = temp;
    front.setName("front");
    up.rotateCounterClockwise();
    down.rotateClockwise();
    updateFaces();
}

/*!
 * \brief Model::rotateLeft rotates cube left
 */
void Model::rotateLeft(){
    //front -> right -> back -> left
    for(int i = 0; i<3; i++){
        rotateRight();
    }
}

/*!
 * \brief Model::rotateFlip flips cube
 */
void Model::rotateFlip(){
    Faces frontTemp = front;
    front  = left;
    front.rotateClockwise();
    front.rotateClockwise();
    front.setName("front");
    left = frontTemp;
    left.rotateClockwise();
    left.rotateClockwise();
    left.setName("left");
    Faces backTemp = back;
    back  = right;
    back.rotateClockwise();
    back.rotateClockwise();
    back.setName("back");
    right = backTemp;
    right.rotateClockwise();
    right.rotateClockwise();
    right.setName("right");
    Faces upTemp = up;
    up = down;
    up.rotateCounterClockwise();
    up.setName("up");
    down = upTemp;
    down.rotateClockwise();
    down.setName("down");
    updateFaces();
}

/*!
 * \brief Model::updateCountdown Method is called every second and sends time to view
 */
void Model::updateCountdown()
{
    time = time.addSecs(1);
    // emit time string depending on if highscore is 0
    if(!isBestTimeZero())
        emit sendCurrentTime("current time: " +time.toString("m:ss")+ "\n" + "best time: " + bestTime.toString("m:ss"));
    else
        emit sendCurrentTime("current time: " +time.toString("m:ss"));
}

/*!
 * \brief Model::stopUpdateCountdown stops keeping track of current time and updates best times accordingly
 */
void Model::stopUpdateCountdown()
{
    timer->stop();
    // check if best time is 0, if so simply set best time as the current time, else set best time if needed
    if(!isBestTimeZero())
    {
        if(time<bestTime)
            bestTime = time;
    }
    else
        bestTime = time;

    // send times
    emit sendCurrentTime("best time: " + bestTime.toString("m:ss"));
    time.setHMS(0,0,0);
}

/*!
 * \brief Model::fixTimerIfResetPressed If cube is scrambled we want to make sure the best time does not get touched
 */
void Model::fixTimerIfResetPressed(){
    timer->stop();
    if(!isBestTimeZero())
        emit sendCurrentTime("best time: " + bestTime.toString("m:ss"));
    else
        emit sendCurrentTime("");
    time.setHMS(0,0,0);
}

/*!
 * \brief Model::isBestTimeZero Method checks the content of best time
 * \return returns true if bestTime is clocked to 0, else false
 */
bool Model::isBestTimeZero(){
    return bestTime.minute()==0 && bestTime.second()==0;
}
