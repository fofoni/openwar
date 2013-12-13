#include <string>
#include <sstream>
#include "newgamedialog.h"
#include "colors.h"

NewGameDialog::NewGameDialog(QWidget *parent) :
    QDialog(parent)
{

    for (int i = 0; i < MAX_PLAYERS; i++)
        player_names << "";
    player_colors << WARColor_RED;
    player_colors << WARColor_GREEN;
    player_colors << WARColor_BLUE;
    player_colors << WARColor_YELLOW;
    player_colors << WARColor_WHITE;
    player_colors << WARColor_BLACK;

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

    button_box = new QDialogButtonBox(
                QDialogButtonBox::Ok |
                QDialogButtonBox::Cancel
    );
    layout->addWidget(button_box);

    connect(num_players_box, SIGNAL(valueChanged(int)),
            this, SLOT(set_num_players(int)));
    for (int i = 0; i < MAX_PLAYERS; i++) {
        connect(player_names_edit[i], SIGNAL(textChanged(QString)),
                this, SLOT(set_names_list()));
        connect(player_colors_combo[i], SIGNAL(currentIndexChanged(int)),
                this, SLOT(set_colors_list()));

    }

    connect(button_box, SIGNAL(accepted()), this, SLOT(accept()));
    connect(button_box, SIGNAL(rejected()), this, SLOT(reject()));

    setLayout(layout);
    setWindowTitle("New game");

    set_num_players(MIN_PLAYERS);

}

bool NewGameDialog::run(int &qtd,
                        QList<QString> &names, QList<QColor> &colors) {
    exec();
    if (result() == QDialog::Rejected)
        return false;
    qtd = num_players;
    names = player_names;
    colors = player_colors;
    return true;
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
    button_box->buttons()[0]->setEnabled(names_are_ok() && colors_are_ok());
}

void NewGameDialog::set_names_list() {
    for (int i = 0; i < MAX_PLAYERS; i++)
        player_names[i] = player_names_edit[i]->text();
    button_box->buttons()[0]->setEnabled(names_are_ok() && colors_are_ok());
}

void NewGameDialog::set_colors_list() {
    for (int i = 0; i < MAX_PLAYERS; i++)
        switch (player_colors_combo[i]->currentIndex()) {
            case 0: player_colors[i] = WARColor_RED; break;
            case 1: player_colors[i] = WARColor_GREEN; break;
            case 2: player_colors[i] = WARColor_BLUE; break;
            case 3: player_colors[i] = WARColor_YELLOW; break;
            case 4: player_colors[i] = WARColor_WHITE; break;
            case 5: player_colors[i] = WARColor_BLACK; break;
        }
    button_box->buttons()[0]->setEnabled(names_are_ok() && colors_are_ok());
}

bool NewGameDialog::names_are_ok() {
    for (int i = 0; i < num_players; i++) {
        // check if they are all nonempty
        if (player_names[i].isEmpty()) return false;
        // check if it is different from the others
        for (int j = i+1; j < num_players; j++)
            if (player_names[i] == player_names[j]) return false;
    }
    return true;
}

bool NewGameDialog::colors_are_ok() {
    for (int i = 0; i < num_players; i++) {
        // check if it is different from the others
        for (int j = i+1; j < num_players; j++)
            if (player_colors[i] == player_colors[j]) return false;
    }
    return true;
}
