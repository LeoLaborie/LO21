#ifndef SCOREPANEL_H
#define SCOREPANEL_H

#include <QWidget>
#include <QVector>

class QLabel;

/**
 * @brief Widget responsable de l'affichage du bloc de scores.
 */
class ScorePanel : public QWidget
{
    Q_OBJECT

    public:
        ScorePanel(int width, int height, QWidget* parent = nullptr);
        QLabel* getjoueurActifLabel() const
        {
            return labelJoueurActif;
        }
        QLabel* getLabelPierre() const
        {
            return labelNombrePierre;
        }
        const QVector<QLabel *>& getScoreLabels() const
        {
            return scoreLabels;
        }

    public slots:
        void setScore(int scoreTotal,
                      int scoreHabitation,
                      int scoreMarche,
                      int scoreCaserne,
                      int scoreTemple,
                      int scoreJardin);
        void setNbPierres(int nbPierres);

    private:
        QVector<QLabel *> scoreLabels;
        QLabel* labelJoueurActif = nullptr;
        QLabel* labelNombrePierre = nullptr;
};

#endif
