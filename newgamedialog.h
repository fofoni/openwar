#ifndef NEWGAMEDIALOG_H
#define NEWGAMEDIALOG_H

#include <QDialog>
#include <QLayout>
#include <QtGui>

class NewGameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewGameDialog(QWidget *parent = 0);
    bool run(int& qtd, QList<QString>& names, QList<QColor>& colors);

    int num_players;
    QList<QString> player_names;
    QList<QColor> player_colors;

private:
    static const int MIN_PLAYERS = 2;
    static const int MAX_PLAYERS = 6;

    QSignalMapper* signal_mapper;

    QSpinBox *num_players_box;
    QList<QLabel *> player_labels;
    QList<QLineEdit *> player_names_edit;
    QList<QComboBox *> player_colors_combo;
    QDialogButtonBox *button_box;

    bool names_are_ok();
    bool colors_are_ok();

private slots:
    void set_num_players(int n);
    void set_names_list();
    void set_colors_list();

};

#endif // NEWGAMEDIALOG_H
