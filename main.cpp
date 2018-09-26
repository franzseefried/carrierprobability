/*#######################################################
 * Programm liest 2 files
 * RRDTM Pedigree
 * Genotypen
 * Programm berechnet für alle Tiere eine Carrierprobablity unter Verwendung des Pedigrees
 *######################################################*/
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <stdexcept>
#include <typeinfo>
using namespace std;


//www.youtube.com/watch?v=rnGoCWNtNR0
class animal{
    int ID,SIRE,DAM,DOB,YOB,ALIVE,F;
    string ITBID16,TVDID12,SEKTION,HBSTUFE,BREED3;
    animal* next;
public:
    int initPed(string);
    void calcF();
    void calcGTprob();
};
class recessive{
    int GT,ID;
    string TVD,ITBID,TVDsire,SEKTION,pctpt,density,gebdat;
public:
    double initRecessive(string);
};

//Methode zum initialisieren == renummerierte Pedigreedatei einlesen
int animal::initPed(string input){
    animal Lanimal;
    ifstream datei(input);
    if(!datei){
        cout << "Cannot open Pedigree file!" << endl;
        exit(1);
    }
    string zeile;
    int nped=0;
    while(getline(datei,zeile)){
       stringstream zeilenpuffer(zeile);
       zeilenpuffer >> Lanimal.ID >> Lanimal.SIRE >> Lanimal.DAM >> Lanimal.YOB >> Lanimal.ITBID16 >> Lanimal.TVDID12 >> Lanimal.DOB >> Lanimal.SEKTION >> Lanimal.ALIVE >> Lanimal.HBSTUFE >> Lanimal.BREED3 ;

       nped++;
       if(nped % 1000000 == 0)
           cout << Lanimal.ID << " " << Lanimal.SIRE << " " << Lanimal.DAM << " " << Lanimal.DOB << " " << Lanimal.ITBID16 << " " << Lanimal.TVDID12 << " " << Lanimal.SEKTION << " " << Lanimal.ALIVE << " " << Lanimal.HBSTUFE << " " << Lanimal.BREED3 << endl;
    }
    cout << "Finished reading Pedigreefile at No. of records " << nped << endl;
    return nped;
}
//Methode zum einlesen der Haplotypenergebnisse
double recessive::initRecessive(string input){
    recessive Lrecessive;
    ifstream datei(input);
    if(!datei){
        cout << "Cannot open Haplotypefile!" << endl;
    }
    string zeile;
    int nrec=0;
    int gtsum=0;
    while(getline(datei,zeile)){
        stringstream zeilenpuffer(zeile);
        gtsum=Lrecessive.GT+gtsum;
        zeilenpuffer >> Lrecessive.TVD >> Lrecessive.GT >> Lrecessive.ID >> Lrecessive.ITBID >> Lrecessive.TVD >> Lrecessive.TVDsire >> Lrecessive.SEKTION >> Lrecessive.pctpt >> Lrecessive.density >> Lrecessive.gebdat ;
        nrec++;
        if(nrec % 1000000 == 0)
            cout << Lrecessive.TVD << " " << Lrecessive.GT << " " << Lrecessive.ID << " " << Lrecessive.ITBID << " " << Lrecessive.TVD << " " << Lrecessive.TVDsire << " " << Lrecessive.SEKTION << " " << Lrecessive.pctpt << " " << Lrecessive.density << " " << Lrecessive.gebdat  << endl;
     }
    double AlleleFrq=0.0;
    AlleleFrq=(static_cast<double>(gtsum)/(static_cast<double>(nrec*2)));
    cout << "Finished reading Haplotypefile at No. of records " << nrec << endl;
    cout << "Allelefrequency of counted allele is:            " << AlleleFrq << endl;
    return AlleleFrq;
}




int main()
{
    animal* LCANIMAL;
    LCANIMAL=new animal();
    LCANIMAL->initPed("/Users/fseefried/Desktop/UpdatedRenumMergedPedi_20180811.txt");

    recessive* LCrecessive;
    LCrecessive=new recessive();
    double lAllelefreq;
    lAllelefreq=LCrecessive->initRecessive("/Users/fseefried/Desktop/RUN1918BSW.19-BH2.Fimpute.all.haploCOUNTS");

    delete(LCANIMAL);
    delete(LCrecessive);
    return 0;
}
