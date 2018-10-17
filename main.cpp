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
    int id,sire,dam,dob,yob,alive;
    string itbid16,tvdid12,sektion,hbstufe,breed3;
    animal* next;
public:
    //NULL record setzen
    void datenSetzen();
    //einlesen der Pedigreedatei
    animal* initPed(int,int,int,int,string,string,int,string,int,string,string);
    //Pedigree drucken
    void printPed();
    //Pedigree abbauen
    void destroyPedi();
};
class recessive{
    int gt,id;
    string tvd,itbid,tvdsire,sektion,pctpt,density,gebdat;
    recessive* next;
public:
    //NULL record setzen
    void recSetzen();
    //einlesen der Haplodatei
    recessive* initRecessive(string,int);
    //Haplofile drucken
    void printHaplo();
    //Haplo abbauen
    void destroyHaplo();
};
//methode zum Datensetzen == NULLrecord setzen
void animal::datenSetzen(){
    id=0;sire=0;dam=0;dob=0;yob=0;alive=0;
    itbid16="";tvdid12="";sektion="";hbstufe="";breed3="";
    next=nullptr;
    cout << "\n Dummyanimals was created" << endl;
}
//Methode zum initialisieren == renummerierte Pedigreedatei einlesen immer das listenende als returnvalue zurueckgeben
//damit das Listenende nicht imme neu ermittelt werden muss. also eine einfach verkettete Liste wobei wir immer das Listenende zurueckgeben
animal* animal::initPed(int idneu, int sireneu, int damneu, int yobneu, string itbid16neu, string tvdid12neu, int dobneu, string sektionneu, int aliveneu, string hbstufeneu, string breed3neu){
    next = new animal();
    next->id      = idneu;
    next->sire    = sireneu;
    next->dam     = damneu;
    next->dob     = dobneu;
    next->yob     = yobneu;
    next->alive   = aliveneu;
    next->itbid16 = itbid16neu;
    next->tvdid12 = tvdid12neu;
    next->sektion = sektionneu;
    next->hbstufe = hbstufeneu;
    next->breed3  = breed3neu;
    next->next    = nullptr;
    return next;
}
void animal::printPed(){
    cout << id << ";" << sire << ";" <<  itbid16 << ";" << alive << endl;
    if (next != nullptr)
        next->printPed();
}
void animal::destroyPedi(){
    if (next != nullptr){
        next->destroyPedi();
        delete(next);
    }

}
void recessive::recSetzen(){
    gt=0;id=0;
    tvd="";itbid="";tvdsire="";sektion="";pctpt="";density="";gebdat="";
    next = nullptr;
    cout << "\n Dummyrecord was generated";
}
recessive* recessive::initRecessive(string tvdN,int gtN){
    next = new recessive();
    next->tvd    = tvdN;
    next->gt     = gtN;
    next->next   = nullptr;
    return next;
}
void recessive::printHaplo(){
    cout << tvd << ";" << gt << endl;
    if(next != nullptr)
        next->printHaplo();
}
void recessive::destroyHaplo(){
    if (next != nullptr){
        next->destroyHaplo();
        delete(next);
    }
}

int main()
{
    string parfile,pedifile,haplofile,zeile,lITB,lTVD,lSEKTION,lHB,lBD3,ltvd,litbnummer,ltvdsire,lsektion,lpct,ldichte,ldateofbirth;;
    int lID,lSIRE,lDAM,lYOB,lDOB,lALIVE,lgt,lidanimal;
    parfile="/Users/fseefried/Documents/projects/carrierprobability/carrierprobability.prm";
    cout << parfile << endl;
    pedifile=readPrmFile(parfile,"Pedigree");
    haplofile=readPrmFile(parfile,"Haplotypefile");
    cout << "\n Pedigree was given as:      " << pedifile << endl;
    cout << "\n Haplotypefile was given as: " << haplofile << endl;

    //Zeiger auf den listenbeginn
    animal* animalanfang;
    recessive* recanfang;
    //1. element manuell erzeugen
    animalanfang=new animal();
    recanfang=new recessive();
    //daten rein
    animalanfang->datenSetzen();
    recanfang->recSetzen();
    //zeiger auf den letzten Eintrag in der animalliste
    animal* animalende;
    animalende=new animal();
    recessive* recende;
    recende = new recessive();
    //positioniere listenende am Beginn auf listenanfang da erstes element zugleich letztes element ist
    animalende=animalanfang;
    recende=recanfang;

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
            lczeiger=animalende->initPed(lID,lSIRE,lDAM,lYOB,lITB,lTVD,lDOB,lSEKTION,lALIVE,lHB,lBD3);
            //update das ANIMALENDE mit zeiger aus der methode anhaengen
            animalende=lczeiger;
    }
    cout << "\n Pedigreedatei gelesen" << endl;
    //animalanfang->printPed();

    //Haplotypefile lesen
    ifstream dateih(haplofile);
        if(!dateih){
            cout << "\n Cannot open Haplotypefile" << endl;
            exit(1);
        }
        while (getline(dateih,zeile)){
            stringstream zeilenpuffer(zeile);
            zeilenpuffer >> ltvd >> lgt >> lidanimal >> litbnummer >> ltvdsire >> lsektion >> lpct >> ldichte >> ldateofbirth;
            recessive* lhzeiger;
            lhzeiger=recende->initRecessive(ltvd,lgt);
            recende=lhzeiger;
        }
    cout << "\n Haplofile gelesen" << endl;
    recanfang->printHaplo();

    animalanfang->destroyPedi();
    recanfang->destroyHaplo();
    return 0;
}
