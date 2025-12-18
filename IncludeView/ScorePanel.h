#ifndef SCOREPANEL_H
#define SCOREPANEL_H

#include <QWidget>
#include <QVector>
#include <QString>

class QLabel;

class ScorePanel : public QWidget
{
    Q_OBJECT

public:
    ScorePanel(int width, int height, QWidget* parent = nullptr);

public slots:
    void setScore(int scoreTotal,
                  int scoreHabitation,
                  int scoreMarche,
                  int scoreCaserne,
                  int scoreTemple,
                  int scoreJardin);
    void setNbPierres(int nbPierres);
    void setNomJoueurActif(const QString& nom);

private:
    QVector<QLabel*> scoreLabels;
    QLabel* labelJoueurActif = nullptr;
    QLabel* labelNombrePierre = nullptr;
};

#endif
