#include <string>
#include <sstream>
#include "newgamedialog.h"

NewGameDialog::NewGameDialog(QWidget *parent) :
    QDialog(parent)
{

    signal_mapper = new QSignalMapper(this);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QHBoxLayout *choose_layout = new QHBoxLayout();
        num_players_box = new QSpinBox;
        num_players_box->setMinimum(MIN_PLAYERS);
        num_players_box->setMaximum(MAX_PLAYERS);
        num_players_box->setMaximumWidth(60);
        choose_layout->addWidget(num_players_box);
        QLabel *label = new QLabel("players");
        choose_layout->addWidget(label);
    layout->addLayout(choose_layout);

    QGridLayout *form_layout = new QGridLayout;
        QLabel *header_name_label = new QLabel("Name:");
        form_layout->addWidget(header_name_label, 0, 1, Qt::AlignLeft);
        QLabel *header_color_label = new QLabel("Color:");
        form_layout->addWidget(header_color_label, 0, 2, Qt::AlignLeft);
        for (int i = 1; i <= MAX_PLAYERS; i++) {
            std::stringstream label_txt; label_txt << "Player " << i << ":";
            player_labels << new QLabel(label_txt.str().c_str());
            form_layout->addWidget(player_labels[i-1], i, 0, Qt::AlignRight);
            player_names_edit << new QLineEdit;
            form_layout->addWidget(player_names_edit[i-1], i, 1);
            player_colors_combo << new QComboBox;
            player_colors_combo[i-1]->addItem("Red");
            player_colors_combo[i-1]->addItem("Green");
            player_colors_combo[i-1]->addItem("Blue");
            player_colors_combo[i-1]->addItem("Yellow");
            player_colors_combo[i-1]->addItem("White");
            player_colors_combo[i-1]->addItem("Black");
            player_colors_combo[i-1]->setCurrentIndex(i-1);
            form_layout->addWidget(player_colors_combo[i-1], i, 2);
        }
    layout->addLayout(form_layout);

    QDialogButtonBox *button_box = new QDialogButtonBox(
                QDialogButtonBox::Ok |
                QDialogButtonBox::Cancel
    );
    layout->addWidget(button_box);

    connect(num_players_box, SIGNAL(valueChanged(int)),
            this, SLOT(set_num_players(int)));

    connect(button_box, SIGNAL(accepted()), this, SLOT(accept()));
    connect(button_box, SIGNAL(rejected()), this, SLOT(reject()));

    setLayout(layout);
    setWindowTitle("New game");

}

void NewGameDialog::run(int &num_players,
                        QList<QString> &names, QList<QColor> &colors) {

    set_num_players(MIN_PLAYERS);
    show();

}

void NewGameDialog::set_num_players(int n) {

    num_players = n;

    for (int i = 0; i < n; i++) {
        player_labels[i]->setEnabled(true);
        player_names_edit[i]->setEnabled(true);
        player_colors_combo[i]->setEnabled(true);
    }
    for (int i = n; i < MAX_PLAYERS; i++) {
        player_labels[i]->setEnabled(false);
        player_names_edit[i]->setEnabled(false);
        player_colors_combo[i]->setEnabled(false);
    }
}
