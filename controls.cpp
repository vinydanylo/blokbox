#include "controls.h"
#include "ui_controls.h"
#include <QMouseEvent>
//#include <QDebug>

// controls constructor
// starts up all stuff
Controls::Controls(QWidget *parent):
  AbstractControl(parent), ui(new Ui::Controls){
  ui->setupUi(this);

  // when the user releases the slider, tells qt a
  // new position was chosen
  connect(ui->horizontalSliderPosition,SIGNAL(sliderReleased()),
          this,SLOT(onSliderReleased()));

  // duration records total music time to be played
  duration=1;

  // the horizontal slider is initially disabled, since no music has
  // been loaded
  ui->horizontalSliderPosition->setDisabled(true);

  // we want no focus on some widgets
  // (qt may draw a rectangle on them in such case)
  ui->pushButtonPlayPause->setFocusPolicy(Qt::NoFocus);
  ui->pushButtonNext->setFocusPolicy(Qt::NoFocus);
  ui->pushButtonPrev->setFocusPolicy(Qt::NoFocus);
  ui->horizontalSliderVolume->setFocusPolicy(Qt::NoFocus);
  // nice red colors to lcdnumbers
  ui->lcdNumberElapsed->setStyleSheet(QString("QLCDNumber{color:red;}"));
  ui->lcdNumberDuration->setStyleSheet(QString("QLCDNumber{color:red;}"));
  playIcon = QIcon(":/resources/play.svg");
  pauseIcon = QIcon(":/resources/pause.svg");
}

// put the trash on the trash
Controls::~Controls(){
  delete ui;
}

// deals with slider release events
void Controls::onSliderReleased(){
  // when the user releases the slider, the component tells qt
  // the new position in percentual values
  //
  // we should not care about ajust lcdnumber here
  // wait until player tell us the stream position has changed
  emit elapsedSelected(ui->horizontalSliderPosition->value()*duration/100);
}

// the folowing method is (almost) self-explained
void Controls::onPlayPauseClicked(void){
  emit playPause();
}

// the folowing method is (almost) self-explained
void Controls::onPrevClicked(){
  emit prev();
}

// the folowing method is (almost) self-explained
void Controls::onNextClicked(){
  emit next();
}

// tells current volume
void Controls::onVolumeChanged(int value){
  // if slider is not down it means someone else
  // is selecting the current volume
  if(!ui->horizontalSliderVolume->isSliderDown()){
    ui->horizontalSliderVolume->setValue(value);
  }
  else{
    // if slider is down the user is
    // selecting the volume in this component
    emit volumeSelected(value);
  }
}

// music duration has changed (current media has changed)
void Controls::onDurationChanged(qint64 value){
  ui->horizontalSliderPosition->setEnabled(true);
  duration = value;
  // display the new duration in lcdnumber
  ui->lcdNumberDuration->display(QTime(0,0).addMSecs(value).toString(QString("hh:mm:ss")));
}

// player state has changed...
void Controls::onPlayerStateChanged(bool state){
  if(state == true){
    // if playing, display pause icon
    ui->pushButtonPlayPause->setIcon(pauseIcon);
  }
  else{
    // if paused, display play icon (duh!)
    ui->pushButtonPlayPause->setIcon(playIcon);
  }

}

// elapsed time has changed
void Controls::onElapsedChanged(qint64 value){
  // adjust the elapsed time on lcdnumber
  ui->lcdNumberElapsed->display(QTime(0,0).addMSecs(value).toString(QString("hh:mm:ss")));
  // if slider is not down, it means someone else is changing
  // elapsed time
  if(!ui->horizontalSliderPosition->isSliderDown()){
    // position slider in the new elapsed time
    ui->horizontalSliderPosition->setValue(100*value/duration);
  }
}



