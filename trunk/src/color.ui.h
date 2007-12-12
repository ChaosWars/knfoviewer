/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/
#include <kcolorcombo.h>

QColor ColorPage::backgroundColor()
{
	return m_backgroundColorCombo->color();
}


QColor ColorPage::textColor()
{
	return m_textColorCombo->color();
}


QColor ColorPage::linkColor()
{
	return m_linkColorCombo->color();
}

void ColorPage::setBackgroundColor( const uint &color )
{
	m_backgroundColorCombo->setColor( color );
}


void ColorPage::setTextColor( const uint &color )
{
	m_textColorCombo->setColor( color );
}


void ColorPage::setLinkColor( const uint &color )
{
	m_linkColorCombo->setColor( color );
}


KColorCombo* ColorPage::backgroundColorCombo()
{
	return m_backgroundColorCombo;
}


KColorCombo* ColorPage::textColorCombo()
{
	return m_textColorCombo;
}


KColorCombo* ColorPage::linkColorCombo()
{
	return m_linkColorCombo;
}


