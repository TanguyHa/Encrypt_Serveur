#include <QApplication>
#include "serveur.h"

int main(int argc,char **argv){
    QApplication app(argc,argv);

    Serveur serv(5,55550);


    return app.exec();
}
