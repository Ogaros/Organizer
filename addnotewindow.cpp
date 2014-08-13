#include "addnotewindow.h"

addNoteWindow::addNoteWindow( QWidget *parent) :
    QWidget(parent)
{
    this->setUI();
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void addNoteWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();
}

void addNoteWindow::setUI()
{
    //Testing changes
    //----Widgets creation----------------------------------------------
    dateLabel = new QLabel("Date:");
    selectedDate = new QDateEdit();
    noteTextLabel = new QLabel("Note text:");
    noteText = new QTextEdit;
    notifyRButtonGroup = new QButtonGroup;
    notifyGroupBox = new QGroupBox("Notify me in advance");
    notifyRBOnce =  new QRadioButton("Once");
    notifyRBRepeat =  new QRadioButton("Every day");
    notifyOnceDateLabel = new QLabel("At:");
    notifyRepeatedDateLabel = new QLabel("Starting at this date:");
    notifyRepeatedTimeLabel = new QLabel("At:");
    notifyOnceDate = new QDateTimeEdit();
    notifyRepeatedDate = new QDateEdit();
    notifyRepeatedTime = new QTimeEdit();
    //------------------------------------------------------------------

    connect(notifyGroupBox, SIGNAL(toggled(bool)), this, SLOT(hideLayoutItems(bool)));
    noteText->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    //----Layouts creation----------------------------------------------
    mainLayout = new QVBoxLayout;
    notifyLayout = new QVBoxLayout;
    //------------------------------------------------------------------

    //----DateEdits display formats-------------------------------------
    selectedDate->setDisplayFormat("dd.MM(MMM).yyyy");
    notifyOnceDate->setDisplayFormat("hh:mm AP dd.MM(MMM).yyyy");
    notifyRepeatedDate->setDisplayFormat("dd.MM(MMM).yyyy");
    notifyRepeatedTime->setDisplayFormat("hh:mm AP");
    //------------------------------------------------------------------

    //----Adding widgets to notification layout--------------------
    notifyLayout->addWidget(notifyRBOnce);
    notifyLayout->addWidget(notifyRBRepeat);
    notifyLayout->addWidget(notifyOnceDateLabel);
    notifyLayout->addWidget(notifyOnceDate);
    notifyLayout->addWidget(notifyRepeatedTimeLabel);
    notifyLayout->addWidget(notifyRepeatedTime);
    notifyLayout->addWidget(notifyRepeatedDateLabel);
    notifyLayout->addWidget(notifyRepeatedDate);
    notifyOnceDateLabel->hide();
    notifyOnceDate->hide();
    notifyRepeatedTimeLabel->hide();
    notifyRepeatedTime->hide();
    notifyRepeatedDateLabel->hide();
    notifyRepeatedDate->hide();
    connect(notifyRBOnce, SIGNAL(toggled(bool)), notifyOnceDateLabel, SLOT(setVisible(bool)));
    connect(notifyRBOnce, SIGNAL(toggled(bool)), notifyOnceDate, SLOT(setVisible(bool)));
    connect(notifyRBRepeat, SIGNAL(toggled(bool)), notifyRepeatedTimeLabel, SLOT(setVisible(bool)));
    connect(notifyRBRepeat, SIGNAL(toggled(bool)), notifyRepeatedTime, SLOT(setVisible(bool)));
    connect(notifyRBRepeat, SIGNAL(toggled(bool)), notifyRepeatedDateLabel, SLOT(setVisible(bool)));
    connect(notifyRBRepeat, SIGNAL(toggled(bool)), notifyRepeatedDate, SLOT(setVisible(bool)));
    //------------------------------------------------------------------

    //----Resizing window when menu expands-----------------------------
    connect(notifyRBOnce, SIGNAL(toggled(bool)), this, SLOT(resizeMe()));
    connect(notifyRBRepeat, SIGNAL(toggled(bool)), this, SLOT(resizeMe()));
    //------------------------------------------------------------------

    //----Notification groupbox settings and rbgroupbox buttons adding--
    notifyRButtonGroup->addButton(notifyRBOnce);
    notifyRButtonGroup->addButton(notifyRBRepeat);
    notifyGroupBox->setLayout(notifyLayout);
    notifyGroupBox->setCheckable(true);
    notifyGroupBox->setChecked(false);
    //------------------------------------------------------------------

    //----Ok/Cancel buttons and their signals connections---------------
    noteAddButtons = new QDialogButtonBox(QDialogButtonBox::Ok |
                                          QDialogButtonBox::Cancel);
    connect(noteAddButtons, SIGNAL(rejected()),this, SLOT(hide()));
    connect(noteAddButtons, SIGNAL(accepted()),this, SLOT(addNote()));
    //------------------------------------------------------------------

    //----Error label setup---------------------------------------------
    errorLabel = new QLabel;
    errorLabel->setStyleSheet("QLabel { color : red; }");
    //------------------------------------------------------------------

    //----Main layout widgets adding------------------------------------
    mainLayout->addWidget(dateLabel);
    mainLayout->addWidget(selectedDate);
    mainLayout->addWidget(noteTextLabel);
    mainLayout->addWidget(noteText);
    mainLayout->addWidget(notifyGroupBox);
    mainLayout->addWidget(errorLabel);
    mainLayout->addWidget(noteAddButtons);
    //------------------------------------------------------------------

    mainLayout->setAlignment(Qt::AlignTop);
    this->setLayout(mainLayout);
    noteText->setFocus();
}

void addNoteWindow::changeDate(const QDate& date)
{
    selectedDate->setDate(date);
    notifyOnceDate->setDate(date);
}

void addNoteWindow::hideLayoutItems(const bool on)
{
    if(on)
    {
        notifyRBOnce->show();
        notifyRBRepeat->show();
    }
    else
    {
        notifyRButtonGroup->setExclusive(false);
        notifyRBOnce->setChecked(false);
        notifyRBRepeat->setChecked(false);
        notifyRButtonGroup->setExclusive(true);
        notifyRBOnce->hide();
        notifyRBRepeat->hide();
        notifyOnceDateLabel->hide();
        notifyOnceDate->hide();
        notifyRepeatedTimeLabel->hide();
        notifyRepeatedTime->hide();
        notifyRepeatedDateLabel->hide();
        notifyRepeatedDate->hide();
    }
    QTimer::singleShot(1, this, SLOT(resizeMe()));
}

void addNoteWindow::resizeMe()
{
    this->resize(this->minimumSizeHint());
    noteText->resize(noteText->minimumSize());
    noteText->resize(noteText->sizeHint());
}

void addNoteWindow::addNote()
{
    bool hasErrors = false;
    errorLabel->clear();
    QDate nDate = selectedDate->date();
    QString nText = noteText->toPlainText();
    if(nText.size() == 0)
    {
        errorLabel->setText(errorLabel->text() + "\n*Please add some text to the note.");
        hasErrors = true;
    }
    bool hasNotification = false;
    bool isRepeated;
    QDate notifDate;
    QTime notifTime;
    if(hasNotification = notifyGroupBox->isChecked())
    {
        if(notifyRBRepeat->isChecked())
        {
            isRepeated = true;
            notifDate = notifyRepeatedDate->date();
            notifTime = notifyRepeatedTime->time();
        }
        else if(notifyRBOnce->isChecked())
        {
            isRepeated = false;
            notifDate = notifyOnceDate->date();
            notifTime = notifyOnceDate->time();
        }
        else
        {
            errorLabel->setText(errorLabel->text() + "\n*You should pick either repeated or one time notification.");
            hasErrors = true;
        }
    }
    if(hasErrors)
    {
        errorLabel->setText(errorLabel->text().trimmed());
        errorLabel->show();
    }
    else
    {
        Note *n = currentNote;
        bool isNew = (n == nullptr);
        if(isNew)
        {
            n = new Note();
        }
        n->nDate = nDate;
        n->nText = nText;
        if(n->isNotified = hasNotification)
        {
            n->notifDate = notifDate;
            n->notifTime = notifTime;
        }
        emit noteAdded(n, isNew);
        emit noteAdded(n->nDate);
        this->hide();
    }
}

void addNoteWindow::loadFields(const QDate& date, Note *n)
{
    currentNote = n;
    errorLabel->hide();
    if(n == nullptr)
    {
        this->setWindowTitle("Add a new note");
        selectedDate->setDate(date);
        notifyOnceDate->setDate(date);
        notifyRepeatedDate->setDate(date);
        notifyGroupBox->setChecked(false);
        notifyRButtonGroup->setExclusive(false);
        notifyRBOnce->setChecked(false);
        notifyRBRepeat->setChecked(false);
        notifyRButtonGroup->setExclusive(true);

        noteText->clear();
    }
    else
    {
        this->setWindowTitle("Edit note");
        selectedDate->setDate(n->nDate);
        noteText->setText(n->nText);
        if(n->isNotified)
        {
            notifyGroupBox->setChecked(true);
            if(n->isRepeated)
            {
                notifyRBRepeat->setChecked(true);
                notifyRBOnce->setChecked(false);
                notifyRepeatedDate->setDate(n->notifDate);
                notifyRepeatedTime->setTime(n->notifTime);
            }
            else
            {
                notifyRBRepeat->setChecked(false);
                notifyRBOnce->setChecked(true);
                notifyOnceDate->setDate(n->notifDate);
                notifyOnceDate->setTime(n->notifTime);
            }
        }
        else
        {
            notifyGroupBox->setChecked(false);
            notifyRButtonGroup->setExclusive(false);
            notifyRBOnce->setChecked(false);
            notifyRBRepeat->setChecked(false);
            notifyRButtonGroup->setExclusive(true);
            notifyOnceDate->setDate(date);
            notifyRepeatedDate->setDate(date);
        }
    }
}
