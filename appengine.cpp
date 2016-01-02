#include "appengine.h"
#include <QDebug>
void AppEngine::createPlaylist()
{
    playlist.addSound(std::shared_ptr<Sound>
                        {new FileSound(SoundSample::Frequency::Hz1000,
                         QString(":/soundSamples/soundSamples/Hz1000Left.wav"),
                         QString(":/soundSamples/soundSamples/Hz1000Right.wav"))});
    playlist.addSound(std::shared_ptr<Sound>
                        {new FileSound(SoundSample::Frequency::Hz250,
                         QString(":/soundSamples/soundSamples/Hz250Left.wav"),
                         QString(":/soundSamples/soundSamples/Hz250Right.wav"))});
    playlist.addSound(std::shared_ptr<Sound>
                        {new FileSound(SoundSample::Frequency::Hz500,
                         QString(":/soundSamples/soundSamples/Hz500Left.wav"),
                         QString(":/soundSamples/soundSamples/Hz500Right.wav"))});
//    playlist.addSound(std::shared_ptr<Sound>                                      //use only if the hardware quality is good
//                        {new FileSound(SoundSample::Frequency::Hz125,
//                         QString(":/soundSamples/soundSamples/Hz125Left.wav"),
//                         QString(":/soundSamples/soundSamples/Hz125Right.wav"))});
    playlist.addSound(std::shared_ptr<Sound>
                        {new FileSound(SoundSample::Frequency::Hz8000,
                         QString(":/soundSamples/soundSamples/Hz8000Left.wav"),
                         QString(":/soundSamples/soundSamples/Hz8000Right.wav"))});
    playlist.addSound(std::shared_ptr<Sound>
                        {new FileSound(SoundSample::Frequency::Hz2000,
                         QString(":/soundSamples/soundSamples/Hz2000Left.wav"),
                         QString(":/soundSamples/soundSamples/Hz2000Right.wav"))});
    playlist.addSound(std::shared_ptr<Sound>
                        {new FileSound(SoundSample::Frequency::Hz4000,
                         QString(":/soundSamples/soundSamples/Hz4000Left.wav"),
                         QString(":/soundSamples/soundSamples/Hz4000Right.wav"))});



    volumesHL->setDecibelHearingLevelCalibrationGain(SoundSample::Frequency::Hz125, 45.0);
    volumesHL->setDecibelHearingLevelCalibrationGain(SoundSample::Frequency::Hz250, 15.0);
    volumesHL->setDecibelHearingLevelCalibrationGain(SoundSample::Frequency::Hz500, 9.0);
    volumesHL->setDecibelHearingLevelCalibrationGain(SoundSample::Frequency::Hz1000, 3.0);
    volumesHL->setDecibelHearingLevelCalibrationGain(SoundSample::Frequency::Hz2000, -3.0);
    volumesHL->setDecibelHearingLevelCalibrationGain(SoundSample::Frequency::Hz4000, -4.0);
    volumesHL->setDecibelHearingLevelCalibrationGain(SoundSample::Frequency::Hz8000, 13.0);


    volumesHL->addVolume(70);
    volumesHL->addVolume(60);
    volumesHL->addVolume(50);
    volumesHL->addVolume(40);
    volumesHL->addVolume(30);
    volumesHL->addVolume(20);
    volumesHL->addVolume(10);
    volumesHL->addVolume(MIN_AVAILABLE_VOLUME_DB);
    volumesHL->addVolume(MAX_AVAILABLE_VOLUME_DB);
    volumesHL->addVolume(90);
    volumesHL->addVolume(LAST_AVAILABLE_VOLUME_DB);

    volumesSPL->addVolume(70);
    volumesSPL->addVolume(60);
    volumesSPL->addVolume(50);
    volumesSPL->addVolume(40);
    volumesSPL->addVolume(30);
    volumesSPL->addVolume(20);
    volumesSPL->addVolume(10);
    volumesSPL->addVolume(MIN_AVAILABLE_VOLUME_DB);
    volumesSPL->addVolume(MAX_AVAILABLE_VOLUME_DB);
    volumesSPL->addVolume(90);
    volumesSPL->addVolume(LAST_AVAILABLE_VOLUME_DB);


    playlist.setVolumeAlgoritm(volumesHL);
}

void AppEngine::resetVariables()
{
    if(currentDirection == SSDir::None)
    {
        audiogramPlotDataLeft.clear();
        audiogramPlotDataRight.clear();
    }
}

void AppEngine::connectAll()
{
    connect(rootObj, SIGNAL(playSequence(int)), this, SLOT(playPlaylist(int)));
    connect(rootObj, SIGNAL(stopPlaying()), this, SLOT(stopPlaying()));
    connect(rootObj, SIGNAL(heardButtonClicked()), this, SLOT(onHearingButtonClicked()));
    connect(rootObj, SIGNAL(saveFileRequest(QUrl)), this, SLOT(saveFileRequest(QUrl)));
    connect(rootObj, SIGNAL(calibrationRequest(int)), this, SLOT(calibrationRequest(int)));
    connect(rootObj, SIGNAL(calibrationDeclined()), calibrationPlayer, SLOT(stop()));
    connect(rootObj, SIGNAL(calibrationPlayRequest()), this, SLOT(calibrationPlayRequest()));
    connect(player, SIGNAL(currentPlaylistElement(AudiogramData)), this, SLOT(onCurrentPlaylistElement(AudiogramData)));
    connect(player, SIGNAL(playlistEnded()), this, SLOT(onPlaylistEnded()));
    connect(player, SIGNAL(aboutToPlayNextElement()), this, SLOT(onAboutToPlayNextElement()));
}

void AppEngine::setTopBarMsgTimeOut(const QString &text, int msec)
{
    tempTopBarMsg = topBarMsg();
    setTopBarMsg(text);
    QTimer::singleShot(msec, this, SLOT(restoreTopBarMsg()));
}

bool AppEngine::calibrationAllowed() const
{
    return m_calibrationAllowed;
}

AppEngine::AppEngine(QObject *parent)
    : QObject(parent), rootObj(nullptr),
      volumesPercent{new VolumePercentLevel},
      volumesSPL{new VolumeDecibelSoundPressureLevel},
      volumesHL{new VolumeDecibelHearingLevel}
{
    audioFormat.setSampleRate(44100);
    audioFormat.setChannelCount(2);
    audioFormat.setSampleSize(16);
    audioFormat.setCodec("audio/pcm");
    audioFormat.setByteOrder(QAudioFormat::LittleEndian);
    audioFormat.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(audioFormat))
        audioFormat = info.nearestFormat(audioFormat);

    player = new SoundPlayer(audioFormat, QAudioDeviceInfo::defaultOutputDevice(), this);
    createPlaylist();
    player->setPlaylist(&playlist);
    algorithm = new AudiometryAlgorithm(player);

    calibrationSample = new FileSound(SoundSample::Frequency::Hz1000,
                                     QString(":/soundSamples/soundSamples/Hz1000Left.wav"),
                                     QString(":/soundSamples/soundSamples/Hz1000Right.wav"));
    calibrationPlayer = new SingleFilePlayer(audioFormat, QAudioDeviceInfo::defaultOutputDevice(), this);
    calibrationPlayer->setFileSound(calibrationSample);
    calibrationPlayer->setVolume(VolumeAlgorithm::decibelToSoundPressureLevel(68));
}

AppEngine::~AppEngine()
{
    delete calibrationPlayer;
    delete calibrationSample;
    delete player;
}

void AppEngine::setRootQmlObject(QObject *rootQmlObj)
{
    rootObj = rootQmlObj;
}

void AppEngine::onPlaylistEnded()
{
    player->resetPlaylist();

    switch(currentDirection)
    {
    case SSDir::Left:
        audiogramPlotDataLeft = algorithm->getAudiogramPlotData();
        currentDirection = SSDir::None;
        break;

    case SSDir::Right:
        audiogramPlotDataRight = algorithm->getAudiogramPlotData();
        currentDirection = SSDir::None;
        break;

    case SSDir::OneAfterAnother:
        audiogramPlotDataLeft = algorithm->getAudiogramPlotData();
        algorithm->resetAll();
        resetVariables();
        currentDirection = SSDir::Right;
        player->playPlaylist(currentDirection);
        setTopBarMsg(tr("Right Channel"));
        return;
        break;

    case SSDir::None:
    default: setCalibrationAllowed(true); break;
    }

    emit playlistEnded();
}

void AppEngine::playPlaylist(int direction)
{
    resetVariables();
    algorithm->resetAll();
    setCalibrationAllowed(false);
    switch(direction)
    {
    case static_cast<int>(SSDir::Left):
        player->playPlaylist(SSDir::Left);
        currentDirection = SSDir::Left;
        setTopBarMsg(tr("Left Channel"));
        break;

    case static_cast<int>(SSDir::Right):
        player->playPlaylist(SSDir::Right);
        currentDirection = SSDir::Right;
        setTopBarMsg(tr("Right Channel"));
        break;

    case static_cast<int>(SSDir::OneAfterAnother):
        currentDirection = SSDir::OneAfterAnother;
        player->playPlaylist(SSDir::Left);
        setTopBarMsg(tr("Left Channel"));
        break;

    case static_cast<int>(SSDir::None):
    default: setTopBarMsg(tr("Channel does not exist")); setCalibrationAllowed(true); break;
    }
}

void AppEngine::onCurrentPlaylistElement(const AudiogramData &data)
{
    algorithm->onCurrentPlaylistElement(data);
}

void AppEngine::onHearingButtonClicked()
{
    algorithm->onHearingButtonClicked();
}

void AppEngine::onAboutToPlayNextElement()
{
    algorithm->onAboutToPlayNextElement();
}

void AppEngine::setTopBarMsg(QString topBarMsg)
{
    if (m_topBarMsg == topBarMsg)
        return;

    m_topBarMsg = topBarMsg;
    emit topBarMsgChanged(topBarMsg);
}

void AppEngine::restoreTopBarMsg()
{
    setTopBarMsg(tempTopBarMsg);
}

void AppEngine::saveFileRequest(const QUrl &url)
{
    AudiogramChart chart(1024, 768);
    chart.setDataLeft(audiogramPlotDataLeft);
    chart.setDataRight(audiogramPlotDataRight);
    if(chart.saveImage(url))
        setTopBarMsgTimeOut(tr("File saved!"), MSG_TIME_OUT_MSEC);
    else
        setTopBarMsgTimeOut(tr("Save error :("), MSG_TIME_OUT_MSEC);
}

void AppEngine::calibrationRequest(int decibel)
{
    volumesHL->setVolumeGainDB(decibel);
    volumesSPL->setVolumeGainDB(decibel);
    calibrationPlayer->stop();
}

void AppEngine::calibrationPlayRequest()
{
    if(isAnythingPlayingNow())
    {
        calibrationPlayer->start();
    }
}

void AppEngine::stopPlaying()
{
    player->stopPlaylist();
    currentDirection = SSDir::None;
}

void AppEngine::setCalibrationAllowed(bool calibrationAllowed)
{
    if (m_calibrationAllowed == calibrationAllowed)
        return;

    m_calibrationAllowed = calibrationAllowed;
    emit calibrationAllowedChanged(calibrationAllowed);
}
/*!
 * \brief AppEngine::onCurrentPlaylistElement Slot: Gets current AudiogramData
 * \param data Audiogram data
 * This function is the first step in user interaction and data Absoulte
 * Threshold of Hearing algorithm.
 *
 * The function resets appropriately the variables and checks,
 * whether current volume is maximal. These variables are therefore
 * interpreted in onAboutToPlayNextElement();
 */

void AudiometryAlgorithm::onCurrentPlaylistElement(const AudiogramData &data)
{
    hearButtonPressed = false;
    canSkipTrack = true;
    currentAudiogramData = data;
    if(currentAudiogramData.getVolumeDb() == MAX_AVAILABLE_VOLUME_DB)
    {
        audiogramDataTemp.append(currentAudiogramData);
        canSkipTrackMaxVol = true;
    }
}

/*!
 * \brief AppEngine::onHearingButtonClicked Slot: Interacts with user input in gathering data
 *
 * This function is a slot for a heardButtonClicked() in QML and is responsible for
 * interaction between the algorithm and the user.
 * The function appends current AudiogramData pending into a buffer and
 * sets flag variables in order to influence onAboutToPlayNextElement();
 */
void AudiometryAlgorithm::onHearingButtonClicked()
{
    if(!hearButtonPressed)
        audiogramDataTemp.append(currentAudiogramData);
    hearButtonPressed = true;
    canSkipTrack = false;
    canSkipTrackMaxVol = false;
    if(currentAudiogramData == previousAudiogramData)
        canSkipMaxDueToButtonPressed = true;
}

/*!
 * \brief AppEngine::onAboutToPlayNextElement Slot.
 */
void AudiometryAlgorithm::onAboutToPlayNextElement()
{
    //the last volume if clicked is added to the chart
    if(rint(currentAudiogramData.getVolumeDb())
            == LAST_AVAILABLE_VOLUME_DB
            && !canSkipTrack)
    {
        audiogramPlotData.update(currentAudiogramData);
        audiogramDataTemp.clear();
    }

    //skip the track if max volume is reached and no button pressed
    if(canSkipTrackMaxVol && !audiogramDataTemp.isEmpty())
    {
        updateAudiogramPlotData();

        player->skipCurrentSoundSet();
        canSkipTrackMaxVol = false;
        return;
    }

    //create a chart in a normal way of user interaction
    //from LAST_AVAILABLE_VOLUME_DB - 10dB up to 0 dB
    if(canSkipTrack && !audiogramDataTemp.isEmpty())
    {
        updateAudiogramPlotData();

        if(currentAudiogramData.getVolumeDb()
                != LAST_AVAILABLE_VOLUME_DB)
            player->skipCurrentSoundSet();
    }

    //skip the maximal volume if the button was ever
    //pressed in this frequency currently played
    if(rint(currentAudiogramData.getVolumeDb())
            == MIN_AVAILABLE_VOLUME_DB
            && canSkipMaxDueToButtonPressed
            && !audiogramDataTemp.isEmpty())
    {
        updateAudiogramPlotData();

        player->skipCurrentSoundSet();
        canSkipTrackMaxVol = false;
    }

    //reset on a new freq
    //max volume cannot be the first value or the last value
    //max value must be placed after the minimal
    if(currentAudiogramData != previousAudiogramData)
        canSkipMaxDueToButtonPressed = false;
    previousAudiogramData = currentAudiogramData;
}

/*!
 * \brief AudiometryAlgorithm::getAudiogramPlotData Gets AudiogramPlotData as a result of the algorithm
 * \return AudiogramPlotData
 */
AudiogramPlotData AudiometryAlgorithm::getAudiogramPlotData() const
{
    return audiogramPlotData;
}

/*!
 * \brief AudiometryAlgorithm::resetAll Restores variables to their default values
 */
void AudiometryAlgorithm::resetAll()
{
    audiogramPlotData.clear();
    audiogramDataTemp.clear();
    currentAudiogramData = AudiogramData();
    previousAudiogramData = AudiogramData();
    canSkipTrack = false;
    canSkipTrackMaxVol = false;
}

void AudiometryAlgorithm::updateAudiogramPlotData()
{
    auto audiogramData = audiogramDataTemp.takeLast();
    audiogramDataTemp.clear();
    audiogramPlotData.update(audiogramData);
}
