/***************************************************************************
 *   Copyright (C) 2004 by Dominik Seichter                                *
 *   domseichter@web.de                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "selectgametypebox.h"

#include "card.h"
#include "cardlist.h"
#include "game.h"
#include "gameinfo.h"

#include <qwidget.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qvalidator.h>
#include <qapplication.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qbuttongroup.h>
#include <klocale.h>
#include <kwizard.h>
#include <qpainter.h>
#include <qpixmap.h>



SelectGameTypeBox::SelectGameTypeBox( QWidget *parent, const char *name ):QHBox(parent, name, 0)
{
	setSpacing(8);
	page = new QHBox( this );
	row1 = new QVBox( page );

	preview = new QLabel( row1);
	info = new QLabel( row1);
	info->setMargin( 11 );
	info->setAlignment( AlignBottom | AlignLeft | Qt::WordBreak);


	info->setMinimumHeight(100);
	group_TypeSelect  = new QButtonGroup( 1, Qt::Horizontal, i18n("Game"), page );


	checkRufspiel = new QRadioButton( i18n("Play a Callgame"), group_TypeSelect );
	checkSolo = new QRadioButton( i18n("Play a Solo"), group_TypeSelect );
	checkGeier = new QRadioButton( i18n("Play a Geier"), group_TypeSelect );
	checkWenz = new QRadioButton( i18n("Play a Wenz"), group_TypeSelect );
	checkDachs = new QRadioButton( i18n("Play a Dachs"), group_TypeSelect );
	connect( group_TypeSelect, SIGNAL(clicked(int)), this, SLOT(typeChanged()));

	checkRufspiel->setChecked(TRUE);

	typeChanged();
	info->setMaximumWidth(preview->size().width());

}

void SelectGameTypeBox::typeChanged()
{
	GameInfo *info=gameInfo();
	updatePreview();
	switch(info->mode())
	{
			case (GameInfo::RUFSPIEL):
						setInfoText(HELP_GAMETYPE_RUFSPIEL);
			break;
			case (GameInfo::WENZ):
						setInfoText(HELP_GAMETYPE_WENZ);
			break;
			case (GameInfo::STICHT):
						setInfoText(HELP_GAMETYPE_SOLO);
			break;
			case (GameInfo::GEIER):
						setInfoText(HELP_GAMETYPE_GEIER);
			break;
			case (GameInfo::DACHS):
						setInfoText(HELP_GAMETYPE_DACHS);
			break;

	}


	delete info;
}

void SelectGameTypeBox::setInfoText(const QString &Text)
{
	info->setText(Text);
}

SelectGameTypeBox::~SelectGameTypeBox()
{
	delete checkRufspiel;
	delete checkSolo;
	delete checkGeier;
	delete checkWenz;
	delete checkDachs;
	delete group_TypeSelect;
	delete page;
	delete row1;
	delete group_TypeSelect;
	delete preview;
	delete info;
}


void SelectGameTypeBox::updatePreview()
{
	int i = 0;
	int x = 0, y = 0;
	CardList list;
	CardList trumpf;
	GameInfo* info = gameInfo();

	list.init();
	for(i=0;i<list.count();i++)
		if( info->istTrumpf( list.at(i) ) )
			trumpf.append( list.at(i) );
	delete info;
	trumpf.sort((eval_func)info->evalCard, (void *)info);

	Card c( Card::SAU, Card::EICHEL );
	QPixmap pix(c.pixmap()->width()*4,c.pixmap()->height()*(int(trumpf.count()/4)+(trumpf.count()%4)));
	pix.fill( Qt::darkGreen );
	QPainter p( &pix );
	for(i=trumpf.count()-1;i>=0;i--)
	{
		QPixmap* pixmap = trumpf.at(i)->pixmap();
		p.drawPixmap( x, y, *pixmap );
		x += pixmap->width()/2;

		int pos = trumpf.count() - i;
		if(!(pos+1)%4)
		{
			x = 0;
			y += pixmap->height();
		}
	}

	p.end();
	pix.resize( pix.width(), y + ( x == 0 ? 0 : c.pixmap()->height() ) );
	preview->setPixmap( pix );
	preview->setMinimumWidth(pix.width()+10);
}


GameInfo* SelectGameTypeBox::gameInfo() const
{
	GameInfo* info = new GameInfo();

	if( checkRufspiel->isChecked() )
		info->setMode(GameInfo::RUFSPIEL);
	else if( checkSolo->isChecked() )
		info->setMode(GameInfo::STICHT);
	else if( checkGeier->isChecked() )
		info->setMode(GameInfo::GEIER);
	else if( checkWenz->isChecked() )
		info->setMode(GameInfo::WENZ);
	else if( checkDachs->isChecked() )
		info->setMode(GameInfo::DACHS);

	return info;
}


#include "selectgametypebox.moc"