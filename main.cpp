/*#######################################################
 * Programm liest 2 files
 * RRDTM Pedigree
 * Genotypen
 * Programm berechnet für alle Tiere eine Genotypeprobability unter Verwendung der Abstammung bzw. der AhnenGenotypens
 *######################################################*/
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <stdexcept>
#include <typeinfo>
using namespace std;

// Fkt zum lesen des Parameterfiles. Diese hat 2 Spalten. Spalte 1 ist der Parameter, Spalte 2 der Wert
string readPrmFile(string prmtxt,string pstr){
    string zeile,parametername,parameterbezeichnung;
    ifstream datei(prmtxt);
    if(!datei){
        cout << "Cannot open Parameterfile!" << endl;
        exit(1);
    }
    string bckval;
    while(getline(datei,zeile)){
       stringstream zeilenpuffer(zeile);
       zeilenpuffer >> parametername >> parameterbezeichnung;
       if(parametername == pstr)
           bckval=parameterbezeichnung;
    }
    return bckval;
};


//www.youtube.com/watch?v=rnGoCWNtNR0
class animal{
    int ID,SIRE,DAM,DOB,YOB,ALIVE;
    string ITBID16,TVDID12,SEKTION,HBSTUFE,BREED3;
    animal* next;
public:
    void datenSetzen();
    animal* initPed(int,int,int,int,string,string,int,string,int,string,string);
    void printPed();
    void destroyPedi();
};
class recessive{
    int GT,ID;
    string TVD,ITBID,TVDsire,SEKTION,pctpt,density,gebdat;
public:
    double initRecessive(string);
};
//methode zum Datensetzen == NULLrecord setzen
void animal::datenSetzen(){
    ID=0;SIRE=0;DAM=0;DOB=0;YOB=0;ALIVE=0;
    ITBID16="";TVDID12="";SEKTION="";HBSTUFE="";BREED3="";
    next=nullptr;
    cout << "\n Dummyanimals was created" << endl;
}
//Methode zum initialisieren == renummerierte Pedigreedatei einlesen immer das listenende als returnvalue zurueckgeben
//damit das Listenende nicht imme neu ermittelt werden muss. also eine einfach verkettete Liste wobei wir immer das Listenende zurueckgeben
animal* animal::initPed(int IDNEU, int SIRENEU, int DAMNEU, int YOBNEU, string ITBID16NEU, string TVDID12NEU, int DOBNEU, string SEKTIONNEU, int ALIVENEU, string HBSTUFENEU, string BREED3NEU){
    next = new animal();
    next->ID      = IDNEU;
    next->SIRE    = SIRENEU;
    next->DAM     = DAMNEU;
    next->DOB     = DOBNEU;
    next->YOB     = YOBNEU;
    next->ALIVE   = ALIVENEU;
    next->ITBID16 = ITBID16NEU;
    next->TVDID12 = TVDID12NEU;
    next->SEKTION = SEKTIONNEU;
    next->HBSTUFE = HBSTUFENEU;
    next->BREED3  = BREED3NEU;
    next->next    = nullptr;
    return next;
}
void animal::printPed(){
    cout << ID << ";" << SIRE << ";" <<  ITBID16 << ";" << ALIVE << endl;
    if (next != nullptr)
        next->printPed();
}
void animal::destroyPedi(){
    if (next != nullptr){
        next->destroyPedi();
        delete(next);
    }

}


int main()
{
    string parfile,pedifile,haplofile,zeile,lITB,lTVD,lSEKTION,lHB,lBD3;
    int lID,lSIRE,lDAM,lYOB,lDOB,lALIVE;
    parfile="/Users/fseefried/Documents/projects/carrierprobability/carrierprobability.prm";
    cout << parfile << endl;
    pedifile=readPrmFile(parfile,"Pedigree");
    haplofile=readPrmFile(parfile,"Haplotypefile");
    cout << "\n Pedigree was given as:      " << pedifile << endl;
    cout << "\n Haplotypefile was given as: " << haplofile << endl;

    //Zeiger auf den listenbeginn
    animal* ANIMALANFANG;
    //1. element manuell erzeugen
    ANIMALANFANG=new animal();
    //daten rein
    ANIMALANFANG->datenSetzen();
    //zeiger auf den letzten Eintrag in der animalliste
    animal* ANIMALENDE;
    ANIMALENDE=new animal();
    //positioniere listenende am Beginn auf listenanfang da erstes element zugleich letztes element ist
    ANIMALENDE=ANIMALANFANG;

    //Pedigreedatei lesen
    ifstream datei(pedifile);
        if(!datei){
          cout << "\n Cannot open Pedigree file!" << endl;
          exit(1);
        }
    while(getline(datei,zeile)){
        stringstream zeilenpuffer(zeile);
        zeilenpuffer >> lID >> lSIRE >> lDAM >> lYOB >> lITB >> lTVD >> lDOB >> lSEKTION >> lALIVE >> lHB >> lBD3 ;
        //lokalen Hilfszeiger
        animal* lczeiger;
        //retrun value gibt das ende der liste zurueck, diese im lczeiger speichern
        lczeiger=ANIMALENDE->initPed(lID,lSIRE,lDAM,lYOB,lITB,lTVD,lDOB,lSEKTION,lALIVE,lHB,lBD3);
        //update das ANIMALENDE mit zeiger aus der methode anhaengen
        ANIMALENDE=lczeiger;
    }
    cout << "\n Pedigreedatei gelesen" << endl;
    ANIMALANFANG->printPed();

    ANIMALANFANG->destroyPedi();

    return 0;
}
