#ifndef SCOREPANEL_H
#define SCOREPANEL_H

#include <QWidget>

class QLabel;

/**
 * @brief Widget responsable de l'affichage du bloc de scores.
 */
class ScorePanel : public QWidget
{
    Q_OBJECT

public:
    ScorePanel(int width, int height, QWidget* parent = nullptr);
    QLabel*JgetScoreLabel() const { return labelScore; }
    QLabel* getjoueurActifLabel() const { return labelJoueurActif; }

private:
    QLabel* labelScore = nullptr;
    QLabel* labelJoueurActif = nullptr;
};

#endif
