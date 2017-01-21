#ifndef SERVEUR_H
#define SERVEUR_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QWidget>

class Serveur : public QWidget
{
    Q_OBJECT
public:
    explicit Serveur(int nombre_max_client,int port);
    void envoyerMessagesATous();

signals:

public slots:
    void nouvelleConnection();
    void donneesRecues();
    void deconnectionClient();


private:
    QTcpServer *serveur;
    QList <QTcpSocket *> clients;
    qint16 tailleMessage;

};

#endif // SERVEUR_H
