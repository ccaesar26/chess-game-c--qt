#pragma once

#include <QDialog>
#include <QComboBox>
#include <QPushButton>

class GameModeDialog : public QDialog
{
	Q_OBJECT

public:
	GameModeDialog(QWidget* parent = nullptr);

	int getSelectedTimeInSeconds();

private:
	QComboBox* m_timeComboBox;
	QPushButton* m_confirmButton;
};
