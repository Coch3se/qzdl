#include <QtGui>
#include <QApplication>
#include <QMainWindow>
#include "configurationManager.h"

#include "multiPane.h"
#include "zdlInterface.h"
#include "bottomPane.h"
#include "topPane.h"

zdlInterface::zdlInterface(QWidget *parent):ZQWidget(parent){
	configurationManager::setInterface(this);
	box = new QVBoxLayout(this);
	topPane *tpane = new topPane(this);
	bottomPane *bpane = new bottomPane(this);
	mpane = new multiPane(this);
	setContentsMargins(0,0,0,0);
	layout()->setContentsMargins(0,0,0,0);
	box->setSpacing(0);
	box->addWidget(tpane);
	box->addWidget(bpane);
}

//Called when there's a change to the configuration that we need to look at.
//The button changed the configuration, and then notifies us that we need
//to look at the configuration to see what we need to do.
void zdlInterface::newConfig(){
	//Grab our configuration
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	//Grab our section in the configuration
	ZDLSection *section = zconf->getSection("zdl.save");
	//Do we have it?
	if (section){
		vector <ZDLLine*> vctr;
		//Search for our string
		section->getRegex("^dlgmode$", vctr);
		for(int i = 0; i < vctr.size(); i++){
			//Are we open?
			if (strcmp(vctr[i]->getValue(), "open") == 0){
				cout << "Opening config" << endl;
				box->addWidget(mpane);
				mpane->setVisible(true);
			//No?
			}else{
				cout << "Removing config" << endl;
				box->removeWidget(mpane);
				mpane->setVisible(false);
				mpane->hide();
			}
		}
		//Do we have the section, but not the key?
		if (vctr.size() < 1){
			cout << "Removing config (no line present)" << endl;
			box->removeWidget(mpane);
			mpane->setVisible(false);
		}
	//Do we not even have the section?
	}else{
		cout << "Removing config (no section present)" << endl;
		box->removeWidget(mpane);
	}
	this->update();
}



void zdlInterface::startRead(){
	emit readChildren(this);
	newConfig();
}

void zdlInterface::writeConfig(){
	emit buildChildren(this);
}