#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "widgetList.h"
#include "orgcalendar.h"
#include "editwindow.h"
#include "deletedialogue.h"
#include "settingswindow.h"
#include "note.h"
#include "notebook.h"
#include "memory"
#include "settings.h"

enum class DeleteOption{One, Outdated, All};
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:
    void noteDeleted();

public slots:
    void showEditWindow();
    void addNote(Note *n, const bool);
    void showNotes();
    void showClosestNote();
    void saveNotes();
    void deleteNoteDialogue();
    void deleteAll();
    void deleteOutdated();
    void showSettings();
    int showDeleteMessageBox(const DeleteOption);
    int showExitWOSavingMessageBox();

private slots:
    void switchButtons();    
    void resizeMe();
    void closeProgram();
    void showFromTray();
    void iconActivated(QSystemTrayIcon::ActivationReason);
    void showTrayMessage();
    void refreshDateFormat();

protected:
    void closeEvent(QCloseEvent *event);


private:
    void setUI();
    void moveToCenter(QWidget *);
    void createMenu();
    void createTrayIcon();
    void createStatusBar();
    void createButtonLayout();
    void createCalendar();
    void createScrollArea();
    void createEditWindow();
    void createSettingsWindow();
    void addNoteLabel();
    void deleteNote(Note *);
    void resizeTimer();
    bool isChanged;
    bool isClosing;

    std::shared_ptr<Notebook>       notes;
    std::shared_ptr<Settings>       settings;
    QList<QLabel*>                  noteLabels;
    std::unique_ptr<EditWindow>     editWindow;
    std::unique_ptr<SettingsWindow> settingsWindow;    
    QHBoxLayout                     *mainLayout;
    QVBoxLayout                     *leftLayout;
    QVBoxLayout                     *buttonsLayout;
    orgCalendar                     *cal;
    QPushButton                     *addButton;
    QPushButton                     *editButton;
    QPushButton                     *removeButton;
    QPushButton                     *todayButton;
    QPushButton                     *quitButton;
    QLabel                          *noteTextTitle;
    QScrollArea                     *scrollArea;
    QVBoxLayout                     *scrollLayout;
    QLabel                          *statusLabel;
    QSystemTrayIcon                 *trayIcon;
    QMenu                           *trayIconMenu;
    QAction                         *quitAction;
    QAction                         *notificationsAction;
    QAction                         *openAction;
    QTimer                          *trayTimer;
};

#endif // MAINWINDOW_H
