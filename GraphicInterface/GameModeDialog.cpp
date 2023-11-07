#include "GameModeDialog.h"

#include <QVBoxLayout>
#include <QLabel>

GameModeDialog::GameModeDialog(QWidget* parent) : QDialog(parent)
{
	setWindowTitle("Game Mode");

	m_timeComboBox = new QComboBox;
	m_timeComboBox->addItem("30 seconds", 30);
	m_timeComboBox->addItem("3 minutes", 180);
	m_timeComboBox->addItem("5 minutes", 300);
	m_timeComboBox->addItem("10 minutes", 600);
	m_timeComboBox->addItem("15 minutes", 900);
	m_timeComboBox->addItem("Untimed");

	m_confirmButton = new QPushButton("Confirm");
	connect(m_confirmButton, &QPushButton::clicked, this, &QDialog::accept);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(new QLabel("Choose a game mode:"));
	layout->addWidget(m_timeComboBox);
	layout->addWidget(m_confirmButton);
	setLayout(layout);

	setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
}

int GameModeDialog::getSelectedTimeInSeconds()
{
	int index = m_timeComboBox->currentIndex();
	if (index == -1) // Untimed
		return -1;
	return m_timeComboBox->currentData().toInt();
}
