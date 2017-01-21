#include "serveur.h"
#include <iostream>

Serveur::Serveur(int nombre_max_client,int port){
    serveur=new QTcpServer;
    if(!serveur->listen(QHostAddress::Any,port)){
        std::cout<<"Le serveur n'as pas pu démarer au port "<<port<<"\n Raison:\n";
        std::cout<<serveur->errorString().toStdString()<<"\n";
    }
    else{
        std::cout<<"Le serveur a pu demarer correctement au port "<<port<<" ...\n";
        connect(serveur,SIGNAL(newConnection()),this,SLOT(nouvelleConnection()));
    }

}
void Serveur::nouvelleConnection(){
   std::cout<<"Nouveau client\n";

   QTcpSocket *nouveauClient = serveur->nextPendingConnection();
   clients << nouveauClient;


   connect(nouveauClient, SIGNAL(readyRead()), this, SLOT(donneesRecues()));
   connect(nouveauClient, SIGNAL(disconnected()), this, SLOT(deconnexionClient()));
}

void Serveur::donneesRecues(){

    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0)
        return;

    QDataStream in(socket);
    if (tailleMessage == 0)
    {
        if (socket->bytesAvailable() < (int)sizeof(quint16))
             return;

        in >> tailleMessage;
    }

    if (socket->bytesAvailable() < tailleMessage)
        return;

    QString message;
    in >> message;
    envoyerATous(message);
    tailleMessage = 0;
}
void Serveur::deconnectionClient(){
    // On détermine quel client se déconnecte
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());

    if (socket == 0) // Si par hasard on n'a pas trouvé le client à l'origine du signal, on arrête la méthode
      return;


    clients.removeOne(socket);
    socket->deleteLater();
}
Serveur::envoyerMessagesATous(){
    // Préparation du paquet
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0; // On écrit 0 au début du paquet pour réserver la place pour écrire la taille
    out << message; // On ajoute le message à la suite
    out.device()->seek(0); // On se replace au début du paquet
    out << (quint16) (paquet.size() - sizeof(quint16)); // On écrase le 0 qu'on avait réservé par la longueur du message


    // Envoi du paquet préparé à tous les clients connectés au serveur
    for (int i = 0; i < clients.size(); i++)
    {
       clients[i]->write(paquet);
    }
}
